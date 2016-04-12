/*
 * tracker.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#include "counter.h"
#include "tracker.h"

namespace CT {

//Tracker::Tracker(){
//	m_id = -1;
//	m_ctr = -1;
//}

Tracker::Tracker(CT::identifier_t id, CT::identifier_t ctr) {
	m_id = id;
	m_ctr = ctr;
	treshold = 10;
	freezDuration = 0;
}

Tracker::~Tracker() {
}

void Tracker::initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect){
	m_tr.start_track(img, rect);
	m_initial = dlib::center(rect);
	m_current = dlib::center(rect);
}

dlib::point Tracker::initial() const {
	return m_initial;
}

dlib::point Tracker::current() const {
	return m_current;
}

void Tracker::setInitial(const dlib::point &p) {
	m_initial = p;
}

void Tracker::setCurrent(const dlib::point &p) {
	m_current = p;
}

CT::identifier_t Tracker::getId() const {
	return m_id;
}

double Tracker::update(dlib::cv_image<dlib::bgr_pixel> & img) {
	double confidence = m_tr.update(img);
	dlib::point center = dlib::center(m_tr.get_position());
	if(isFreezing(m_current, center))
		incrFreezDuration();
	else
		resetFreezingDuration();
	m_current = center;
	return confidence;
}

dlib::correlation_tracker Tracker::getTracker(){
	return m_tr;
}

void Tracker::setCounter(CT::identifier_t ctr) {
	m_ctr = ctr;
}

CT::identifier_t Tracker::getCounter(){
	return m_ctr;
}


void Tracker::incrFreezDuration(){
	freezDuration++;
}
int Tracker::getFreezDuration(){
	return freezDuration;
}

bool Tracker::isFreezing(dlib::point current, dlib::point center){
	double distance = std::sqrt((current.x()-center.x())*(current.x()-center.x()) + (current.y()-center.y())*(current.y()-center.y()));
	if(distance < treshold)
		return true;
	else
		return false;
}

void Tracker::resetFreezingDuration(){
	freezDuration = 0;
}
} /* namespace CT */
