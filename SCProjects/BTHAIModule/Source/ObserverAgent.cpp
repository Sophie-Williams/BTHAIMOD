#include "ObserverAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"

ObserverAgent::ObserverAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ObserverAgent";
	//Broodwar->printf("ObserverAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ObserverAgent::computeActions()
{
	bool defensive = true;
	if (unit->isCloaked() && !unit->isDetected()) defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
}
