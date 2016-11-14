#ifndef ADAPTER_H_
#define ADAPTER_H_

#include <stdint.h>
#include <Hal/Mutexo.h>
#include <hw/inout.h>
#include <stdint.h>

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Adapter(Ports) zugriff zu Managen.
 */
class Adapter
{
public:

    /**
     *  Constructor des Adapters.
     *
     *  @param baseaddress Die Baseaddress die verwenden werden soll.
     */
    Adapter(uint16_t);

    /**
     * Destructor des Adapters.
     */
    ~Adapter();

    /**
     * Uebernimmt die Bits aus der Bitmask in die Baseaddress.
     * (Adaptiv, die Bits die schon gesetzt waren bleiben bestehen)
     *
     * @param bitmask Eine Bitmask die gesetzt werden soll.
     */
    void setBitMask(uint8_t bitmask);

    /**
     * Entfernt eine Bitmask von der Baseaddress
     *
     * @param bitmask Eine Bitmask die entfernt werden soll.
     */
    void removeBitMask(uint8_t bitmask);

    /**
     * Checkt ob die Uebergebene Bitmask in der Baseadress vorhanden ist.
     *
     * @return Gibt 0 bei nicht Enthalten und ein Wert > 0 bei Enthalten zurueck.
     */
    uint8_t checkBitMask(uint8_t bitmask);

    /**
     * Setzt den Bitmask Wert an die Speicheradresse Baseadress + Offset.
     * (Vorherige werte werden einfach ueberschrieben.)
     *
     * @param bitmask Eine Bitmask die uebernommen werden soll.
     * @param offset  Ein Offset um die Baseadress zu erweitern.
     */
    void forceBitmask(uint8_t bitmask,uint16_t offset);

    /**
     * Lade ein Wert aus einer Speicheradresse.
     *
     * @param offset Ein offset fuer die Baseaddress
     *
     * @return Gibt den Wert aus der Baseaddress + Offset zurueck.
     */
    uint8_t getValuefromAddress(uint8_t offset);



private:
    Mutexo* synchronizer; ///< Mutex um den Zugriff auf die Speicheradressen ab zu sichern.
    uint16_t baseaddress; ///< Baseadress auf die Zugegriffen werden soll.
};

#endif /* ADAPTER_H_ */
