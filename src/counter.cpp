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

Counter::Counter(CT::Line &l, int id_): m_id(id_) {
	// Computes the slope and coordinate at origin of the line
	m_line = l;
	m_a = (m_line.getFirstEndpoint().y() - m_line.getSecondEndpoint().y())/(m_line.getFirstEndpoint().x() - m_line.getSecondEndpoint().x());
	m_b = m_line.getFirstEndpoint().y() - m_a*m_line.getFirstEndpoint().x();
	m_in = 0;
	m_out = 0;
	m_stay = 0;
}


void Counter::addTracker(CT::Tracker& tracker){
	trackers.push_back(tracker);
}


int Counter::removeTracker(CT::Tracker& tracker){
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
	// Number of object that entered or left the line linked to this Counter instance
	std::cout<<"nb of tracker in counter "<<m_id<<": "<<trackers.size()<<std::endl;
	for(uint i = 0; i < trackers.size(); i++){
		dlib::point c = trackers[i].current();
		dlib::point init = trackers[i].initial();
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" "<<__func__<<"] Initial pos of tracker "<<trackers[i].getId()<<" is "<<init<<std::endl;
		if(c.y() >= std::min(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y()) && c.y() <= std::max(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y())) {
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


CT::identifier_t Counter::getId() const{
	return m_id;
}

void Counter::setLine(CT::Line &l) {
	m_line = l;
}

} // Namespace CT
