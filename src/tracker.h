/*
 * tracker.h
 *
 *  Created on: 11 mars 2016
 *      Author: vivien
 */

#ifndef SRC_TRACKER_H_
#define SRC_TRACKER_H_
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "identifiert.h"

namespace CT {
class Counter;
}

namespace CT {

class Tracker {
public:
	Tracker();
	virtual ~Tracker();
	void initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect);
	dlib::point initial() const;
	dlib::point current() const;
	dlib::correlation_tracker getTracker();
	void setInitial(const dlib::point &p);
	void setCurrent(const dlib::point &p);
	CT::identifier_t getId() const;
	double update(dlib::cv_image<dlib::bgr_pixel> & img);
	void setCounter(CT::Counter * ctr);

private:
	dlib::correlation_tracker m_tr;
	CT::identifier_t m_id;
	CT::identifier_t m_counter_id;
	dlib::point m_current;
	dlib::point m_initial;
};

} /* namespace CT */

#endif /* SRC_TRACKER_H_ */
