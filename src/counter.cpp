/*
 * Counter.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "counter.h"
#include "tracker.h"
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
	m_trackers.push_back(tracker);
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
	std::cout<<"nb of tracker in counter "<<m_id<<": "<<m_trackers.size()<<std::endl;
	for(uint i = 0; i < m_trackers.size(); i++){
		dlib::point c = m_trackers[i].current();
		dlib::point init = m_trackers[i].initial();
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" "<<__func__<<"] Initial pos of tracker "<<m_trackers[i].getId()<<" is "<<init<<std::endl;
		if(c.y() >= std::min(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y()) && c.y() <= std::max(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y())) {
			if(!isInside(init) && isInside(c)) {
				m_in ++;
				m_trackers[i].setInitial(c);
			}
			else if(isInside(init) && !isInside(c)) {
				m_out ++;
				m_trackers[i].setInitial(c);
			}
			else
				m_stay ++;
		}
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" in "<<__func__<<"] New set init position of tracker "<<m_trackers[i].getId()<<" is "<<m_trackers[i].initial()<<std::endl;
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
