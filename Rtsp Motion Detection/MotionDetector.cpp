#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <Windows.h>
#include "MotionDetector.h"



using namespace cv;
using namespace std;


VideoCapture camera;

MotionDetector::MotionDetector(const std::string& streamURL, int sensitivity)
{
	
	MotionDetector::streamURL = streamURL;
	MotionDetector::sensitivity = sensitivity;
	
	// inserting '0' as a parameter uses your webcam as a stream
	camera.open(streamURL);

	//set the video size to 512x288 to process faster
	camera.set(3, 512);
	camera.set(4, 288);

}

MotionDetector::~MotionDetector(){ 
	camera.release();
}


void MotionDetector::Start() {

	try {
		Mat frame, gray, frameDelta, thresh, staticFrame, tempFrame;
		vector<vector<Point> > cnts;
		int framesCount = 0;
		string defaultRtspUrl = "rtsp://admin:admin123@user10.ddns.net/user=admin&password=admin123&channel=16&stream=0.sdp";
		string defaultMjpgUrl = "http://38.81.159.248:80/mjpg/video.mjpg";
		bool motionDetected = false;

		// inserting '0' as a parameter uses your webcam as a stream
		//VideoCapture camera(0); //open camera




		//convert to grayscale and set the first frame
		camera.read(frame);
		cvtColor(frame, staticFrame, COLOR_BGR2GRAY);
		GaussianBlur(staticFrame, staticFrame, Size(21, 21), 0);

		cout << endl << "Listnening on: {" << defaultRtspUrl << "}" << endl << endl;

		while (camera.read(frame)) {

			//if (camera.read(frame)) {
			framesCount++;

			//convert the frame to grayscale
			cvtColor(frame, gray, COLOR_BGR2GRAY);
			GaussianBlur(gray, gray, Size(21, 21), 0);

			if (framesCount == 10) {
				cvtColor(frame, staticFrame, COLOR_BGR2GRAY);
				GaussianBlur(staticFrame, staticFrame, Size(21, 21), 0);
				framesCount = 0;
			}

			//compute the difference between the static frame and the current frame
			absdiff(staticFrame, gray, frameDelta);
			threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);

			dilate(thresh, thresh, Mat(), Point(-1, -1), 2);
			findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			for (int i = 0; i < cnts.size(); i++) {
				if (contourArea(cnts[i]) < sensitivity) {
					continue;
				}

				//putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
				//cout << "Motion Detected";
				motionDetected = true;
			}
			if (motionDetected) {
				cout << "MotionDetected ";
				motionDetected
					= false;
			}


			//namedWindow("Camera", WINDOW_KEEPRATIO);
			//cv::resizeWindow("Camera", 600, 400);
			//imshow("Camera", frame);

			//exit if ESC is pressed
			if (waitKey(1) == 27) {
				break;
			}


			//}
		}

	}
	catch (Exception e) {
		cout << e.err << endl;
	}
}
void MotionDetector::Stop() {
	camera.release();
}