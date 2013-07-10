#ifndef __REAVERDROP_H__
#define __REAVERDROP_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for the ReaverDrop Protoss tactics.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ReaverDrop : public Commander {

private:
	int stage;
	Constructor* builder;

	Squad* mainSquad;
	Squad* drop1;
	Squad* drop2;
	Squad* sc1;
	Squad* sc2;
	
public:
	ReaverDrop();

	/** Destructor. */
	~ReaverDrop();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "ReaverDrop";
	}
};

#endif
