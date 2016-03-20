#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "root.h"

int main(int argc, char* argv[]) {
	try {
		CT::Root::get().run();
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
