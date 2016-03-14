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
	m_id = 0;
	// TODO Auto-generated constructor stub
}

Tracker::~Tracker() {
	// TODO Auto-generated destructor stub
}

void Tracker::initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect){
	m_tr.start_track(img, rect);
}

void Tracker::track(dlib::cv_image<dlib::bgr_pixel> & img) {
	m_tr.update(img);
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

int Tracker::getId() const {
	return m_id;
}

} /* namespace CT */
