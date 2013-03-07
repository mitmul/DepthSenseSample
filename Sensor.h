#pragma once

#include <vector>
#include <iostream>
#include <DepthSense.hxx>
#include "GLViewer.h"

using namespace std;
using namespace cv;
using namespace DepthSense;

class Sensor
{
public:
	Sensor(void);
	~Sensor(void);

	void setup(GLViewer &_viewer);

private:
	void onNewAudioSample(AudioNode node, AudioNode::NewSampleReceivedData data);
	void onNewColorSample(ColorNode node, ColorNode::NewSampleReceivedData data);
	void onNewDepthSample(DepthNode node, DepthNode::NewSampleReceivedData data);
	void configureAudioNode();
	void configureColorNode();
	void configureDepthNode();
	void configureNode(Node node);
	void onNodeConnected(Device device, Device::NodeAddedData data);
	void onNodeDisconnected(Device device, Device::NodeRemovedData data);

	void onDeviceConnected(Context context, Context::DeviceAddedData data);
	void onDeviceDisconnected(Context context, Context::DeviceRemovedData data);

private:
	Context device_context;
	DepthNode d_node;
	ColorNode c_node;
	AudioNode a_node;

	bool device_found;
	ProjectionHelper *proj_helper;
	StereoCameraParameters stereo_param;

	GLViewer *viewer;
	Mat color_img;
};

