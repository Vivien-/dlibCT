/*
 * detector.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#include "detector.h"

namespace CT {

Detector::Detector() {
	_detector = dlib::get_frontal_face_detector();
}

Detector::~Detector() {
}

std::vector<dlib::rectangle> Detector::detect(dlib::cv_image<dlib::bgr_pixel> & img) {
	std::vector<dlib::rectangle> r = _detector(img);
	return r;
}

} /* namespace CT */
