#ifndef __PROTOSSCOMMANDER_H__
#define __PROTOSSCOMMANDER_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for playing Protoss.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ProtossCommander : public Commander {

private:
	int stage;
	Constructor* builder;

	Squad* mainSquad;
	Squad* backupSquad;
	Squad* scoutSquad;

public:
	ProtossCommander();

	/** Destructor. */
	~ProtossCommander();

	/** Called each update to issue orders. */
	virtual void computeActions();
};

#endif
