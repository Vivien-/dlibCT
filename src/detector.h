/*
 * detector.h
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

namespace CT {

class Detector {
public:
	Detector();
	virtual ~Detector();
	std::vector<dlib::rectangle> detect(dlib::cv_image<dlib::bgr_pixel> & img);

private:
	dlib::frontal_face_detector _detector;
};

} /* namespace CT */

#endif /* DETECTOR_H_ */
