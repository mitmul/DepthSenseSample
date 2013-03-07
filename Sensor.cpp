#include "Sensor.h"


Sensor::Sensor(void)
	: device_found(false),
	proj_helper(NULL)
{
	color_img.create(480, 640, CV_8UC3);
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
}

void Sensor::onNewDepthSample(DepthNode node, DepthNode::NewSampleReceivedData data)
{
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

	Mat img = color_img.clone();
	vector<Point3d> points;
	vector<Vec4b> colors;
	for(int y = 0; y < 240; ++y)
	{
		for(int x = 0; x < 320; ++x)
		{
			Vertex p = data.vertices[y * 320 + x];
			points.push_back(Point3d(p.x, p.y, p.z));

			if(!img.empty())
			{
				Point2D pos;
				proj_helper->get2DCoordinates(&p, &pos, 1, CAMERA_PLANE_COLOR);
				int x_pos = (int)pos.x;
				int y_pos = (int)pos.y;
				int img_pos = y_pos * 640 * 3 + x_pos * 3;
				if(img_pos >= 0 && img_pos < 640 * 480 * 3)
				{
					colors.push_back(Vec4b(img.data[img_pos + 0], img.data[img_pos + 1], img.data[img_pos + 2], 1));
				}
				else
					colors.push_back(Vec4b(0, 0, 0, 0));
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
	config.saturation = false;

	d_node.setEnableVertices(true);

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