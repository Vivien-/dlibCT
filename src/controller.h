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
class gui;
}

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
	void display();
	// Create a new line (following the user chosing the 2 endpoints on the image)
	void addLine(dlib::point &p1, dlib::point &p2);
	// For each line, update the number of tracker that entered or left that line
	void updateCountersSituation();
	// Attach a tracker to a counter (a counter own multiple trackers)
	void setTrackerToCounter(const CT::identifier_t tr, const CT::identifier_t ctr);
	// remove line id frome lines
	void removeLine(CT::identifier_t id);
	// debug
	void printSituation();
	//writable editor window
	CT::gui * m_editor;
	void setEditor(CT::gui* w);
	// Return the best line (closest ?) to the object
	CT::identifier_t getBestLine(dlib::point p);
	std::map<CT::identifier_t, CT::Line> getLines();
	// Add a line to the set of selected lines
	void addSelectedLine(CT::identifier_t id);
	// Delete every line in the set of selected lines
	void deleteSelectedLines();

private:
	//draw tracked object on frame
	void displayTrackers();
	std::set<CT::identifier_t> to_be_removed;
	std::map<CT::identifier_t, CT::Tracker> trackers;
	std::map<CT::identifier_t, CT::Counter> counters;
	std::map<CT::identifier_t, CT::Line> lines;
	int maxFreezDuration;
	// Threshold value of wether we keep a tracker or remove it (depending of the confidence of the tracker)
 	double m_threshold;
 	void setTrackersToCounters();
 	void updateCounters();
};

} /* namespace CT */

#endif /* SRC_CONTROLLER_H_ */
