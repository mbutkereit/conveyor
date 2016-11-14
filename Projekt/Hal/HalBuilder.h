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
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class HalBuilder
{
public:
    HalBuilder();
    virtual ~HalBuilder();

    /**
     * Oeffnet den .
     *
     * @return Gibt Konstant 0 zurueck.
     */
    Hardware* getHardware();
private:
    Adapter* adapterA; ///<
    Adapter* adapterB; ///<
    Adapter* adapterC; ///<
    Adapter* adapterD; ///<
    static Hardware* instance_; ///<
    Mutexo mutex; ///<

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    Hardware* buildHardware();

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    HumanMachineInterface* buildHumanMachineInterface();

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    Motor* buildMotor();

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    TrafficLight* buildTrafficLights();

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    MeasuringTool* buildMeasuringTool();

    /**
         * Oeffnet den .
         *
         * @return Gibt Konstant 0 zurueck.
         */
    Altimetry* buildAltimetry();
};

#endif /* HALBUILDER_H_ */
