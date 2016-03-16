/*
 * Line.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "line.h"

namespace CT {

//Line::Line() {
//	m_id = 0;
//	m_a = 0;
//	m_b = 0;
//}

Line::Line(dlib::point &p1, dlib::point &p2, CT::identifier_t id_) {
	m_p1 = p1;
	m_p2 = p2;
	m_id = id_;
	m_a = (p1.y() - p2.y())/(p1.x() - p2.x());
	m_b = p1.y() - m_a*p2.x();
}

Line::~Line() {
	std::cout<<"~Line()"<<std::endl;
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

float Line::getSlope(void) {
	return m_a;
}

float Line::getYIntercept(void) {
	return m_b;
}

bool Line::isInside(const dlib::point& p) const {
	if(m_a > 0)
		return (m_a*p.x() - p.y() + m_b > 0);
	else
		return (m_a*p.x() - p.y() + m_b < 0);
}

} // Namespace CT
