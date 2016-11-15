#ifndef HALBUILDER_H_
#define HALBUILDER_H_

#include "Hal/Adapter.h"
#include "Hal/Hardware.h"
#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"
#include "Hal/Mutexo.h"


#define BASEADRESS_A 0x300 ///<
#define BASEADRESS_B 0x301 ///<
#define BASEADRESS_C 0x302 ///<
#define BASEADRESS_D 0x320 ///<

#define CONTROL_ADDRESS_0 0x303 ///<
#define CONTROL_BITMASK 0x8A    ///<

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um ein Hardware Objekt zu erstellen (Singelton like).
 */
class HalBuilder
{
public:

    /**
     *  Constructor des HalBuilders.
     */
    HalBuilder();

    /**
     *  Destructor des HalBuilders
     */
    virtual ~HalBuilder();

    /**
     * Getter fuer die Hardware.
     *
     * @return Gibt das Singelton der fertigen Hardware zurueck.
     */
    Hardware* getHardware();
private:
    Adapter* adapterA; ///< Adapter fuer den Port A der DIO.
    Adapter* adapterB; ///< Adapter fuer den Port D der DIO.
    Adapter* adapterC; ///< Adapter fuer den Port C der DIO.
    Adapter* adapterD; ///< Adapter fuer den Port A der AIO.
    static Hardware* instance_; ///< Singleton Speicher.
    Mutexo mutex; ///< Mutex um die instanzierung des Singelton sicher zustellen.

    /**
         * Baut die Hardware aus den Unterobjekten.
         *
         * @return Gibt die gebaute Hardware zurueck.
         */
    Hardware* buildHardware();

    /**
         * Oeffnet den .
         *
         * @return Gibt das gebaute HumanMachineInterface zurueck.
         */
    HumanMachineInterface* buildHumanMachineInterface();

    /**
         * Oeffnet den .
         *
         * @return Gibt das gebaute Motor zurueck.
         */
    Motor* buildMotor();

    /**
         * Oeffnet den .
         *
         * @return Gibt das gebaute TrafficLight zurueck.
         */
    TrafficLight* buildTrafficLights();

    /**
         * Oeffnet den .
         *
         * @return Gibt das gebaute MeasuringTool zurueck.
         */
    MeasuringTool* buildMeasuringTool();

    /**
         * Oeffnet den .
         *
         * @return Gibt das gebaute Altimetry zurueck.
         */
    Altimetry* buildAltimetry();
};

#endif /* HALBUILDER_H_ */
