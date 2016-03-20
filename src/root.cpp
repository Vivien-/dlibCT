/*
 * root.cpp
 *
 *  Created on: 18 mars 2016
 *      Author: vivien
 */

#include "root.h"

namespace CT {

Root * Root::s_instance = nullptr;

Root & Root::get() {
	if (!s_instance) {
		s_instance = new Root;
	}
	return *s_instance;
}

Root::Root()
{
	m_controller = new CT::Controller();
	m_window = new CT::metadata_editor();
	m_controller->setEditor(m_window);
	m_window->setController(m_controller);
}

Root::~Root()
{
	delete m_controller;
	delete m_window;
}

void Root::run() {
	//get video from a device ( 0 for the webcam)
	cv::VideoCapture cap(0);
	//get the face detector
	dlib::frontal_face_detector d = dlib::get_frontal_face_detector();
	uint64 nfrm = 0;
	dlib::image_display & dplay = m_window->display;

	// Grab and process frames until the main window is closed by the user.
	while(!m_window->is_closed()) {
		// Grab a frame
		cv::Mat temp;
		cap >> temp;

		// Turn OpenCV's Mat into something dlib can deal with. don't modify temp while using cimg.
		dlib::cv_image<dlib::bgr_pixel> cimg(temp);
		dplay.set_image(cimg);

		// Detect faces every 20 frames (less laggy)
		if(nfrm++ % 20 == 0) {
			std::vector<dlib::rectangle> faces = d(cimg);
			m_controller->process(faces, cimg);
		}
		//update tracker
		m_controller->update(cimg);
		//display tracked object and lines
		m_controller->display();
		//update number of people that crossed each lines for the current frame
		m_controller->updateCountersSituation();
		//display those numbers
		m_controller->printSituation();
	}
	//opencv videocapture cause some memory leak... even with the release() call (which is probably useless)
	cap.release();
}

}
