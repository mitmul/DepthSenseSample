#include "Sensor.h"


Sensor::Sensor(void)
	: device_found(false),
	proj_helper(NULL)
{
	color_img.create(480, 640, CV_8UC3);
	depth_img.create(480, 640, CV_8UC1);
}

Sensor::~Sensor(void)
{
	device_context.stopNodes();

	if(c_node.isSet()) device_context.unregisterNode(c_node);
	if(d_node.isSet()) device_context.unregisterNode(d_node);
	if(a_node.isSet()) device_context.unregisterNode(a_node);
	if(proj_helper) delete proj_helper;
}

void Sensor::setup(GLViewer &_viewer)
{
	cout << "setup...";
	viewer = &_viewer;
	device_context = Context::create("localhost");
	device_context.deviceAddedEvent().connect<Sensor>(this, &Sensor::onDeviceConnected);
	device_context.deviceRemovedEvent().connect<Sensor>(this, &Sensor::onDeviceDisconnected);

	vector<Device> da = device_context.getDevices();
	if(da.size() >= 1)
	{
		device_found = true;
		da[0].nodeAddedEvent().connect<Sensor>(this, &Sensor::onNodeConnected);
		da[0].nodeRemovedEvent().connect<Sensor>(this, &Sensor::onNodeDisconnected);

		vector<Node> na = da[0].getNodes();
		for(int n = 0; n < (int)na.size(); ++n)
		{
			configureNode(na[n]);
		}
	}
	cout << "finish!" << endl;
	device_context.startNodes();
	device_context.run();
}

void Sensor::onNewAudioSample(AudioNode node, AudioNode::NewSampleReceivedData data)
{
}

void Sensor::onNewColorSample(ColorNode node, ColorNode::NewSampleReceivedData data)
{
	color_img.data = (uchar *)(const uint8_t *)data.colorMap;

	// カラー画像を表示
	imshow("Color Image", color_img);
	waitKey(1);
}

void Sensor::onNewDepthSample(DepthNode node, DepthNode::NewSampleReceivedData data)
{
	// デプスデータの射影パラメータ
	if(!proj_helper)
	{
		proj_helper = new ProjectionHelper(data.stereoCameraParameters);
		stereo_param = data.stereoCameraParameters;
	}
	else if(stereo_param != data.stereoCameraParameters)
	{
		proj_helper->setStereoCameraParameters(data.stereoCameraParameters);
		stereo_param = data.stereoCameraParameters;
	}

	// デプス画像を表示
	Mat depth(240, 320, CV_16SC1, (void *)(const int16_t *)data.depthMap);
	depth.convertTo(depth_img, CV_8U, 255.0 / 1000.0);
	cv::resize(depth_img, depth_img, Size(640, 480));
	imshow("Depth Image", depth_img);
	waitKey(1);

	vector<Point3d> points;
	vector<Vec3b> colors;
	for(int y = 0; y < 240; ++y)
	{
		for(int x = 0; x < 320; ++x)
		{
			Vertex p = data.vertices[y * 320 + x];
			points.push_back(Point3d(p.x, p.y, p.z));

			if(!color_img.empty())
			{
				// カラー画像座標系に射影
				Point2D pos;
				proj_helper->get2DCoordinates(&p, &pos, 1, CAMERA_PLANE_COLOR);
				int x_pos = (int)pos.x;
				int y_pos = (int)pos.y;

				// その位置の色画素値を記録
				if(x_pos >= 0 && x_pos < color_img.cols &&
					y_pos >= 0 && y_pos < color_img.rows)
				{
					colors.push_back(color_img.at<Vec3b>(y_pos, x_pos));
				}
				else
					colors.push_back(Vec3b(0, 0, 0));
			}
		}
	}
	viewer->setPoints(points);
	viewer->setColors(colors);
}

void Sensor::configureAudioNode()
{
}

void Sensor::configureColorNode()
{
	c_node.newSampleReceivedEvent().connect<Sensor>(this, &Sensor::onNewColorSample);

	ColorNode::Configuration config = c_node.getConfiguration();
	config.frameFormat = FRAME_FORMAT_VGA;
	config.compression = COMPRESSION_TYPE_MJPEG;
	config.powerLineFrequency = POWER_LINE_FREQUENCY_50HZ;
	config.framerate = 25;

	c_node.setEnableColorMap(true);

	try
	{
		device_context.requestControl(c_node, 0);
		c_node.setConfiguration(config);
	}
	catch(std::exception &ex)
	{
		cerr << "Sensor::configureColorNode()" << endl
			<< ex.what() << endl;
	}
}

void Sensor::configureDepthNode()
{
	d_node.newSampleReceivedEvent().connect<Sensor>(this, &Sensor::onNewDepthSample);

	DepthNode::Configuration config = d_node.getConfiguration();
	config.frameFormat = FRAME_FORMAT_QVGA;
	config.framerate = 25;
	config.mode = DepthNode::CAMERA_MODE_CLOSE_MODE;
	config.saturation = true;

	d_node.setEnableVertices(true);
	d_node.setEnableDepthMap(true);

	try
	{
		device_context.requestControl(d_node, 0);
		d_node.setConfiguration(config);
	}
	catch(std::exception &ex)
	{
		cerr << "Sensor::configureDepthNode()" << endl
			<< ex.what() << endl;
	}
}

void Sensor::configureNode(Node node)
{
	if((node.is<DepthNode>()) && (!d_node.isSet()))
	{
		d_node = node.as<DepthNode>();
		configureDepthNode();
		device_context.registerNode(node);
	}
	if((node.is<ColorNode>()) && (!c_node.isSet()))
	{
		c_node = node.as<ColorNode>();
		configureColorNode();
		device_context.registerNode(node);
	}
	if((node.is<AudioNode>()) && (!a_node.isSet()))
	{
		a_node = node.as<AudioNode>();
		configureAudioNode();
		device_context.registerNode(node);
	}
}

void Sensor::onNodeConnected(Device device, Device::NodeAddedData data)
{
}

void Sensor::onNodeDisconnected(Device device, Device::NodeRemovedData data)
{
}

void Sensor::onDeviceConnected(Context context, Context::DeviceAddedData data)
{
	if(!device_found)
	{
		data.device.nodeAddedEvent().connect<Sensor>(this, &Sensor::onNodeConnected);
		data.device.nodeRemovedEvent().connect<Sensor>(this, &Sensor::onNodeDisconnected);
		device_found = true;
	}
}

void Sensor::onDeviceDisconnected(Context context, Context::DeviceRemovedData data)
{
	device_found = false;
}