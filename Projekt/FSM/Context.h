#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

struct Data {
	Data(int d) :
			data1(d) {
	}
	int data1;
};

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse die einen Context aus dem State Pattern darstellt.
 * (Soll verwendet werden um den ersten Automaten zu Implementieren.)
 *
 * Example: http://prg.phoenix.uberspace.de/2016/04/19/state-machine/
 */
class Context {
private:
	struct State { //top-level state
		State() :
				data(NULL) {
		}
		virtual void signalLBBeginInterrupted() {
		}
		virtual void signalLBEndInterrupted() {
		}
		virtual void signalLBAltimetryInterrupted() {
		}
		virtual void signalLBSwitchInterrupted() {
		}
		virtual void signalLBBeginNotInterrupted() {
		}
		virtual void signalLBEndNotInterrupted() {
		}
		virtual void signalLBAltimetryNotInterrupted() {
		}
		virtual void signalLBSwitchNotInterrupted() {
		}
		virtual void signalEStop() {
		}
		virtual void signalStart() {
		}
		virtual void signalStop() {
		}
		virtual void signalReset() {
		}
		virtual void signalLBSkidInterrupted() {
		}
		virtual void signalLBSkidNotInterrupted() {
		}
		virtual void signalAltimetryCompleted() {
		}

		virtual void signalLBNextConveyor() {
		}

		Data* data; // pointer to data, which physically resides inside the context class (contextdata)
	}*statePtr;   // a pointer to current state. Used for polymorphism.

	struct StateA: public State {
		void signalLBBeginInterrupted() {
			LOG_DEBUG << "Signal Anfang!!" << std::endl;
		}

		void signalLBBeginNotInterrupted() {
			LOG_DEBUG << "Signal Anfang nicht mehr unterbrochen!!" << std::endl;
		}

		void signalLBEndInterrupted() {
			LOG_DEBUG << "Signal Hinten!!" << std::endl;
		}
		void signalLBEndNotInterrupted() {
			LOG_DEBUG << "Signal Hinten nicht mehr unterbrochen!!" << std::endl;
		}

		void signalLBAltimetryInterrupted() {
			hb.getHardware()->getAltimetry()->startAltimetry();
			LOG_DEBUG << "Signal Hoehenmessung!!" << std::endl;
		}
		void signalLBAltimetryNotInterrupted() {
			LOG_DEBUG << "Signal Hoehenmessung nicht mehr unterbrochen!!"
					<< std::endl;
		}

		void signalLBSwitchInterrupted() {
			LOG_DEBUG << "Ist Metal ? "
					<< (int) hb.getHardware()->getMT()->isItemMetal()
					<< std::endl;
			LOG_DEBUG << "Signal Weiche!!" << std::endl;
		}
		void signalLBSwitchNotInterrupted() {
			LOG_DEBUG << "Signal Weiche nicht mehr unterbrochen!!" << std::endl;
		}

		void signalEStop() {
			LOG_DEBUG << "Signal Estop!!" << std::endl;
		}
		void signalStart() {
			LOG_DEBUG << "Signal Start!!" << std::endl;
		}
		void signalStop() {
			LOG_DEBUG << "Signal Stop!!" << std::endl;
		}
		void signalReset() {
			LOG_DEBUG << "Signal Reset!!" << std::endl;
		}

		void signalLBSkidInterrupted() {
			LOG_DEBUG << "Signal Rutsche!!" << std::endl;
		}
		void signalLBSkidNotInterrupted() {
			LOG_DEBUG << "Signal Rutsche nicht mehr unterbrochen!!"
					<< std::endl;
		}

		void signalAltimetryCompleted() {
			LOG_DEBUG << "Hoehenmessung: "
					<< hb.getHardware()->getAltimetry()->getHeight() << "\n"
					<< std::endl;
			LOG_DEBUG << "Signal Hoehenmessung ist fertig.!!" << std::endl;
		}
		void signalLBNextConveyor() {
			LOG_DEBUG << "Nachricht von der anderen Anlage!!"
					<< std::endl;
		}
	};

	StateA stateMember;   //The memory for the state is part of context object
	Data contextdata;  //Data is also kept inside the context object

public:

	/**
	 *  Constructor des Contexts.
	 */
	Context();

	/**
	 *  Destructor des Contexts.
	 */
	virtual ~Context();
	
	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	bool isContextimEnzustand(){return false;}

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBBeginInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBEndInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBAltimetryInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSwitchInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBBeginNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBEndNotInterrupted();

	/**
	 *  Constructor des Adapters.
	 *
	 *  @param baseaddress Die Baseaddress die verwenden werden soll.
	 */
	void signalLBAltimetryNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSwitchNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalEStop();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalStart();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalStop();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalReset();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSkidInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBSkidNotInterrupted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalAltimetryCompleted();

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	void signalLBNextConveyor();

};

#endif /* CONTEXT_H_ */
