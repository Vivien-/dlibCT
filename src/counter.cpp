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

Counter::Counter(CT::identifier_t id) {
	m_id = id;
	m_id_line = id;
	m_in = 0;
	m_out = 0;
	m_stay = 0;
}

Counter::~Counter() {
	std::cout<<"~Counter()"<<std::endl;
}

void Counter::addTracker(CT::identifier_t tracker){
	m_id_trackers.insert(std::make_pair(tracker,tracker));
}

void Counter::removeTracker(CT::identifier_t tracker){
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


CT::identifier_t Counter::getLine(){
	return m_id_line;
}
} // Namespace CT
