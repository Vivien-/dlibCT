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

namespace CT {

class Root {
public:
	virtual ~Root();
	static Root & get();
	CT::Controller & controller();
	CT::metadata_editor & window();
	void run();

private:
    Root();
    static Root * s_instance;

    CT::metadata_editor * m_window;
    Controller * m_controller;
};

} /* namespace CT */

#endif /* SRC_ROOT_H_ */
