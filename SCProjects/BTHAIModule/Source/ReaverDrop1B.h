#ifndef __REAVERDROP1B_H__
#define __REAVERDROP1b_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for the ReaverDrop 1 base Protoss tactics.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ReaverDrop1B : public Commander {

private:
	int stage;
	Constructor* builder;

	Squad* mainSquad;
	Squad* drop1;
	Squad* drop2;
	Squad* sc1;
	Squad* sc2;
	
public:
	ReaverDrop1B();

	/** Destructor. */
	~ReaverDrop1B();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "ReaverDrop1B";
	}
};

#endif
