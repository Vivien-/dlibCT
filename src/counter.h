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
#include "identifiert.h"
#include "line.h"

namespace CT {
class Tracker;
}

namespace CT {


class Counter {
public:
	Counter();
	virtual ~Counter();
	Counter(CT::Line &l, int id_);
	void addTracker(CT::Tracker& tracker);
	int removeTracker(CT::Tracker& tracker);
	int getIn() const;
	int getOut() const;
	void updateSituation();
	CT::identifier_t getId() const;
	void setLine(CT::Line &l);

private:
	std::vector<CT::Tracker > m_trackers;
	CT::identifier_t m_id;
	CT::Line m_line;
	float m_a;
	float m_b;
	int m_in, m_out, m_stay;
	bool isInside(const dlib::point& c) const;
};


} //Namespace CT

#endif /* COUNTER_H_ */
