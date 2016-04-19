/*
 * gui.cpp
 *
 *  Created on: 14 avr. 2016
 *      Author: mohammed
 */

#include "gui.h"
#include <chrono>
namespace CT {



/*** ONLY USED SO WE CAN SEE EXECUTION TIME ***/

//shorter definition for execution time infos debug
typedef std::chrono::high_resolution_clock::time_point TIME_TYPE;

/**
 *  for shorter definition timer for execution time debug
 **/

TIME_TYPE clock_now() {
	return std::chrono::high_resolution_clock::now();
}

void infoExecTimeLast(int nfrm, double df_t, double u_t, double d_t, double uc_t, double ps_t, TIME_TYPE current_t, TIME_TYPE initial_t) {
	std::cout<<"\nMeans of time: \n\t"
			<<"detection faces(): "<<df_t/nfrm<<" microseconds\n\t"
			<<"update(): "<<u_t/nfrm<<" microseconds\n\t"
			<<"display(): "<<d_t/nfrm<<" microseconds\n\t"
			<<"updateCountersSituation(): "<<uc_t/nfrm<<" microseconds\n\t"
			<<"printSituation(): "<<ps_t/nfrm<<" microseconds\n\t"
			<<"mean of FPS: "<<(double)nfrm/(double)(std::chrono::duration_cast<std::chrono::microseconds>(current_t - initial_t ).count() / 1000000)<<std::endl;

	std::cout<<"Sum of time: \n\t"
			<<"detection faces(): "<<df_t<<" microseconds\n\t"
			<<"update(): "<<u_t<<" microseconds\n\t"
			<<"display(): "<<d_t<<" microseconds\n\t"
			<<"updateCountersSituation(): "<<uc_t<<" microseconds\n\t"
			<<"printSituation(): "<<ps_t<<" microseconds"<<std::endl;
}

void infoExecTime(TIME_TYPE t1, TIME_TYPE t2, TIME_TYPE t3, TIME_TYPE t4, TIME_TYPE t5, TIME_TYPE t6, TIME_TYPE current_t) {
	std::cout<<"Current FPS: "<<(double)1.0000000/(double)(std::chrono::duration_cast<std::chrono::microseconds>( t5 - current_t ).count())*1000000<<std::endl;
	std::cout<<"Time for detection (+process()) execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t1 - t6 ).count()<<std::endl;
	std::cout<<"Time for update() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()<<std::endl;
	std::cout<<"Time for display() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count()<<std::endl;
	std::cout<<"Time for updateCountersSituation() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count()<<std::endl;
	std::cout<<"Time for printSituation() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t5 - t4 ).count()<<std::endl;
}

/*** END FUNCTIONS TO DISPLAY EXECUTION TIME ***/




gui::gui() : svm(*this), display(*this), video(*this), url(*this), local(*this), link(*this), run(*this), stop(*this), draw_lines(*this){
	m_controller = nullptr;

	//main window settings
	set_title("Crowd estimator");
	set_size(900,600);

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
	run.set_click_handler(*this, &gui::run_handler);
	//
	stop.set_pos(100,400);
	stop.set_name("stop");

	//
	draw_lines.set_pos(10,300);
	draw_lines.set_name("draw lines");
	draw_lines.set_click_handler(*this, &gui::draw_lines_handler);
	choose_local = true;
	can_draw_lines = false;
	click_step = 0;
	ready_to_run = false;
	//video display settings
	display.set_pos(200,0);
	display.set_size(800,600);
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
	cap = cv::VideoCapture(video_file);
	cv::Mat temp;
	cap >> temp;
	dlib::cv_image<dlib::bgr_pixel> cimg(temp);
	const int left_menu_size = 200;
	set_size(display.left() + cimg.nc() + 4 + left_menu_size, display.top() + cimg.nr() + 4 + left_menu_size);
	display.set_size(display.left() + cimg.nc() + 4, display.top() + cimg.nr() + 4);
	display.set_image(cimg);
	cap.release();
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
		CT::identifier_t id =  m_controller->getBestLine(p);
		m_controller->addSelectedLine(id);
	}
}

CT::Controller* gui::controller() {
	return m_controller;
}

void gui::setController(CT::Controller * ctrl) {
	m_controller = ctrl;
}


void gui::run_listener(){
	while(true){
		std::cout<<"listener: "<<ready_to_run<<std::endl;
		if(ready_to_run){
			cap = cv::VideoCapture(video_file);
			typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
			dlib::object_detector<image_scanner_type> d;
			dlib::deserialize(svm_file) >> d;
			uint64 nfrm = 0;

			//Check how much time is consummed for each functions
			double df_t;
			double p_t;
			double u_t;
			double d_t;
			double uc_t;
			double ps_t;
			TIME_TYPE initial_t = clock_now();

			cv::Mat temp;
			// Grab and process frames until the main window is closed by the user.
			while(cap.read(temp)) {
				std::cout<<"Reading: "<<ready_to_run<<std::endl;
				if(ready_to_run){
					nfrm++;
					TIME_TYPE current_t = clock_now();
					// Grab a frame
					// Turn OpenCV's Mat into something dlib can deal with. don't modify temp while using cimg.
					dlib::cv_image<dlib::bgr_pixel> cimg(temp);
					display.set_image(cimg);
					// Detect faces every 10 frames (less laggy)
					TIME_TYPE t6 = clock_now();
					if(nfrm % 10 == 0) {
						std::vector<dlib::rectangle> faces = d(cimg);
						m_controller->process(faces, cimg);
					}

					TIME_TYPE t1 = clock_now();
					m_controller->update(cimg);              //update tracker
					TIME_TYPE t2 = clock_now();
					m_controller->updateCountersSituation(); //update number of people that crossed each lines for the current frame
					TIME_TYPE t3 = clock_now();
					m_controller->display();                 //display tracked object and lines
					TIME_TYPE t4 = clock_now();
					m_controller->printSituation();          //display those numbers
					TIME_TYPE t5 = clock_now();

					infoExecTime(t1,t2,t3,t4,t5,t6,current_t);

					df_t += std::chrono::duration_cast<std::chrono::microseconds>( t1 - t6 ).count();
					u_t  += std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
					uc_t  += std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
					d_t += std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count();
					ps_t += std::chrono::duration_cast<std::chrono::microseconds>( t5 - t4 ).count();

					if(nfrm == cap.get(CV_CAP_PROP_FRAME_COUNT) - 2) {
						infoExecTimeLast(nfrm, df_t, u_t, d_t, uc_t, ps_t, current_t, initial_t);
						break;
					}
				}
			}
			//opencv videocapture cause some memory leak... even with the release() call (which is probably useless)
			cap.release();
			ready_to_run = false;
		}
	}
}

void gui::run_handler(){
	ready_to_run = !ready_to_run;
	if(ready_to_run)
		run.set_name("pause");
}


} /* namespace CT */
