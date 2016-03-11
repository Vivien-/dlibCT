/*
 * controller.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#include "controller.h"

namespace CT {

Controller::Controller() {
	// TODO Auto-generated constructor stub

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::process(std::vector<dlib::rectangle> & curObjDetected, dlib::cv_image<dlib::bgr_pixel> & cimg) {
	// Track newly detected faces if don't overlap with already tracked face
	for(uint i = 0; i < curObjDetected.size(); i++) {
		dlib::rectangle face = curObjDetected[i];
		bool alreadytracked = false;
		for(uint j = 0; j < trackers.size(); j++) {
			if(face.contains(dlib::center(trackers[j].get_position())) || trackers[j].get_position().contains(dlib::center(face))) {
				alreadytracked = true;
			}
		}
		if(!alreadytracked) {
			dlib::correlation_tracker tracker;
			tracker.start_track(cimg, face);
			trackers.push_back(tracker);
		}
	}
}

void Controller::update(dlib::cv_image<dlib::bgr_pixel> & cimg) {
	for(uint i = 0; i < trackers.size(); i++)
		trackers[i].update(cimg);
	this->deleteUselessTracker();
}

void Controller::display(dlib::image_window &win, dlib::cv_image<dlib::bgr_pixel> & cimg){
	win.clear_overlay();
	win.set_image(cimg);
	for(uint i = 0; i < trackers.size(); i++)
		win.add_overlay(trackers[i].get_position(), dlib::rgb_pixel(0,255,0));
}

void Controller::deleteUselessTracker() {
	for(auto it = trackers.begin(); it != trackers.end(); ) {
		dlib::correlation_tracker cur = trackers[it];
		for(auto it2 = trackers.begin(); it2 != trackers.end(); ) {
			dlib::correlation_tracker that = trackers[it2];
			if(cur != that) {
				if(cur.get_position().contains(that.get_position()))

			}
		}
	}
}

} /* namespace CT */
