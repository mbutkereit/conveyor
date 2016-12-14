#ifndef CONTEXTI_H_
#define CONTEXTI_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse die einen Context aus dem State Pattern darstellt.
 * (Soll verwendet werden um den ersten Automaten zu Implementieren.)
 *
 * Example: http://prg.phoenix.uberspace.de/2016/04/19/state-machine/
 */
class ContextI {
public:

	 virtual ~ContextI(){};
	/**
	*
	*return: gibt true zurück wenn der Context den Enzustand erreicht hat und false wenn Context noch nicht in einem Enzustand ist.
	*/
	virtual bool isContextimEnzustand(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBBeginInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBEndInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBAltimetryInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBSwitchInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBBeginNotInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBEndNotInterrupted(void)=0;

	/**
	 *  Constructor des Adapters.
	 *
	 *  @param baseaddress Die Baseaddress die verwenden werden soll.
	 */
	virtual void signalLBAltimetryNotInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBSwitchNotInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalEStop(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalStart(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalStop(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalReset(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBSkidInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBSkidNotInterrupted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalAltimetryCompleted(void)=0;

	/**
	 * @todo Ausstehende implementierung Dokumentieren.
	 */
	virtual void signalLBNextConveyor(void)=0;

    /**
     * @todo Ausstehende implementierung Dokumentieren.
     */
    virtual void signalTimerTick(void)=0;
};

#endif /* CONTEXTI_H_ */
