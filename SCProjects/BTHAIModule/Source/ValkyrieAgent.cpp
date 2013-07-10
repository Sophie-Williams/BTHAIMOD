#include "ValkyrieAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Commander.h"
#include "TargetingAgent.h"

ValkyrieAgent::ValkyrieAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ValkyrieAgent";
	//Broodwar->printf("ValkyrieAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ValkyrieAgent::computeActions()
{
	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
