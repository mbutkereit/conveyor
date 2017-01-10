/*
 * Puck.h
 *
 *  Created on: 21.11.2016
 *      Author: abs949
 */

#ifndef PUCK_H_
#define PUCK_H_
#define NOT_DEFINED -1

enum PuckType {
	DRILL_HOLE_UPSIDE,
	DRILL_HOLE_UPSIDE_METAL,
	DRILL_HOLE_UPSIDE_PLASTIC,
	NO_DRILL_HOLE,
	TYPE404PT,
};

class Puck {
public:
	Puck(int id) :
			ID(id), heightReading_1(NOT_DEFINED), heightReading_2(NOT_DEFINED), pucktype(
					TYPE404PT) {
		std::cerr << "Konstruktor id ist" << id <<"\n";
	}
	virtual ~Puck() {
	}

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

	int getId(){
		if(ID == 0){
			std::cerr << "IST 0";
		}
		return ID;
	}

	void setId(int id) {
		ID = id;
	}

	PuckType getPuckType() const {
		return pucktype;
	}

	void setPuckType(PuckType pucktype) {
		this->pucktype = pucktype;
	}

	void operator=(Puck puck){
		this->ID =puck.ID;
		this->heightReading_1 =puck.heightReading_1;
		this->heightReading_2 = puck.heightReading_2;
		this->pucktype =puck.pucktype;
	}

private:
	int ID;
	int heightReading_1;
	int heightReading_2;
	PuckType pucktype;
};

#endif /* PUCK_H_ */
