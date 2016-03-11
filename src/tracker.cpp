/*
 * tracker.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#include "tracker.h"

namespace CT {

Tracker::Tracker() {
	printf("tracker");
	// TODO Auto-generated constructor stub

}

Tracker::~Tracker() {
	// TODO Auto-generated destructor stub
}

void Tracker::initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect){
	tr.start_track(img, rect);
}

void Tracker::track(dlib::cv_image<dlib::bgr_pixel> & img) {
	tr.update(img);
}

} /* namespace CT */
