#ifndef GLVIEWER_H
#define GLVIEWER_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <boost\thread.hpp>
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <gl\glut.h>
#include "glut.hpp"

using namespace std;
using namespace cv;

class GLViewer
{
public:
	GLViewer();

	void init(int argc, char* argv[]);
	void setImage(const Mat &_image);
	void setPoints(const vector<Point3d> &_points);
	void setColors(const vector<Vec4b> &_colors);
	void start();

private:
	void resize(int w, int h);
	void display();
	void idle();
	void mouse(int button, int state, int x, int y);
	void wheel(int button, int dir, int x, int y);
	void motion(int x, int y);
	void keyboard(unsigned char key, int x, int y);
	void special(int key, int x, int y);

	void qNormalizeAngle(int &angle);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	void calcFPS();
	void drawFPS();
	void drawText(const string &_text, const double x, const double y);
	void drawImage();
	void drawPoints();

	GLuint texture;
	Mat image;

	int frame_count;
	float fps;
	int current_time;
	int previous_time;

	int xRot, yRot, zRot;
	float xOrigin, yOrigin, zOrigin;
	int last_x, last_y;
	int mouse_button;
	int mouse_click;

	vector<Point3d> points;
	vector<Vec4b> colors;
};

#endif // GLVIEWER_H