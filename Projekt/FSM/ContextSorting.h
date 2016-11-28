/*
 * ContextSorting.h
 *
 *  Created on: 21.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTSORTING_H_
#define CONTEXTSORTING_H_

#include <iostream>
#include "Puck.h"
using namespace std;

struct Datacs{
	Datacs(): currentPm(TYPE404PM), currentPh(TYPE404PH), sequenceOK(0){}
	Puckmaterial currentPm;
	Puckdrillhole currentPh;
	int sequenceOK;
};

class ContextSorting {
public:
	static ContextSorting* getInstance();

	void transact() {
		statePtr->transact();
	} // context delegates signals to state

	void setCurrentPh(Puckdrillhole currentPh) {
		csdata.currentPh = currentPh;
	}

	void setCurrentPm(Puckmaterial currentPm) {
		csdata.currentPm = currentPm;
	}

	int getSequenceOk() const {
		return csdata.sequenceOK;
	}

	virtual ~ContextSorting(){};

private:
	struct Sorting { //top-level state
		Datacs* data;
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Sorting {
		virtual void transact() {
			if (data->currentPh == DRILL_HOLE_UPSIDE && data->currentPm == PLASTIC) {
				data->sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal1;
			} else {
				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideWoMetal1: public Sorting {
		virtual void transact() {
			if (data->currentPh == DRILL_HOLE_UPSIDE && data->currentPm == PLASTIC) {
				data->sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal2;
			} else {
				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideWoMetal2: public Sorting {
		virtual void transact() {
			if (data->currentPh == DRILL_HOLE_UPSIDE && data->currentPm == METAL) {
				data->sequenceOK = 1;
				new (this) DrillHoleUpSideMetal;
			} else {
				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideMetal: public Sorting {
		virtual void transact() {
			if (data->currentPh == DRILL_HOLE_UPSIDE && data->currentPm == PLASTIC) {
				data->sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal1;
			} else {
				data->sequenceOK = 0;
			}
		}
	};

	static ContextSorting* instance_;
	StateStart stateMember; //The memory for the state is part of context object
	Datacs csdata;

	ContextSorting() : statePtr(&stateMember), csdata() // assigning start state
	{
		statePtr->data = &csdata;
	}
	ContextSorting(const ContextSorting& other);
	ContextSorting& operator=(const ContextSorting& other);
};

#endif /* CONTEXTSORTING_H_ */
