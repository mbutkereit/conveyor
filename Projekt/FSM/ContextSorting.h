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

class ContextSorting {
public:
	ContextSorting(): statePtr(&stateMember), currentPm(TYPE404PM), currentPh(TYPE404PH), sequenceOK(0)// assigning start state
    {}

	void transact(){statePtr->transact();} // context delegates signals to state

	void setCurrentPh(Puckdrillhole currentPh) {
		this->currentPh = currentPh;
	}

	void setCurrentPm(Puckmaterial currentPm) {
		this->currentPm = currentPm;
	}

	int getSequenceOk() const {
		return sequenceOK;
	}

private:
	struct Sorting { //top-level state
		virtual void transact() {
		}
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateStart: public Sorting {
		virtual void transact() {
			if(currentPh == DRILL_HOLE_UPSIDE && currentPh == PLASTIC)
			{
				sequenceOK = 1;
				new (this) DrillHoleUpSideWoMetal1;
			}
			else
			{
				sequenceOK = 0;
			}
		}
	};

		struct DrillHoleUpSideWoMetal1: public Sorting {
			virtual void transact() {
				if (currentPh == DRILL_HOLE_UPSIDE && currentPh == PLASTIC)
				{
					sequenceOK = 1;
					new (this) DrillHoleUpSideWoMetal2;
				}
				else
				{
					sequenceOK = 0;
				}
			}
		};

		struct DrillHoleUpSideWoMetal2: public Sorting
		{
			virtual void transact() {
				if (currentPh == DRILL_HOLE_UPSIDE && currentPh == METAL)
				{
					sequenceOK = 1;
					new (this) DrillHoleUpSideMetal;
				}
				else
				{
					sequenceOK = 0;
				}
			}
		};

		struct DrillHoleUpSideMetal: public Sorting
		{
			virtual void transact() {
				if (currentPh == DRILL_HOLE_UPSIDE && currentPh == PLASTIC) {
					sequenceOK = 1;
					new (this) DrillHoleUpSideWoMetal1;
				}
				else
				{
					sequenceOK = 0;
				}
			}
		};

		StateStart stateMember; //The memory for the state is part of context object
		Puckmaterial currentPm;
		Puckdrillhole currentPh;
		int sequenceOK;
};

#endif /* CONTEXTSORTING_H_ */
