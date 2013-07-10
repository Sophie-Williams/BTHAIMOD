#ifndef __HYDRAMUTA_H__
#define __HYDRAMUTA_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/** This is the Commander class for a balanced Zerg tactic
 * based on Hydralisks and Mutalisks.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class HydraMuta : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* l1;
	Squad* sc1;
	Squad* sc2;

public:
	HydraMuta();

	/** Destructor. */
	~HydraMuta();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "HydraMuta";
	}
};

#endif
