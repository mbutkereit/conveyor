/*
 * Puck.h
 *
 *  Created on: 21.11.2016
 *      Author: abs949
 */

#ifndef PUCK_H_
#define PUCK_H_

	enum Pucktype
	{
		METAL, PLASTIC, DRILL_HOLE_UPSIDE, NO_DRILL_HOLE, TYPE404
	};
class Puck {
public:
	Puck(int id, Pucktype t, int h1, int h2): ID(id), ptype(t), heightReading_1(h1), heightReading_2(h2){}
	virtual ~Puck();

	int getHeightReading1() const {
		return heightReading_1;
	}

	int getHeightReading2() const {
		return heightReading_2;
	}

	int getId() const {
		return ID;
	}

	Pucktype getPtype() const {
		return ptype;
	}

private:
	int ID;
	Pucktype ptype;
	int heightReading_1;
	int heightReading_2;
};

#endif /* PUCK_H_ */
