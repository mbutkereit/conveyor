#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"

extern HalBuilder hb; ///< Der HalBuilder um sicher und zentral auf die Hardware zuzugreifen.

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse die einen Context aus dem State Pattern darstellt.
 * (Soll verwendet werden um den ersten Automaten zu Implementieren.)
 */
class Context {

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
};

#endif /* CONTEXT_H_ */
