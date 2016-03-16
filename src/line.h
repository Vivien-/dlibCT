/*
 * Line.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 *
 *  Describe a line by its endpoints
 */

#ifndef LINE_H_
#define LINE_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <dlib/gui_widgets.h>
#include "identifiert.h"

namespace CT {

class Line {
public:
//	Line();
	Line(dlib::point &p1, dlib::point &p2, CT::identifier_t id_);
	virtual ~Line();
	dlib::point getFirstEndpoint(void) const;
	dlib::point getSecondEndpoint(void) const;
	CT::identifier_t getId(void) const;
	void setFirstEndpoint(int _x, int _y);
	void setSecondEndpoint(int _x, int _y);
	float getSlope(void);
	float getYIntercept(void);
	bool isInside(const dlib::point& p) const;
	bool position(dlib::point other);

private:
	dlib::point m_p1;
	dlib::point m_p2;
	float m_a;
	float m_b;
	CT::identifier_t m_id;
};

} //Namespace CT

#endif /* LINE_H_ */
