/*
 * gui.cpp
 *
 *  Created on: 14 avr. 2016
 *      Author: mohammed
 */

#include "gui.h"

namespace CT {

gui::gui() : svm(*this), display(*this), video(*this), url(*this), local(*this), link(*this), run(*this), stop(*this), draw_lines(*this){
	m_controller = nullptr;

	//main window settings
	set_title("Crowd estimator");
	set_size(800,500);

	//svm button settings
	svm.set_pos(10,100);
	svm.set_name("SVM file");
	svm.set_click_handler(*this, &gui::svm_handler);

	//video button settings
	video.set_pos(10,50);
	video.set_name("Video");
	video.set_click_handler(*this, &gui::video_handler);


	//
	url.set_pos(10, 10);
	url.set_name("url");
	url.set_click_handler(*this, &gui::url_handler);

	//
	local.set_pos(55, 10);
	local.set_name("local");
	local.set_click_handler(*this, &gui::local_handler);
	local.set_checked();
	//
	link.set_pos(10, 50);
	link.set_size(90,25);
	link.set_text("http://");
	link.hide();


	//
	run.set_pos(10,400);
	run.set_name("run");

	//
	stop.set_pos(50,400);
	stop.set_name("stop");

	//
	draw_lines.set_pos(10,300);
	draw_lines.set_name("draw lines");
	draw_lines.set_click_handler(*this, &gui::draw_lines_handler);
	choose_local = true;
	can_draw_lines = false;
	click_step = 0;
	//video display settings
	display.set_pos(200,0);
	display.set_size(600,500);
	display.set_image_clicked_handler(*this, &gui::display_window_handler);
}

gui::~gui() {
	// TODO Auto-generated destructor stub
}

void gui::url_handler(){
	local.set_unchecked();
	video.hide();
	link.show();
	choose_local = false;
}

void gui::local_handler(){
	url.set_unchecked();
	link.hide();
	video.show();
	choose_local = true;
}


void gui::video_handler(){
	open_existing_file_box(*this, &gui::open_video_handler);
}

void gui::open_video_handler(const std::string& file_name){
	video_file = file_name;
}

void gui::svm_handler(){
	open_existing_file_box(*this, &gui::open_svm_handler);
}

void gui::open_svm_handler(const std::string& file_name){
	svm_file = file_name;
}


void gui::draw_lines_handler(){
	can_draw_lines = !can_draw_lines;
	click_step = 0;
}


void gui::display_window_handler(const dlib::point& p, bool is_double_click, unsigned long state){
	if(is_double_click && can_draw_lines) {
		if(click_step == 0) {
			p1 = dlib::point(p.x(),p.y());
			click_step = 1;
		}
		else {
			p2 = dlib::point(p.x(),p.y());
			m_controller->addLine(p1,p2);
			display.add_overlay(dlib::image_window::overlay_line(p1, p2, dlib::rgb_pixel(0,0,255)));
			click_step = 0;
		}
	} else if(is_double_click && !can_draw_lines) {
		//controller.removeLine(controller.getBestLine(p));

	}
}

void gui::setController(CT::Controller * ctrl) {
	m_controller = ctrl;
}

} /* namespace CT */
