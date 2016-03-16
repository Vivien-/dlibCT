/*
 * controller.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien & mohammed
 */

#include "controller.h"
#include "id_generator.h"
#include <boost/lexical_cast.hpp>

namespace CT {

Controller::Controller() {
	m_threshold = 10.0;
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
		for(auto it = trackers.begin(); it != trackers.end(); ++it) {
			if(face.contains(dlib::center(trackers.find(it->first)->second.getTracker().get_position())) || trackers.find(it->first)->second.getTracker().get_position().contains(dlib::center(face))) {
				alreadytracked = true;
				break;
			}
		}
		// If the object was not detected yet we initialise a tracker on the object position
		if(!alreadytracked) {
			dlib::point p = dlib::center(face);
			CT::Tracker tracker(IDGenerator::instance().next(), getBestLine(p));
			tracker.initTrack(cimg,face);
			trackers.insert(std::make_pair(tracker.getId(), tracker));
		}
	}
}

void Controller::update(dlib::cv_image<dlib::bgr_pixel> & cimg) {
	// Each object tracked has its position updated
	for(auto it = trackers.begin(); it != trackers.end(); ) {
		// Confidence that the area covored by the tracker looks like the initial tracked object
		double confidence = trackers.find(it->first)->second.update(cimg);
		// If the confidence is under a certain threshold, this tracker can be removed (the object probably disappeared frome the image)
		if(confidence < m_threshold){
			CT::Counter & counter = counters.find(trackers.find((it)->first)->second.getCounter())->second;
			counter.removeTracker(it->first);
			trackers.erase(trackers.find((it++)->first)->second.getId());
		} else
			++it;
	}
}

void Controller::display(dlib::image_window &win, dlib::cv_image<dlib::bgr_pixel> & cimg){
	win.clear_overlay();
	win.set_image(cimg);
	// Create a circle of radius 5 on th center of the tracked object
	int i = 0;
	for(auto it = trackers.begin(); it != trackers.end(); ++it)
		win.add_overlay(dlib::image_window::overlay_circle(dlib::center(trackers.find(it->first)->second.getTracker().get_position()), 5, dlib::rgb_pixel(0,255,0), boost::lexical_cast<std::string>(trackers.find(it->first)->second.getId())));
}

void Controller::addLine(dlib::point &p1, dlib::point &p2) {
	CT::identifier_t next_id = IDGenerator::instance().next();
	CT::Line l = CT::Line(p1, p2, next_id);
	CT::Counter c(next_id);
	lines.insert(std::make_pair(next_id, l));
	counters.insert(std::make_pair(next_id, c));
	next_id++;
}


void Controller::setTrackersToCounters(){
	for(auto it = trackers.begin(); it != trackers.end(); ++it) {
			dlib::point p = trackers.find(it->first)->second.current();//it->second.current();
			CT::identifier_t best_counter_id = getBestLine(p);
			setTrackerToCounter(it->first, best_counter_id);
		}
}

void Controller::updateCounters(){
	for(auto it = counters.begin(); it != counters.end(); ++it) {
			CT::Line &current_line = lines.find(it->first)->second;
			CT::Counter &current_counter = it->second;
			std::map<CT::identifier_t, int> id_trackers = current_counter.getIdTrackers();
			std::cout<<"id_trackers.size(): "<<id_trackers.size()<<std::endl;
			for(auto that_id = id_trackers.begin(); that_id != id_trackers.end(); ++that_id) {
				CT::Tracker &that_tracker = trackers.find(that_id->first)->second;
				dlib::point initial = that_tracker.initial();
				dlib::point current = that_tracker.current();
				if(current_line.position(initial) == true && current_line.position(current) == false){
					current_counter.incrOut();
					that_tracker.setInitial(current);
				}
				else if(current_line.position(initial) == false && current_line.position(current) == true){
					current_counter.incrIn();
					that_tracker.setInitial(current);
				}
				/*if(current.y() >= std::min(current_line.getFirstEndpoint().y(),
											current_line.getSecondEndpoint().y())
				&& current.y() <= std::max(current_line.getFirstEndpoint().y(),
											current_line.getSecondEndpoint().y())) {

					if(!(current_line.isInside(initial)) && current_line.isInside(current)) {
						current_counter.incrIn();
						that_tracker.setInitial(current);
					}
					else if(current_line.isInside(initial) && !(current_line.isInside(current)) ) {
						current_counter.incrOut();
						that_tracker.setInitial(current);
					}
					else {
						std::cout<<"Staying in here yo"<<std::endl;
						m_stay ++;
					}
				}
*/			}
		}
}

void Controller::updateCountersSituation() {
	setTrackersToCounters();
	updateCounters();
}

void Controller::setTrackerToCounter(CT::identifier_t tr, CT::identifier_t ctr){
	trackers.find(tr)->second.setCounter(ctr);
	counters.find(ctr)->second.addTracker(tr);
}

void Controller::printSituation() {
	if(lines.size()) {
		std::cout<<"number of counter: "<<counters.size()<<std::endl;
		for(auto it = counters.begin(); it != counters.end(); ++it) {

			int entered = counters.find(it->first)->second.getIn();//it->second.getIn();
			int left = counters.find(it->first)->second.getOut();  //it->second.getOut();
			std::cout<<"Counters id "<<it->first<<" entered: "<<entered<<" left: "<<left<<std::endl;
		}
	}
}



CT::identifier_t Controller::getBestLine(dlib::point &p) {
	return 1;
}

} /* namespace CT */
