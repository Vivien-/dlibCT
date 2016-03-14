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

namespace CT {

class Tracker {
public:
	Tracker();
	virtual ~Tracker();
	void initTrack(dlib::cv_image<dlib::bgr_pixel> & img, dlib::rectangle & rect);
	void track(dlib::cv_image<dlib::bgr_pixel> & img);
	dlib::point initial() const;
	dlib::point current() const;
	void setInitial(const dlib::point &p);
	void setCurrent(const dlib::point &p);
	int getId() const;

private:
	dlib::correlation_tracker m_tr;
	int m_id;
	dlib::point m_current;
	dlib::point m_initial;
};

} /* namespace CT */

#endif /* SRC_TRACKER_H_ */
