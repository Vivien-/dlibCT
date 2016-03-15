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

Counter::Counter(CT::identifier_t id, CT::identifier_t &l) {
	// Computes the slope and coordinate at origin of the line
	m_id = id;
	m_id_line = l;
	//m_a = (m_line.getFirstEndpoint().y() - m_line.getSecondEndpoint().y())/(m_line.getFirstEndpoint().x() - m_line.getSecondEndpoint().x());
	//m_b = m_line.getFirstEndpoint().y() - m_a*m_line.getFirstEndpoint().x();
	m_in = 0;
	m_out = 0;
	m_stay = 0;
}

Counter::~Counter() {
	std::cout<<"~Counter()"<<std::endl;
}

void Counter::addTracker(CT::identifier_t& tracker){
	for(uint i = 0; i < m_id_trackers.size(); ++i) {
		if(m_id_trackers[i] == tracker)
			return ;
	}
	m_id_trackers.insert(tracker,tracker);
}


void Counter::removeTracker(CT::identifier_t & tracker){
	//return tracker position in trackers
	//trackers.erase(trackers.begin()+ position);
	m_id_trackers.erase(tracker);
}

int Counter::getIn() const{
	return m_in;
}


int Counter::getOut() const{
	return m_out;
}

//void Counter::updateSituation() {
	// Number of object that entered or left the line linked to this Counter instance
	/*std::cout<<"nb of tracker in counter "<<m_id<<": "<<m_trackers.size()<<std::endl;
	for(uint i = 0; i < m_id_trackers.size(); i++){
		dlib::point c = m_id_trackers[i].current();
		dlib::point init = m_trackers[i].initial();
		std::cout<<"Init: isInside("<<i<<"): "<<isInside(init)<<"\n Current: isInside("<<c<<") "<<isInside(c)<<std::endl;
		if(c.y() >= std::min(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y()) && c.y() <= std::max(m_line.getFirstEndpoint().y(), m_line.getSecondEndpoint().y())) {
			if(!isInside(init) && isInside(c)) {
				m_in ++;
				m_trackers[i].setInitial(c);
			}
			else if(isInside(init) && !isInside(c)) {
				m_out ++;
				m_trackers[i].setInitial(c);
			}
			else {
				std::cout<<"Staying in here yo"<<std::endl;
				m_stay ++;
			}
		}
	}*/
//}

//bool Counter::isInside(const dlib::point& c) const {
//	if(m_a > 0)
//		return (m_a*c.x() - c.y() + m_b > 0);
//	else
//		return (m_a*c.x() - c.y() + m_b < 0);
//}


CT::identifier_t Counter::getId() const{
	return m_id;
}

void Counter::setLine(CT::identifier_t &l) {
	m_id_line = l;
}

void Counter::incrIn(){
	m_in ++;
}
void Counter::incrOut(){
	m_out++;
}
int Counter::getCount(){
	return (m_in - m_out);
}





} // Namespace CT
