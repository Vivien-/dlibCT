/*
 * tracker.cpp
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#include "counter.h"
#include "tracker.h"

namespace CT {

Tracker::Tracker() {
	printf("tracker");
	m_id = 0;
	m_ctr = nullptr;
	// TODO Auto-generated constructor stub
}

Tracker::~Tracker() {
	delete m_ctr;
}

void Tracker::initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect){
	m_tr.start_track(img, rect);
	m_initial = dlib::center(rect);
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
	m_current = dlib::center(m_tr.get_position());
	return confidence;
}

dlib::correlation_tracker Tracker::getTracker(){
	return m_tr;
}

void Tracker::setCounter(CT::Counter & ctr) {
	m_ctr = new CT::Counter(ctr);
}

} /* namespace CT */
