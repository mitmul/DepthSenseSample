#include "GLViewer.h"

GLViewer::GLViewer()
	: frame_count(0),
	fps(0.0),
	current_time(0),
	previous_time(0),
	xRot(0),
	yRot(0),
	zRot(0),
	xOrigin(0.0),
	yOrigin(0.0),
	zOrigin(-1.5),
	last_x(0),
	last_y(0),
	mouse_button(0),
	mouse_click(1),
	scale(1.0)
{
	image.create(640, 480, CV_8UC3);
	image = Mat::zeros(image.size(), image.type());
	putText(image, "initial state", Point(0, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 200, 200), 1, CV_AA);
	flip(image, image, 0);
}

void GLViewer::init(int argc, char* argv[])
{
	try
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		glutCreateWindow("Depth Sense Viewer");
		glutReshapeWindow(640, 480);

		boost::function<void (void)> display_func = boost::bind(&GLViewer::display, this);
		boost::function<void (int, int)> resize_func = boost::bind(&GLViewer::resize, this, _1, _2);
		boost::function<void (void)> idle_func = boost::bind(&GLViewer::idle, this);
		boost::function<void (int, int, int, int)> mouse_func = boost::bind(&GLViewer::mouse, this, _1, _2, _3, _4);
		boost::function<void (int, int, int, int)> wheel_func = boost::bind(&GLViewer::wheel, this, _1, _2, _3, _4);
		boost::function<void (int, int)> motion_func = boost::bind(&GLViewer::motion, this, _1, _2);
		boost::function<void (unsigned char, int, int)> keyboard_func = boost::bind(&GLViewer::keyboard, this, _1, _2, _3);
		boost::function<void (int, int, int)> special_func = boost::bind(&GLViewer::special, this, _1, _2, _3);

		glut::DisplayFunc(display_func);
		glut::ReshapeFunc(resize_func);
		glut::IdleFunc(idle_func);
		glut::MouseFunc(mouse_func);
		glut::MouseWheelFunc(wheel_func);
		glut::MotionFunc(motion_func);
		glut::KeyboardFunc(keyboard_func);
		glut::SpecialFunc(special_func);

		glClearColor(0, 0, 0, 0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glutMainLoop();
	}
	catch(std::exception &ex)
	{
		cerr << "GLViewer::init()" << endl
			<< ex.what() << endl;
	}
}

void GLViewer::start()
{
	glutMainLoop();
}

void GLViewer::setImage(const Mat &_image)
{
	image = _image.clone();
}

void GLViewer::setPoints(const vector<Point3d> &_points)
{
	points = _points;
	calcFPS();
}

void GLViewer::setColors(const vector<Vec3b> &_colors)
{
	colors = _colors;
}

void GLViewer::display()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFPS();

	glPushMatrix();
	glTranslatef(xOrigin, yOrigin, zOrigin);
	glRotatef(M_PI, 0.0, 1.0, 0.0);
	glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
	drawPoints();
	glPopMatrix();

	glutSwapBuffers();
}

void GLViewer::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(58.0, 74.0 / 58.0, 0.001, 1000.0);
}

void GLViewer::idle()
{
	glutPostRedisplay();
}

void GLViewer::mouse(int button, int state, int x, int y)
{
	mouse_button = button;
	mouse_click = state;

	last_x = x;
	last_y = y;
}

void GLViewer::wheel(int button, int dir, int x, int y)
{
	zOrigin += (double)dir / 10.0;
	cout << zOrigin << endl;
}

void GLViewer::motion(int x, int y)
{
	int dx = x - last_x;
	int dy = y - last_y;
	if(mouse_click == 0 && mouse_button == 0)
	{
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	}
	else if(mouse_click == 0 && mouse_button == 2)
	{
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	last_x = x;
	last_y = y;
}

void GLViewer::keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 's':
		zOrigin -= 0.1;
		break;
	case 'b':
		zOrigin += 0.1;
		break;
	}
}

void GLViewer::special(int key, int x, int y)
{
	switch(key)
	{
	case 100:
		xOrigin -= 0.1;
		break;
	case 101:
		yOrigin += 0.1;
		break;
	case 102:
		xOrigin += 0.1;
		break;
	case 103:
		yOrigin -= 0.1;
		break;
	}
}

void GLViewer::qNormalizeAngle(int &angle)
{
	while(angle < 0)
		angle += 360 * 16;
	while(angle > 360 * 16)
		angle -= 360 * 16;
}

void GLViewer::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if(angle != xRot)
	{
		xRot = angle;
	}
}

void GLViewer::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if(angle != yRot)
	{
		yRot = angle;
	}
}

void GLViewer::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if(angle != zRot)
	{
		zRot = angle;
	}
}

void GLViewer::calcFPS()
{
	//  Increase frame count
	frame_count++;

	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	current_time = glutGet(GLUT_ELAPSED_TIME);

	//  Calculate time passed
	int timeInterval = current_time - previous_time;

	//  calculate the number of frames per second
	fps = frame_count / (timeInterval / 1000.0f);

	//  Set time
	previous_time = current_time;

	//  Reset frame count
	frame_count = 0;
}

void GLViewer::drawFPS()
{
	glColor3d(1, 1, 1);
	stringstream ss; ss << fps << " fps";
	drawText(ss.str(), -1, -1);
}

void GLViewer::drawText(const string &_text, const double x, const double y)
{
	glRasterPos2d(x, y);
	for(char *p = (char*)_text.c_str(); *p; ++p)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
}

void GLViewer::drawImage()
{
	// ‰æ‘œ•`‰æ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 1.0);
	glVertex2d(0, 1);

	glTexCoord2d(0.0, 0.0);
	glVertex2d(0, 0);

	glTexCoord2d(1.0, 0.0);
	glVertex2d(1, 0);

	glTexCoord2d(1.0, 1.0);
	glVertex2d(1, 1);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void GLViewer::drawPoints()
{
	if(!points.empty())
	{
		bool color = false;
		if(points.size() == colors.size())
			color = true;

		glBegin(GL_POINTS);
		int points_size = (int)points.size();
		for(int i = 0; i < points_size; ++i)
		{
			if(color)
			{
				Vec3b c = colors[i];
				glColor3d((double)c[2] / 255.0, (double)c[1] / 255.0, (double)c[0] / 255.0);
			}
			Point3d p = points[i];
			glVertex3d(p.x / 1000.0, p.y / 1000.0, p.z / 1000.0);
		}
		glEnd();
	}
}