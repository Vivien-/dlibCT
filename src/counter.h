/*
 * Counter.h
 *
 *  Created on: 11 february. 2016
 *      Author: vivien & Mohammed
 *
 *  Counter is a class that counts the number of people entering and leaving each train car.
 *  The entrance of a train car is represented by a line. Whenever a tracker crosses a line,
 *  the number of people inside or outside the car is incremented. To each counter we associate
 *  a single line and a set of trackers. These trackers are generally nearby the line.
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "tracker.h"

namespace CT {


class Counter {
public:
	Counter();
	virtual ~Counter();
	Counter(dlib::point &p1, dlib::point &p2, int id_);
	void addTracker(CT::Tracker& tracker);
	int removeTracker(CT::Tracker& tracker);
	int getIn() const;
	int getOut() const;
	void updateSituation();
	int getId() const;
	void setLine(dlib::point &p1, dlib::point &p2);
	std::vector<CT::Tracker> trackers;
private:
	int m_id;
	dlib::point m_p1;
	dlib::point m_p2;
	float m_a;
	float m_b;
	int m_in, m_out, m_stay;
	bool isInside(const dlib::point& c) const;
};


}//Namespace CT

#endif /* COUNTER_H_ */
