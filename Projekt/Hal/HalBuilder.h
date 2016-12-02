#ifndef HALBUILDER_H_
#define HALBUILDER_H_

#include "Hal/Adapter.h"
#include "Hal/Hardware.h"
#include "Hal/Motor.h"
#include "Hal/HumanMachineInterface.h"
#include "Hal/MeasuringTool.h"
#include "Hal/TrafficLight.h"
#include "Hal/Mutexo.h"


#define BASEADRESS_A 0x300 ///< Die Basisaddresse um auf den Port A zu zugreifen.
#define BASEADRESS_B 0x301 ///< Die Basisaddresse um auf den Port B zu zugreifen.
#define BASEADRESS_C 0x302 ///< Die Basisaddresse um auf den Port C zu zugreifen.
#define BASEADRESS_D 0x320 ///< Die Basisaddresse um auf die AIO zu zugreifen.

#define CONTROL_ADDRESS_0 0x303 ///< Controll Addresse um die Ports A - C zu konfigurieren als In oder Out.
#define CONTROL_BITMASK 0x8A    ///< Die benoetigte Configuration fuer die Controll Addresse.

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
         * Baut das HumanMachineInterface aus den Unterobjekten.
         *
         * @return Gibt das gebaute HumanMachineInterface zurueck.
         */
    HumanMachineInterface* buildHumanMachineInterface();

    /**
         * Baut den Motor aus den Unterobjekten.
         *
         * @return Gibt das gebaute Motor zurueck.
         */
    Motor* buildMotor();

    /**
         * Baut die TrafficLights aus den Unterobjekten.
         *
         * @return Gibt das gebaute TrafficLight zurueck.
         */
    TrafficLight* buildTrafficLights();

    /**
         * Baut das MeasuringTool aus den Unterobjekten.
         *
         * @return Gibt das gebaute MeasuringTool zurueck.
         */
    MeasuringTool* buildMeasuringTool();

    /**
         * Baut die Altimetry aus den Unterobjekten.
         *
         * @return Gibt das gebaute Altimetry zurueck.
         */
    Altimetry* buildAltimetry();
};

#endif /* HALBUILDER_H_ */
