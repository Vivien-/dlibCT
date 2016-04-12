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
	for(auto it = trackers.begin(); it != trackers.end();) {
		// Confidence that the area covored by the tracker looks like the initial tracked object
		double confidence = trackers.find(it->first)->second.update(cimg);
		// If the confidence is under a certain threshold, this tracker can be removed (the object probably disappeared frome the image)
		if(confidence < m_threshold || trackers.find(it->first)->second.getFreezDuration() >= maxFreezDuration){
			CT::Counter & counter = counters.find(trackers.find((it)->first)->second.getCounter())->second;
			std::cout<<"Associated counter: "<<trackers.find((it)->first)->second.getCounter()<<std::endl;
			std::cout<<"Tracker id "<<trackers.find(it->first)->second.getId()<<" removed from counter "<<counter.getId()<<std::endl;
			counter.removeTracker(trackers.find(it->first)->second.getId());
			trackers.erase(trackers.find((it++)->first)->second.getId());

		} else{
//			CT::Tracker t = trackers.find(it->first)->second;
//			trackers.find(it->first)->second.setCounter(getBestLine(t.current()));
			++it;
		}

	}
}

void Controller::display(){
	//clear previously drawn stuff. If you want to display the path of the tracked object, comment that line
	m_editor->display.clear_overlay();
	//display the lines (corresponding with a counter)
	for(auto it = lines.begin(); it != lines.end(); ++it) {
		dlib::point p1 = lines.find(it->first)->second.getFirstEndpoint();
		dlib::point p2 = lines.find(it->first)->second.getSecondEndpoint();
		m_editor->display.add_overlay(dlib::image_window::overlay_line(p1, p2, dlib::rgb_pixel(0,0,255)));
	}
	// Create a circle of radius 3 on the center of the tracked object and label it with the id of the tracker
	displayTrackers();
}

void Controller::displayTrackers() {
	for(auto it = trackers.begin(); it != trackers.end(); ++it) {
		CT::Tracker & t = trackers.find(it->first)->second;
		dlib::rectangle center_rect = dlib::centered_rect(dlib::center(t.getTracker().get_position()), 5, 5);
		m_editor->display.add_overlay(dlib::image_window::overlay_rect(center_rect, dlib::rgb_pixel(8*t.getId()*t.getId(), 5*t.getId()*t.getId()*t.getId(), t.getId()*t.getId()), boost::lexical_cast<std::string>(t.getId())));
		m_editor->display.add_overlay(dlib::image_window::overlay_rect(t.getTracker().get_position(), dlib::rgb_pixel(8*t.getId()*t.getId(), 5*t.getId()*t.getId()*t.getId(), t.getId()*t.getId()), boost::lexical_cast<std::string>(t.getId())));
	}
}

void Controller::addLine(dlib::point &p1, dlib::point &p2) {
	CT::identifier_t next_id = IDGenerator::instance().next();
	std::cout<<"adding line : "<<next_id<<std::endl;
	CT::Line l = CT::Line(p1, p2, next_id);
	CT::Counter c(next_id);
	lines.insert(std::make_pair(next_id, l));
	counters.insert(std::make_pair(next_id, c));
	//next_id++;
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
	if(lines.size()) {
		for(auto it = counters.begin(); it != counters.end(); ++it) {
			int entered = counters.find(it->first)->second.getIn();//it->second.getIn();
			int left = counters.find(it->first)->second.getOut();  //it->second.getOut();
			s += "Counters[" + boost::lexical_cast<std::string>(counters.find(it->first)->second.getId()) + "]: In=" + boost::lexical_cast<std::string>(entered) + " Out=" + boost::lexical_cast<std::string>(left) + "\n";
		}
	}
	m_editor->display.add_overlay(dlib::image_window::overlay_rect(dlib::rectangle(), dlib::rgb_pixel(255,255,255), s));
	std::cout<<s<<std::endl;
}

CT::identifier_t Controller::getBestLine(dlib::point p) {
	double distance = std::numeric_limits<double>::max();
	CT::identifier_t id = -1;
	for(auto it = lines.begin(); it != lines.end(); ++it){
		CT::Line & current_line = lines.find(it->first)->second;
		std::cout<<"Nb of lines: "<<lines.size()<<std::endl;
		std::cout<<"Current line id: "<<current_line.getId()<<std::endl;
		double current_distance = current_line.distance(p);
		if(current_distance < distance){
			distance = current_distance;
			id = current_line.getId();
		}
	}
	return id;
}

void Controller::setEditor(CT::metadata_editor* w) {
	m_editor = w;
}


} /* namespace CT */
