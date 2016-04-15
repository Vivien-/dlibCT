/*
 * Line.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "line.h"

namespace CT {

Line::Line(dlib::point &p1, dlib::point &p2, CT::identifier_t id_) {
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

const float length_squared(const dlib::point & v, const dlib::point & w){
	return std::pow(v.x() - w.x(), 2) + pow(v.y() - w.y(), 2);
}

double Line::distance(dlib::point & other){
	const float l2 = length_squared(m_p1, m_p2);
	if (l2 == 0.0)
		return std::sqrt(length_squared(other, m_p1));

	float t = dlib::dot(other - m_p1, m_p2 - m_p1) / l2;
	t = std::max((float)0.0, std::min((float)1.0, t));
	dlib::point projection (m_p1 + t * (m_p2 - m_p1));  // Projection falls on the segment
	return std::sqrt(length_squared(other, projection));
}

} // Namespace CT
