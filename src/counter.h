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
//	Counter();
	Counter(CT::identifier_t id);
	virtual ~Counter();
	void addTracker(CT::identifier_t tracker);
	void removeTracker(CT::identifier_t tracker);
	std::map<CT::identifier_t, int> getIdTrackers();
	int getIn() const;
	int getOut() const;
	void incrIn();
	void incrOut();
	int getCount();
	//void updateSituation();
	CT::identifier_t getId() const;
	void setLine(CT::identifier_t l);
	CT::identifier_t getLine();

private:
	std::map<CT::identifier_t, CT::identifier_t> m_id_trackers;
	CT::identifier_t m_id;
	CT::identifier_t m_id_line;
	int m_in, m_out, m_stay;
	//bool isInside(const dlib::point& c) const;
};


} //Namespace CT

#endif /* COUNTER_H_ */
