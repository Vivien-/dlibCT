/*
 * controller.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien & mohammed
 */

#include "controller.h"
#include "metadata_editor.h"
#include "id_generator.h"
#include <boost/lexical_cast.hpp>

namespace CT {

Controller::Controller() {
	m_threshold = 4.5;
	m_editor = nullptr;
	maxFreezDuration = 15;
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
		for(auto & tracker : trackers) {
			if(face.contains(dlib::center(tracker.second.getTracker().get_position())) || tracker.second.getTracker().get_position().contains(dlib::center(face))) {
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
	for(auto it = trackers.begin(); it != trackers.end();) {
		// Confidence that the area covored by the tracker looks like the initial tracked object
		double confidence = trackers.find(it->first)->second.update(cimg);
		// If the confidence is under a certain threshold, this tracker can be removed (the object probably disappeared frome the image)
		if(confidence < m_threshold || trackers.find(it->first)->second.getFreezDuration() >= maxFreezDuration){
			if(counters.size()) {
				CT::Counter & counter = counters.find(trackers.find((it)->first)->second.getCounter())->second;
				counter.removeTracker(trackers.find(it->first)->second.getId());
			}
			trackers.erase(trackers.find((it++)->first)->second.getId());

		} else{
			++it;
		}

	}
}

void Controller::display(){
	//clear previously drawn stuff. If you want to display the path of the tracked object, comment that line
	m_editor->display.clear_overlay();
	//display the lines (corresponding with a counter)
	for(auto & line : lines) {
		dlib::point p1 = line.second.getFirstEndpoint();
		dlib::point p2 = line.second.getSecondEndpoint();
		m_editor->display.add_overlay(dlib::image_window::overlay_line(p1, p2, dlib::rgb_pixel(0,0,255)));
	}
	// Create a circle of radius 3 on the center of the tracked object and label it with the id of the tracker
	displayTrackers();
}

void Controller::displayTrackers() {
	for(auto & tracker : trackers) {
		CT::Tracker & t = tracker.second;
		dlib::rectangle center_rect = dlib::centered_rect(dlib::center(t.getTracker().get_position()), 5, 5);
		m_editor->display.add_overlay(dlib::image_window::overlay_rect(center_rect, dlib::rgb_pixel(8*t.getId()*t.getId(), 5*t.getId()*t.getId()*t.getId(), t.getId()*t.getId()), boost::lexical_cast<std::string>(t.getId())));
		m_editor->display.add_overlay(dlib::image_window::overlay_rect(t.getTracker().get_position(), dlib::rgb_pixel(8*t.getId()*t.getId(), 5*t.getId()*t.getId()*t.getId(), t.getId()*t.getId()), boost::lexical_cast<std::string>(t.getId())));
	}
}

void Controller::addLine(dlib::point &p1, dlib::point &p2) {
	CT::identifier_t next_id = IDGenerator::instance().next();
	CT::Line l = CT::Line(p1, p2, next_id);
	CT::Counter c(next_id);
	lines.insert(std::make_pair(next_id, l));
	counters.insert(std::make_pair(next_id, c));
}

void Controller::setTrackersToCounters(){
	for(auto & tracker : trackers) {
		dlib::point p = tracker.second.current();//it->second.current();
		CT::identifier_t best_counter_id = getBestLine(p);
		if(best_counter_id != -1)
			setTrackerToCounter(tracker.first, best_counter_id);
	}
}

void Controller::updateCounters(){
	for(auto & counter : counters) {
		CT::Counter &current_counter = counter.second;
		CT::Line &current_line = lines.find(current_counter.getLine())->second;
		std::map<CT::identifier_t, int> id_trackers = current_counter.getIdTrackers();
		for(auto & that_id : id_trackers) {
			CT::Tracker &that_tracker = trackers.find(that_id.first)->second;
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
		}
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
	std::string s;
	std::cout<<"number of counters : "<<counters.size()<<std::endl;
	for(auto & counter : counters) {
		int entered = counter.second.getIn();
		int left = counter.second.getOut();
		s += "Counters[" + boost::lexical_cast<std::string>(counter.first) + "]: In=" + boost::lexical_cast<std::string>(entered) + " Out=" + boost::lexical_cast<std::string>(left) + "\n";
	}
	std::cout<<s<<std::endl;
	m_editor->display.add_overlay(dlib::image_window::overlay_rect(dlib::rectangle(), dlib::rgb_pixel(0,255,0), s));
}

CT::identifier_t Controller::getBestLine(dlib::point p) {
	double distance = std::numeric_limits<double>::max();
	double current_distance = 0;
	CT::identifier_t id = -1;
	for(auto & line : lines){
		current_distance = line.second.distance(p);
		if(current_distance < distance){
			distance = current_distance;
			id = line.second.getId();
		}
	}
	return id;
}

void Controller::setEditor(CT::metadata_editor* w) {
	m_editor = w;
}


} /* namespace CT */
