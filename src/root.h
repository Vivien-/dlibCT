/*
 * root.h
 *
 *  Created on: 18 mars 2016
 *      Author: vivien
 */

#ifndef SRC_ROOT_H_
#define SRC_ROOT_H_
#include "controller.h"
#include "metadata_editor.h"
#include "gui.h"

namespace CT {

class Root {
public:
	virtual ~Root();
	static Root & get();
	CT::Controller & controller();
	CT::gui & window();
	void run(int argc, char* argv[]);

private:
    Root();
    static Root * s_instance;

    CT::gui * m_window;
    Controller * m_controller;
};

} /* namespace CT */

#endif /* SRC_ROOT_H_ */
