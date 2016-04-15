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
	dlib::image_display display;
    CT::Controller * m_controller;
    void setController(CT::Controller * ctrl);

private:
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
	dlib::button svm;
	dlib::button video;
	dlib::radio_button url;
	dlib::radio_button local;
	dlib::text_box link;
	dlib::button run;
	dlib::button stop;
	dlib::check_box draw_lines;
	bool choose_local;
	std::string svm_file;
	std::string video_file;
	bool can_draw_lines;
	int click_step;
	dlib::point p1;
	dlib::point p2;
};

} /* namespace CT */

#endif /* SRC_GUI_H_ */
