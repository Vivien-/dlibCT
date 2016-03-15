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

//Counter::Counter() {
//	std::cout<<"Counter(): error: should call Counter(CT::identifier_t)"<<std::endl;
//	m_in = 0;
//	m_out = 0;
//	m_stay = 0;
//	m_id = -1;
//	m_id_line = -1;
//}

Counter::Counter(CT::identifier_t id) {
	// Computes the slope and coordinate at origin of the line
	m_id = id;
	m_id_line = id;
	//m_a = (m_line.getFirstEndpoint().y() - m_line.getSecondEndpoint().y())/(m_line.getFirstEndpoint().x() - m_line.getSecondEndpoint().x());
	//m_b = m_line.getFirstEndpoint().y() - m_a*m_line.getFirstEndpoint().x();
	m_in = 0;
	m_out = 0;
	m_stay = 0;
}

Counter::~Counter() {
	std::cout<<"~Counter()"<<std::endl;
}

void Counter::addTracker(CT::identifier_t tracker){
	for(uint i = 0; i < m_id_trackers.size(); ++i) {
		if(m_id_trackers[i] == tracker)
			return ;
	}
	m_id_trackers.insert(std::make_pair(tracker,tracker));
}

void Counter::removeTracker(CT::identifier_t tracker){
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

CT::identifier_t Counter::getId() const{
	return m_id;
}

void Counter::setLine(CT::identifier_t l) {
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

std::map<CT::identifier_t, int> Counter::getIdTrackers(){
	return  m_id_trackers;
}

} // Namespace CT
