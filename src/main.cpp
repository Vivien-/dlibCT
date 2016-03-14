#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "controller.h"
#include "tracker.h"

int main(int argc, char* argv[]) {
	try {
		cv::VideoCapture cap(0);
		dlib::image_window win;
		//		cap.set(CV_CAP_PROP_FRAME_WIDTH, 800);
		//		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 520);

		dlib::frontal_face_detector d = dlib::get_frontal_face_detector();
		CT::Controller controller;
		uint64 nfrm = 0;

		//endpoints of the line (used for tests)
		dlib::point p1;
		dlib::point p2;

		// Grab and process frames until the main window is closed by the user.
		while(!win.is_closed()) {
			// Grab a frame
			cv::Mat temp;
			cap >> temp;


			// Turn OpenCV's Mat into something dlib can deal with. don't modify temp while using cimg.
			dlib::cv_image<dlib::bgr_pixel> cimg(temp);

			// Detect faces
			if(nfrm++ % 100 == 0) {
				std::vector<dlib::rectangle> faces = d(cimg);
				controller.process(faces, cimg);
			}

			//update tracker
			controller.update(cimg);

			//display line
			dlib::draw_line(cimg, p1, p2, dlib::rgb_pixel(0,0,255));
			//display tracked object
			controller.display(win, cimg);

			if(nfrm == 1) {
				win.get_next_double_click(p1);
				win.get_next_double_click(p2);
				controller.addLine(p1,p2);
			}
		}
	} catch(dlib::serialization_error& e)	{
		std::cout << "You need dlib's default face landmarking model file to run this example." << std::endl;
		std::cout << "You can get it from the following URL: " << std::endl;
		std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
		std::cout << std::endl << e.what() << std::endl;
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
