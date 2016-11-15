#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"
#include "Hal/Altimetry.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class Hardware
{
	public:

    /**
     *  Constructor der Hardware.
     *
     *  @param hmi HumanMachineInterface um zentral drauf zugreifen zu koennen.
     *  @param motor Motor um zentral drauf zugreifen zu koennen.
     *  @param tl TrafficLight um zentral drauf zugreifen zu koennen.
     *  @param mt MeasuringTool um zentral drauf zugreifen zu koennen.
     *  @param am Altimetry um zentral drauf zugreifen zu koennen.
     */
		Hardware(HumanMachineInterface* hmi, Motor* motor,TrafficLight* tl, MeasuringTool* mt, Altimetry* am);

		/**
         *  Destructor der Hardware.
         */
		~Hardware();

	    /**
	     * Getter fuer HumanMachineInterface.
	     *
	     * @return Gibt eine Addresse zu einem HumanMachineInterface Objekt zurueck.
	     */
		HumanMachineInterface* getHMI();

	    /**
	     * Getter fuer Motor.
	     *
         * @return Gibt eine Addresse zu einem HumanMachineInterface Objekt zurueck.

	     */
		Motor* getMotor();

	    /**
	     * Getter fuer TrafficLight. .
	     *
         * @return Gibt eine Addresse zu einem HumanMachineInterface Objekt zurueck.
	     */
		TrafficLight* getTL();

	    /**
	     * Getter fuer Altimetry.
	     *
         * @return Gibt eine Addresse zu einem HumanMachineInterface Objekt zurueck.
	     */
		Altimetry* getAltimetry();

	    /**
	     * Getter fuer MeasuringTool
	     *
         * @return Gibt eine Addresse zu einem HumanMachineInterface Objekt zurueck.
	     */
		MeasuringTool* getMT();

	private:
		HumanMachineInterface* hmi; ///< Das HumanMachineInterface fuer die Hardware.
		Motor* motor; ///< Das Motor fuer die Hardware.
		TrafficLight* tl; ///<  Das HumanMachineInterface fuer die Hardware.
		MeasuringTool* mt; ///< Das MeasuringTool fuer die Hardware.
		Altimetry* am; ///< Das Altimetry fuer die Hardware.
};

#endif /* HARDWARE_H_ */
