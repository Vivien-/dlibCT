// Copyright (C) 2011  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_METADATA_EdITOR_H__
#define DLIB_METADATA_EdITOR_H__

#include <dlib/gui_widgets.h>
#include "dlib/data_io.h"

namespace CT {
class Controller;
}

namespace CT {

// ----------------------------------------------------------------------------------------

class metadata_editor : public dlib::drawable_window {
public:
    metadata_editor();
    ~metadata_editor();
    dlib::image_display display;
    CT::Controller * m_controller;
    void setController(CT::Controller * ctrl);

private:
    virtual void on_window_resized();
    virtual void on_keydown(unsigned long key, bool is_printable, unsigned long state);
    virtual void on_mouse_down(unsigned long btn, unsigned long state, long x, long y, bool is_double_click);

    dlib::label overlay_label_name;
    dlib::point p1;
    dlib::point p2;
    int click_step;
};

// ----------------------------------------------------------------------------------------

} //namespace CT

#endif // DLIB_METADATA_EdITOR_H__

