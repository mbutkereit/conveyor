#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <iostream>
#include "Logger/Logger.h"
#include "Hal/HalBuilder.h"

extern HalBuilder hb; ///<

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class Context {

public:
	Context();
    virtual ~Context();

    /**
         *  Constructor des Adapters.
         *
         *  @param baseaddress Die Baseaddress die verwenden werden soll.
         */
	void signalLBBeginInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBEndInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBAltimetryInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBSwitchInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBBeginNotInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBEndNotInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBAltimetryNotInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBSwitchNotInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalEStop();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalStart();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalStop();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalReset();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBSkidInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalLBSkidNotInterrupted();

	/**
	     *  Constructor des Adapters.
	     *
	     *  @param baseaddress Die Baseaddress die verwenden werden soll.
	     */
	void signalAltimetryCompleted();
};

#endif /* CONTEXT_H_ */
