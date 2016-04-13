/*
 * Line.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "line.h"

namespace CT {

Line::Line(dlib::point &p1, dlib::point &p2, CT::identifier_t id_) {
	std::cout<<__FILE__<<__FUNCTION__<<" at "<<__LINE__<<"=>\n"
			<<p1<<" and "<<p2<<std::endl;
	m_p1 = p1;
	m_p2 = p2;
	m_id = id_;
	if(p1 == p2)
		m_a = 0;
	else
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

bool Line::position(dlib::point & other){
	if(m_a > 0)
		return (m_p2.x() - m_p1.x()) * (other.y() - m_p1.y()) - (m_p2.y() - m_p1.y()) * (other.x() - m_p1.x()) < 0;
	else
		return (m_p2.x() - m_p1.x()) * (other.y() - m_p1.y()) - (m_p2.y() - m_p1.y()) * (other.x() - m_p1.x()) > 0;
}


double Line::distance(dlib::point & other){
	return std::fabs(-m_a*other.x() + other.y() - m_b)/std::sqrt(1 + m_a*m_a);
}

} // Namespace CT
