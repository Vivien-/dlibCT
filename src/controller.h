/*
 * controller.h
 *
 *  Created on: 11 mars 2016
 *      Author: vivien & mohammed
 *
 *  The class Controller manages the diferrent parts of the software : lines, counters and trackers
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include "tracker.h"
#include "counter.h"
#include "line.h"

namespace CT {

class Controller {
public:
	Controller();
	virtual ~Controller();
	// Track newly detected faces if don't overlap with already tracked face
	void process(std::vector<dlib::rectangle> & objDetected, dlib::cv_image<dlib::bgr_pixel> & cimg);
	// Update all trackers position for the current frame
	void update(dlib::cv_image<dlib::bgr_pixel> & cimg);
	// Display all the trackers on the current frame
	void display(dlib::image_window &win, dlib::cv_image<dlib::bgr_pixel> & cimg);
	// Create a new line (following the user chosing the 2 endpoints on the image)
	void addLine(dlib::point &p1, dlib::point &p2);
	// For each line, update the number of tracker that entered or left that line
	void updateCountersSituation();
	// Attach a tracker to a counter (a counter own multiple trackers)
	void setTrackerToCounter(const CT::identifier_t tr, const CT::identifier_t ctr);
	// debug
	void printSituation();

private:
	// Return the best line (closest ?) to the object
	CT::identifier_t getBestLine(dlib::point &p);
	std::map<CT::identifier_t, CT::Tracker> trackers;
	// Next id is used to determinate the next line id and counter id (which are the same)
	//CT::identifier_t next_id;
	std::map<CT::identifier_t, CT::Counter> counters;
	std::map<CT::identifier_t, CT::Line> lines;
	// Threshold value of wether we keep a tracker or remove it (depending of the confidence of the tracker)
 	double m_threshold;
};

} /* namespace CT */

#endif /* SRC_CONTROLLER_H_ */
