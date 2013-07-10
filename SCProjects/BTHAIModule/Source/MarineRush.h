#ifndef __MARINERUSH_H__
#define __MARINERUSH_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for a Terran Marine rush strategy.
 * 
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class MarineRush : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* backupSquad;
	Squad* rushSquad;
	Squad* sc1;
	Squad* sc2;
	
public:
	MarineRush();

	/** Destructor. */
	~MarineRush();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "MarineRush";
	}
};

#endif
