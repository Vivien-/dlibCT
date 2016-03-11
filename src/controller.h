/*
 * controller.h
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

namespace CT {

class Controller {
public:
	Controller();
	virtual ~Controller();
	void process(std::vector<dlib::rectangle> & objDetected, dlib::cv_image<dlib::bgr_pixel> & cimg);
	void update(dlib::cv_image<dlib::bgr_pixel> & cimg);
	void display(dlib::image_window &win, dlib::cv_image<dlib::bgr_pixel> & cimg);
	void deleteUselessTracker();

private:
	std::vector<dlib::correlation_tracker> trackers;

};

} /* namespace CT */

#endif /* SRC_CONTROLLER_H_ */
