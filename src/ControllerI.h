/*
 * ControllerI.h
 *
 *  Created on: 2 mai 2016
 *      Author: mohammed
 */

#ifndef SRC_CONTROLLERI_H_
#define SRC_CONTROLLERI_H_

namespace CT {
class ControllerI {
public:
	ControllerI();
	virtual ~ControllerI();
	virtual void addCounter(CT::CounterI &counter)=0;
	virtual void removeCounter(CT::identifier_t counter)=0;
	virtual CT::CounterI & getCounter(CT::identifier_t counter)=0;
	virtual std::map<CT::identifier_t,CT::CounterI> getCounters()=0;
	virtual void addTracker(CT::TrackerI &tracker)=0;
	virtual void removeTracker(CT::identifier_t tracker)=0;
private:
//	DetectorI<T> d;
//	std::map<CT::identifier_t, CT::CounterI> counters;
//	std::map<CT::identifier_t, CT::TrackerI<T>> trackers;
//	std::map<CT::identifier_t, std::set<CT::identifier_t>> associationTable;
};

} /* namespace CT */

#endif /* SRC_CONTROLLERI_H_ */
