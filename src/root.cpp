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
}

Root::~Root()
{
	delete m_controller;
	delete m_window;
}

void Root::run(int argc, char* argv[]) {
	std::string _video_file = "";
	std::string _svm_file = "";
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--video" || std::string(argv[i]) == "-v") {
			_video_file = std::string(argv[i + 1]);
		} else if (std::string(argv[i]) == "--svm" || std::string(argv[i]) == "-s") {
			_svm_file = std::string(argv[i + 1]);
		}
	}
	m_window->show();
	m_controller->setEditor(m_window);
	m_window->setController(m_controller);
	m_window->setParameters(_video_file, _svm_file);
	m_window->run_listener();
	m_window->wait_until_closed();
}

}
