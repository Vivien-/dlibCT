/*
 * Line.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "line.h"

namespace CT {

Line::Line() {
	m_id = 0;
}

Line::Line(dlib::point &a, dlib::point &b, CT::identifier_t id_) {
	m_p1 = a;
	m_p2 = b;
	m_id = id_;
}

Line::~Line() {
	// TODO Auto-generated destructor stub
}

dlib::point Line::getFirstEndpoint(void) const{
	return m_p1;
}

dlib::point Line::getSecondEndpoint(void) const{
	return m_p2;
}

CT::identifier_t Line::getId(void) const{
	return m_id;
}

void Line::setFirstEndpoint(int _x, int _y){
	m_p1 = dlib::point(_x, _y);
}

void Line::setSecondEndpoint(int _x, int _y){
	m_p2 = dlib::point(_x, _y);
}

} // Namespace CT
