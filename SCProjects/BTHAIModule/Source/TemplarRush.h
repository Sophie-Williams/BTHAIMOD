#ifndef __TEMPLARRUSH_H__
#define __TEMPLARRUSH_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for the TemplarRush Protoss tactics.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TemplarRush : public Commander {

private:
	int stage;
	Constructor* builder;

	Squad* mainSquad;
	Squad* rushSquad;
	Squad* sc1;
	Squad* sc2;
	Squad* detectorSquad;

public:
	TemplarRush();

	/** Destructor. */
	~TemplarRush();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "TemplarRush";
	}
};

#endif
