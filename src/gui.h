/*
 * gui.h
 *
 *  Created on: 14 avr. 2016
 *      Author: mohammed
 */

#ifndef SRC_GUI_H_
#define SRC_GUI_H_
#include <iostream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include "controller.h"
namespace CT {

class gui : public dlib::drawable_window{
public:
	gui();
	virtual ~gui();
    void setController(CT::Controller * ctrl);
    CT::Controller* controller();
	dlib::image_display display;
	void run_listener();
	void setParameters(const std::string & _video_file, const std::string & _svm_file, bool _ready_to_run);

private:
    CT::Controller * m_controller;
	void svm_handler();
	void display_handler();
	void video_handler();
	void url_handler();
	void local_handler();
	void link_handler();
	CT::Line getClosestLine(dlib::point p);
	void open_video_handler(const std::string& file_name);
	void open_svm_handler(const std::string& file_name);
	void draw_lines_handler();
	void display_window_handler(const dlib::point& p, bool is_double_click, unsigned long state);
	void delete_line_handler(const dlib::image_display::overlay_line);
	void run_handler();
	void progress_bar_handler();
	virtual void on_keydown(unsigned long key, bool is_printable, unsigned long state);
	virtual void on_window_resized();
	dlib::button svm;
	dlib::button video;
	dlib::radio_button url;
	dlib::radio_button local;
	dlib::text_box link;
	dlib::scroll_bar progress;
	dlib::button run;
	dlib::button stop;
	dlib::check_box draw_lines;
	bool choose_local;
//	std::string svm_file;
	std::vector<dlib::file> svms;
	std::string video_file;
	bool can_draw_lines;
	int click_step;
	dlib::point p1;
	dlib::point p2;
	cv::VideoCapture cap;
	bool ready_to_run;
	std::string directory;
	std::vector < dlib::object_detector< dlib::scan_fhog_pyramid<dlib::pyramid_down<3> > > > detectors;
};

} /* namespace CT */

#endif /* SRC_GUI_H_ */
