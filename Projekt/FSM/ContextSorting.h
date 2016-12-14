/*
 * ContextSorting.h
 *
 *  Created on: 21.11.2016
 *      Author: abs949
 */

#ifndef CONTEXTSORTING_H_
#define CONTEXTSORTING_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Puck.h"

struct Datacs{
	Datacs(): sequenceOK(0),currentPt(TYPE404PT){}

	int sequenceOK;
	PuckType currentPt;
};

class ContextSorting {
public:
	static ContextSorting* getInstance();

	void transact() {
		statePtr->transact();
	} // context delegates signals to state



	int getSequenceOk() const {
		return csdata.sequenceOK;
	}

	void setCurrentPt(PuckType currentPt) {
			csdata.currentPt = currentPt;
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
			LOG_DEBUG <<"State: Sorting: StateStart: \n";

			if (data->currentPt == DRILL_HOLE_UPSIDE_PLASTIC) {

				data->sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal1;
			} else {

				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideWoMetal1: public Sorting {
		virtual void transact() {
			LOG_DEBUG <<"State: Sorting: DrillHoleUpSideWoMetal1 \n";
			if (data->currentPt == DRILL_HOLE_UPSIDE_PLASTIC) {

				data->sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal2;
			} else {

				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideWoMetal2: public Sorting {
		virtual void transact() {
			LOG_DEBUG <<"State: Sorting: DrillHoleUpSideWoMetal2 \n";
			if (data->currentPt == DRILL_HOLE_UPSIDE_METAL) {

				data->sequenceOK = 1;
				new (this) DrillHoleUpSideMetal;
			} else {

				data->sequenceOK = 0;
			}
		}
	};

	struct DrillHoleUpSideMetal: public Sorting {
		virtual void transact() {
			LOG_DEBUG <<"State: Sorting: DrillHoleUpSideMetal \n";
			if (data->currentPt == DRILL_HOLE_UPSIDE_PLASTIC) {

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
