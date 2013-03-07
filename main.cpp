#include "GLViewer.h"
#include "Sensor.h"

GLViewer *viewer;
bool viewer_initialize;

void setupViewer(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	viewer_initialize = false;
	boost::thread view(boost::bind(setupViewer, argc, argv));

	while(!viewer_initialize)
		cout << ".";
	cout << endl;

	Sensor sensor;
	sensor.setup(*viewer);

	return 0;
}

void setupViewer(int argc, char* argv[])
{
	viewer = new GLViewer();
	viewer_initialize = true;
	viewer->init(argc, argv);
}