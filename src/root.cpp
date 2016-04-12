/*
 * root.cpp
 *
 *  Created on: 18 mars 2016
 *      Author: vivien
 */

#include "root.h"
#include <chrono>

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

void Root::run(int argc, char* argv[]) {
	cv::VideoCapture cap;
	if (argc==1) {
	    cap = cv::VideoCapture(0);
	} else {
	    cap = cv::VideoCapture(argv[1]);
	}

//	cv::Mat frame;
//	cap >> frame;
//	std::cout<<frame<<std::endl;
//	while(!m_window->is_closed()){
//		cap >>frame;
//		cv::imshow("toto",frame);
//	}
	/**
	 * If you want to use your own detecter uncomment the following lines
	 *
	 */
	typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
	dlib::object_detector<image_scanner_type> d;
	dlib::deserialize("/home/mohammed/Bureau/best.svm") >> d;
	/**/

	/**
	 * Else using the given get the face detector
	 */
	//dlib::frontal_face_detector d = dlib::get_frontal_face_detector();

	uint64 nfrm = 0;
	dlib::image_display & dplay = m_window->display;

	//Check much time is consummed for each functions
	double df_t;
	double p_t;
	double u_t;
	double d_t;
	double uc_t;
	double ps_t;

	std::chrono::high_resolution_clock::time_point initial_t = std::chrono::high_resolution_clock::now();
	cv::Mat temp, temp2;

	// Grab and process frames until the main window is closed by the user.
	while(!m_window->is_closed()) {
		nfrm++;
		std::chrono::high_resolution_clock::time_point current_t = std::chrono::high_resolution_clock::now();
		// Grab a frame
		//cap.read(frame);
		//cv::Mat temp(frame, CV_LOAD_IMAGE_COLOR);
		cap >> temp;
		// Turn OpenCV's Mat into something dlib can deal with. don't modify temp while using cimg.
		dlib::cv_image<dlib::bgr_pixel> cimg(temp);
		dplay.set_image(cimg);

		// Detect faces every 50 frames (less laggy)
		std::chrono::high_resolution_clock::time_point t6 = std::chrono::high_resolution_clock::now();
		if(nfrm % 50 == 0) {
			std::vector<dlib::rectangle> faces = d(cimg);
			m_controller->process(faces, cimg);
		}
		std::chrono::high_resolution_clock::time_point t8 = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		//update tracker
		m_controller->update(cimg);
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		//display tracked object and lines
		m_controller->display();
		std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
		//update number of people that crossed each lines for the current frame
		m_controller->updateCountersSituation();
		std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
		//display those numbers
		m_controller->printSituation();
		std::chrono::high_resolution_clock::time_point t5 = std::chrono::high_resolution_clock::now();

//		std::cout<<"Current FPS: "<<(double)1.0000000/(double)(std::chrono::duration_cast<std::chrono::microseconds>( t5 - current_t ).count())*1000000<<std::endl;
//		std::cout<<"Time for detection (+process()) execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t8 - t6 ).count()<<std::endl;
//		std::cout<<"Time for update() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count()<<std::endl;
//		std::cout<<"Time for display() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count()<<std::endl;
//		std::cout<<"Time for updateCountersSituation() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count()<<std::endl;
//		std::cout<<"Time for printSituation() execution: "<<std::chrono::duration_cast<std::chrono::microseconds>( t5 - t4 ).count()<<std::endl;
//
//		df_t += std::chrono::duration_cast<std::chrono::microseconds>( t8 - t6 ).count();
//		u_t += std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
//		d_t += std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
//		uc_t += std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count();
//		ps_t += std::chrono::duration_cast<std::chrono::microseconds>( t5 - t4 ).count();
//		//TODO: mean of each funcution execution time
//
//		if(nfrm == cap.get(CV_CAP_PROP_FRAME_COUNT) - 2) {
//			std::cout<<"\nMeans of time: \n\t"
//					<<"detection faces(): "<<df_t/nfrm<<" microseconds\n\t"
//					<<"update(): "<<u_t/nfrm<<" microseconds\n\t"
//					<<"display(): "<<d_t/nfrm<<" microseconds\n\t"
//					<<"updateCountersSituation(): "<<uc_t/nfrm<<" microseconds\n\t"
//					<<"printSituation(): "<<ps_t/nfrm<<" microseconds\n\t"
//					<<"mean of FPS: "<<(double)nfrm/(double)(std::chrono::duration_cast<std::chrono::microseconds>( current_t - initial_t ).count() / 1000000)<<std::endl;
//
//			std::cout<<"Sum of time: \n\t"
//					<<"detection faces(): "<<df_t<<" microseconds\n\t"
//					<<"update(): "<<u_t<<" microseconds\n\t"
//					<<"display(): "<<d_t<<" microseconds\n\t"
//					<<"updateCountersSituation(): "<<uc_t<<" microseconds\n\t"
//					<<"printSituation(): "<<ps_t<<" microseconds"<<std::endl;
//		}

	}
	//opencv videocapture cause some memory leak... even with the release() call (which is probably useless)
	cap.release();
}

}
