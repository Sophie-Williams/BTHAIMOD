#ifndef __WRAITHHARASS_H__
#define __WRAITHHARASS_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for a Wraith based Terran strategy.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class WraithHarass : public Commander {

private:
	int stage;
	
	Squad* mainSQ;
	Squad* groundSQ2;
	Squad* airSQ;
	Squad* sc1;
	Squad* sc2;
	
public:
	WraithHarass();

	/** Destructor. */
	~WraithHarass();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "WraithHarass";
	}
};

#endif
