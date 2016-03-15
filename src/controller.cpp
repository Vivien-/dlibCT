/*
 * controller.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien & mohammed
 */

#include "controller.h"
#include <boost/lexical_cast.hpp>

namespace CT {

Controller::Controller() {
	m_threshold = 10.0;
	next_id = 0;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::process(std::vector<dlib::rectangle> & curObjDetected, dlib::cv_image<dlib::bgr_pixel> & cimg) {
	// Track newly detected faces if don't overlap with already tracked face
	for(uint i = 0; i < curObjDetected.size(); i++) {
		dlib::rectangle face = curObjDetected[i];
		bool alreadytracked = false;
		// We check that the detected object is not tracked yey
		for(uint j = 0; j < trackers.size(); j++) {
			if(face.contains(dlib::center(trackers[j].getTracker().get_position())) || trackers[j].getTracker().get_position().contains(dlib::center(face))) {
				alreadytracked = true;
				break;
			}
		}
		// If the object was not detected yet we initialise a tracker on the object position
		if(!alreadytracked) {
			CT::Tracker tracker;
			tracker.initTrack(cimg,face);
			trackers.push_back(tracker);
		}
	}
}

void Controller::update(dlib::cv_image<dlib::bgr_pixel> & cimg) {
	// Each object tracked has its position updated
	for(uint i = 0; i < trackers.size(); ++i) {
		// Confidence that the area covored by the tracker looks like the initial tracked object
		double confidence = trackers[i].update(cimg);
		// If the confidence is under a certain threshold, this tracker can be removed (the object probably disappeared frome the image)
		if(confidence < m_threshold){
			trackers.erase(trackers.begin() + i);
		}
	}
}

void Controller::display(dlib::image_window &win, dlib::cv_image<dlib::bgr_pixel> & cimg){
	win.clear_overlay();
	win.set_image(cimg);
	// Create a circle of radius 5 on th center of the tracked object
	for(uint i = 0; i < trackers.size(); i++)
		win.add_overlay(dlib::image_window::overlay_circle(dlib::center(trackers[i].getTracker().get_position()), 5, dlib::rgb_pixel(0,255,0), boost::lexical_cast<std::string>(i)));
}

void Controller::addLine(dlib::point &p1, dlib::point &p2) {
	CT::Line l = CT::Line(p1, p2, next_id);
	CT::Counter c(l, next_id);
	c.setLine(l);
	lines.push_back(l);
	counters.push_back(c);
	next_id++;
}

void Controller::updateCountersSituation() {
	for(auto it = trackers.begin(); it != trackers.end(); ++it) {
		setTrackerToCounter(*it, counters[0]);
	}
	// Count the number of object entering/leaving a line (i.e. a counter)
	for(auto it = counters.begin(); it != counters.end(); ++it)
		it->updateSituation();
}

void Controller::setTrackerToCounter(CT::Tracker& tr, CT::Counter& ctr){
	tr.setCounter(ctr);
	ctr.addTracker(tr);
}

void Controller::printSituation() {
	if(lines.size()) {
		updateCountersSituation();
		for(auto it = counters.begin(); it != counters.end(); ++it) {
			int entered = it->getIn();
			int left = it->getOut();
			std::cout<<"entered: "<<entered<<" left: "<<left<<std::endl;
		}
	}
}


} /* namespace CT */
