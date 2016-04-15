/*
 * root.cpp
 *
 *  Created on: 18 mars 2016
 *      Author: vivien
 */

#include "root.h"
#include <chrono>

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
	m_window = new CT::gui();
	m_window->show();
	m_controller->setEditor(m_window);
	m_window->setController(m_controller);
}

Root::~Root()
{
	delete m_controller;
	delete m_window;
}

void Root::run(int argc, char* argv[]) {
//	CT::gui my_gui;
//	my_gui.show();
//	my_gui.wait_until_closed();

	cv::VideoCapture cap;
	if (argc==1) {
	    cap = cv::VideoCapture(0);
	} else {
	    cap = cv::VideoCapture(argv[1]);
	}

	/**
	 * If you want to use your own detecter uncomment the following lines
	 */
	typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
	dlib::object_detector<image_scanner_type> d;
	dlib::deserialize("/home/mohammed/dlib-18.18/examples/build/object_detector.svm") >> d;

	/**
	 * Else using the given get the face detector
	 */
	//dlib::frontal_face_detector d = dlib::get_frontal_face_detector();

	uint64 nfrm = 0;
	dlib::image_display & dplay = m_window->display;

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
	while(!m_window->is_closed()) {
		nfrm++;
		TIME_TYPE current_t = clock_now();
		// Grab a frame
		cap >> temp;
		// Turn OpenCV's Mat into something dlib can deal with. don't modify temp while using cimg.
		dlib::cv_image<dlib::bgr_pixel> cimg(temp);
		dplay.set_image(cimg);
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
	//opencv videocapture cause some memory leak... even with the release() call (which is probably useless)
	cap.release();
}

}
