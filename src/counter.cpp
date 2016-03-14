/*
 * Counter.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "counter.h"

#include <opencv2/opencv.hpp>

namespace CT {

Counter::Counter() {
}

Counter::~Counter() {
	// TODO Auto-generated destructor stub
	std::cout<<"~Counter()"<<std::endl;
}

Counter::Counter(dlib::point &p1, dlib::point &p2, int id_): m_id(id_){
	m_p1 = p1;
	m_p2 = p2;
	m_a = (p1.y() - p2.y())/(p1.x() - p2.x());
	m_b = p1.y() - m_a*p1.x();
	m_in = 0;
	m_out = 0;
	m_stay = 0;
}


void Counter::addTracker(Tracker& tracker){
	trackers.push_back(tracker);
}


int Counter::removeTracker(Tracker& tracker){
//return tracker position in trackers
	//trackers.erase(trackers.begin()+ position);
	return -1;
}

int Counter::getIn() const{
	return m_in;
}


int Counter::getOut() const{
	return m_out;
}

void Counter::updateSituation() {
	std::cout<<"nb of tracker in counter "<<m_id<<": "<<trackers.size()<<std::endl;
	for(uint i = 0; i < trackers.size(); i++){
		dlib::point c = trackers[i].current();
		dlib::point init = trackers[i].initial();
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" "<<__func__<<"] Initial pos of tracker "<<trackers[i].getId()<<" is "<<init<<std::endl;
		if(c.y() >= std::min(m_p1.y(), m_p2.y()) && c.y() <= std::max(m_p1.y(), m_p2.y())) {
			if(!isInside(init) && isInside(c)) {
				m_in ++;
				trackers[i].setInitial(c);
			}
			else if(isInside(init) && !isInside(c)) {
				m_out ++;
				trackers[i].setInitial(c);
			}
			else
				m_stay ++;
		}
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" in "<<__func__<<"] New set init position of tracker "<<trackers[i].getId()<<" is "<<trackers[i].initial()<<std::endl;
	}
}

bool Counter::isInside(const dlib::point& c) const {
	if(m_a > 0)
		return (m_a*c.x() - c.y() + m_b > 0);
	else
		return (m_a*c.x() -c.y() + m_b < 0);
}


int Counter::getId() const{
	return m_id;
}


void Counter::setLine(dlib::point &p1, dlib::point &p2) {
	m_p1 = p1;
	m_p2 = p2;
}

} // Namespace CT
