#ifndef WORKPIECE_MESSAGE_H_
#define WORKPIECE_MESSAGE_H_

#include <stdint.h>
#include "Serializer/Serializer.h"
#include  <queue>
#include "Hal/HalBuilder.h"

/**
 * Die Klassen enth�lt Methoden zum Versand von Werkst�ckinformationen zur n�chsten Anlage.
 */
class WorkpieceMessage
{
public:

	/**
	 * Singleton
	 *@return gibt die einzige Instanz, dieser Klasse zur�ck.
	 */
    static WorkpieceMessage* getWorkpieceMessage()
    {
        static WorkpieceMessage instances;
        return &instances;
    }

    /**
     * Sendet Informationen �ber ein Werkst�ck an die n�chste Anlage.
     * @param hohehnmesswert_1 der H�henmesswert von Anlage 1
     * @param hohehnmesswert_2 der H�henmesswert von Anlage 2
     * @param typ der Typ des Werkst�ckes
     * @param id die Id des Werkst�ckes
     */
    void send(uint16_t hohehnmesswert_1 ,uint16_t hohehnmesswert_2,uint8_t typ ,uint16_t id );

    /**
     * interne Methode um ein Obejekt auf die Queue zu schreiben
     * @param element das Element
     */
    void enqueue(struct workpiece_package_without_ch  element);

    /**
     * Gibt die Informationen �ber ein Werkst�ck zur�ck
     * @return Null wenn kein Werkst�ck vorhanden ist, sonst die Informationen.
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
    * Enth�lt die empfangenen Informationen, bis sie ben�tigt werden.
    */
   std::queue<struct workpiece_package_without_ch> queue;

};

#endif /* SERIALIZER_H_ */
