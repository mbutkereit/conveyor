#ifndef WORKPIECE_MESSAGE_H_
#define WORKPIECE_MESSAGE_H_

#include <stdint.h>
#include "Serializer/Serializer.h"
#include  <queue>
#include "Hal/HalBuilder.h"

/**
 * Die Klassen enthält Methoden zum Versand von Werkstückinformationen zur nächsten Anlage.
 */
class WorkpieceMessage
{
public:

	/**
	 * Singleton
	 *@return gibt die einzige Instanz, dieser Klasse zurück.
	 */
    static WorkpieceMessage* getWorkpieceMessage()
    {
        static WorkpieceMessage instances;
        return &instances;
    }

    /**
     * Sendet Informationen über ein Werkstück an die nächste Anlage.
     * @param hohehnmesswert_1 der Höhenmesswert von Anlage 1
     * @param hohehnmesswert_2 der Höhenmesswert von Anlage 2
     * @param typ der Typ des Werkstückes
     * @param id die Id des Werkstückes
     */
    void send(uint16_t hohehnmesswert_1 ,uint16_t hohehnmesswert_2,uint8_t typ ,uint16_t id );

    /**
     * interne Methode um ein Obejekt auf die Queue zu schreiben
     * @param element das Element
     */
    void enqueue(struct workpiece_package_without_ch  element);

    /**
     * Gibt die Informationen über ein Werkstück zurück
     * @return Null wenn kein Werkstück vorhanden ist, sonst die Informationen.
     */
    struct workpiece_package_without_ch getWorkpieceInfo();

private:



    /**
	 * Destruktor
	 */
	~WorkpieceMessage();


    /**
     * Konstruktor
     */
    WorkpieceMessage();

   /*
    * Enthält die empfangenen Informationen, bis sie benötigt werden.
    */
   std::queue<struct workpiece_package_without_ch> queue;

};

#endif /* SERIALIZER_H_ */
