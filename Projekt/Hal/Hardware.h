#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"
#include "Hal/Altimetry.h"

class Hardware
{
	public:
		Hardware(HumanMachineInterface* hmi, Motor* motor,TrafficLight* tl, MeasuringTool* mt, Altimetry* am);
		~Hardware();

	    /**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
		HumanMachineInterface* getHMI();

	    /**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
		Motor* getMotor();

	    /**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
		TrafficLight* getTL();

	    /**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
		Altimetry* getAltimetry();

	    /**
	     * Oeffnet den .
	     *
	     * @return Gibt Konstant 0 zurueck.
	     */
		MeasuringTool* getMT();

	private:
		HumanMachineInterface* hmi;
		Motor* motor;
		TrafficLight* tl;
		MeasuringTool* mt;
		Altimetry* am;
};

#endif /* HARDWARE_H_ */
