#ifndef INFO_MESSAGE_H_
#define INFO_MESSAGE_H_

#include "Serializer/Serializer.h"
#include "Logger.h"



#define RUTSCHE_VOLL 0x01
#define QUITIERUNGS_BIT 0x04
#define BAND_FREI 0x08
#define ESTOP_BIT 0x02
#define LB_INTERRUPTED 0x10

#define BAND 1
/**
 * Diese Klasse enth�lt ein Paket, das von jeder Anlage mit Informationen gef�llt werden kann.
 */
class InfoMessage {
public:

	/**
	 * Konstruktor
	 */
	InfoMessage();

	/*
	 * Destruktor
	 */
	~InfoMessage();

	/*
	 *Gibt die einzige existierende Instanz zur�ck.
	 *@return die Instanz
	 */
	static InfoMessage* getInfoMessage() {
		static InfoMessage instances;
		return &instances;
	}

	/**
	 * Gibt an, ob die Rutsche1 voll ist.
	 * @return Gibt true zur�ck, wenn die Rutsche voll ist und false wenn die Rutsche leer ist.
	 */
	bool istBand1RutscheVoll();

	/**
	 * Gibt an, ob die Rutsche2 voll ist.
	 * @return Gibt true zur�ck, wenn die Rutsche voll ist und false wenn die Rutsche leer ist.
	 */
	bool istBand2RutscheVoll();

	/**
	 * Gibt an, ob das Band2 frei ist.
	 * @return Gibt true zur�ck, wenn das Band freil ist und false wenn das Band leer ist.
	 */
	bool istBand2Frei();

	/**
	 * Die Methode aktualisiert die Bereiche im Paket die nicht zu seiner Anlage geh�ren.
	 * Der Bereich, der zur Anlage geh�rt, wird mit neuen Informationen �berschrieben.
	 * @param info_package_without_ch* ein Zeiger auf das empfangene Paket
	 */
	void update(struct info_package_without_ch* infoUpdate);

	/**
	 * Die Methode setzt Rutsche1 auf voll.
	 */
	void setBand1RutscheVoll();

	/**
	 * Die Methode setzt Rutsche1 auf leer.
	 */
	void setBand1RutscheLeer();

	/**
	 * Die Methode setzt Rutsche2 auf voll.
	 */
	void setBand2RutscheVoll();

	/**
	 * Die Methode setzt Rutsche2 auf leer.
	 */
	void setBand2RutscheLeer();

	/**
	 * Die Methode setzt Band2 auf frei.
	 */
	void setBand2Frei();

	/**
	 * Die Methode setzt Band2 auf besetzt.
	 */
	void setBand2NichtFrei();

	/**
	 * Die Methode setzt das Bit LBInterrupt, um der vorherigen Anlage zu signalisieren, dass die vordere Lichtschranke unterbrochen wurde.
	 */
	void setLBinterruptedBit();

	/**
	 * Die Methode setzt das Bit LBInterrupt zur�ck.
	 */
	void resetisinterruptedBit();

	/**
	 * Die Methode setzt das Quittierungsbit an der entsprechenden Anlage zur�ck.
	 * 0=Es ist Quittiert und 1=Es ist nicht Quittiert.
	 */
	void setQuittierung();

	/**
	 * Gibt nur den Inhalt des Paketes �ber den Logger aus.
	 */
	void InhaltdesPaketesausgeben();

	/**
	 * Der Estop an der Anlage wird gesetzt.Solange Estop an irgendeiner Anlage gesetzt ist,
	 *  bekommt jede Anlage das Ereignis Estop.
	 */
	void setESTOP();

	/**
	 * Informiert �ber den Status von Estop.
	 * @return Gibt true zur�ck, wenn an irgendeiner Anlage Estop ger�ckt wird und setzt alle Quittierungsbits auf 1.
	 * Gibt false zur�ck, wenn kein Estop gedr�ckt wurde.
	 */
	bool isESTOPGedrueckt();

	/**
	 * Der Estop an der Anlage wird zur�ckgesetzt.
	 */
	void removeESTOP();

	/**
	 *  Informiert �ber den Status der Quittierung.
	 *  @return Gibt true zur�ck, wenn �berall quittiert wurde und false wenn nicht �berall quittiert wurde.
	 */
	bool wurdeUeberallQuitiert();

	/**
	 * Guckt, ob die vordere Lichtschranke beim Nachfolger unterbrochen war.
	 * @return true, wenn unterbrochen und false wenn nicht unterbrochen
	 */
	bool isLbNextConveyorInterrupted();

	/**
	 *Getter
	 *@return das Paket
	 */
	struct info_package* getMessage();

private:


	/**
	 * Die Struktur enth�lt das Paket mit den Informationen.
	 */
	struct info_package info;
};

#endif /* SERIALIZER_H_ */
