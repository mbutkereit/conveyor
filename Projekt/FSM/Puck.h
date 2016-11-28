/*
 * Puck.h
 *
 *  Created on: 21.11.2016
 *      Author: abs949
 */

#ifndef PUCK_H_
#define PUCK_H_
#define NOT_DEFINED -1

	enum Puckmaterial
	{
		METAL, PLASTIC, TYPE404PM
	};

	enum Puckdrillhole
	{
		DRILL_HOLE_UPSIDE, NO_DRILL_HOLE, TYPE404PH
	};

class Puck {
public:
	Puck(int id): ID(id), puckmaterial(TYPE404PM), puckdrillhole(TYPE404PH), heightReading_1(NOT_DEFINED), heightReading_2(NOT_DEFINED){}
	virtual ~Puck(){}

	int getHeightReading1() const {
		return heightReading_1;
	}

	void setHeightReading1(int heightReading1) {
		heightReading_1 = heightReading1;
	}

	int getHeightReading2() const {
		return heightReading_2;
	}

	void setHeightReading2(int heightReading2) {
		heightReading_2 = heightReading2;
	}

	int getId() const {
		return ID;
	}

	void setId(int id) {
			ID = id;
	}

	Puckdrillhole getPuckdrillhole() const {
		return puckdrillhole;
	}

	void setPuckdrillhole(Puckdrillhole puckdrillhole) {
		this->puckdrillhole = puckdrillhole;
	}

	Puckmaterial getPuckmaterial() const {
		return puckmaterial;
	}

	void setPuckmaterial(Puckmaterial puckmaterial) {
		this->puckmaterial = puckmaterial;
	}

private:
	int ID;
	Puckmaterial puckmaterial;
	Puckdrillhole puckdrillhole;
	int heightReading_1;
	int heightReading_2;
};

#endif /* PUCK_H_ */
