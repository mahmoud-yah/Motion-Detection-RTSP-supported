//#include <opencv2/opencv.hpp>
//#include <opencv2/core/ocl.hpp>
#include <Windows.h>
#include "MotionDetector.h"
#include <thread>


using namespace std;


int main(int argc, char** argv) {

	MotionDetector motionDetector{ "http://158.58.130.148/mjpg/video.mjpg" ,500 };
	motionDetector.Start();
	return 0;

}
