// Copyright (C) 2011  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include "metadata_editor.h"
#include <dlib/array.h>
#include <dlib/queue.h>
#include <dlib/static_set.h>
#include <dlib/misc_api.h>
#include <dlib/image_io.h>
#include <dlib/array2d.h>
#include <dlib/pixel.h>
#include <sstream>
#include <ctime>
#include "controller.h"

using namespace std;
using namespace dlib;

namespace CT {

metadata_editor::metadata_editor() : display(*this), overlay_label_name(*this) {
	click_step = 0;
	// set the size of this window.
	on_window_resized() ;

	// make sure the window is centered on the screen.
	unsigned long width, height;
	get_size(width, height);
	unsigned long screen_width = 500;
	unsigned long screen_height = 500;
	get_display_size(screen_width, screen_height);
	set_pos((screen_width-width)/2, (screen_height-height)/2);
	display.set_size(screen_width, screen_height);

	set_title("Crowd movement estimation");
	show();
} 

metadata_editor::~metadata_editor() {
	close_window();
}

void metadata_editor::on_window_resized() {
	drawable_window::on_window_resized();
	unsigned long width, height;
	get_size(width, height);
	display.set_size(width - display.left(), height - display.top());
}

void metadata_editor::on_keydown(unsigned long int key, bool is_p, unsigned long int state) {
	std::cout<<"Key: "<<key<<"is_printable (?): " <<is_p<<"State: "<<state<<std::endl;
}

void metadata_editor::on_mouse_down(unsigned long btn, unsigned long state, long x, long y, bool is_double_click) {
	if(is_double_click) {
		if(click_step == 0) {
			p1 = dlib::point(x,y);
			click_step = 1;
		}
		else {
			p2 = dlib::point(x,y);
			m_controller->addLine(p1,p2);
			click_step = 0;
		}
	}
}

void metadata_editor::setController(CT::Controller * ctrl) {
	m_controller = ctrl;
}

} //Namespace CT
