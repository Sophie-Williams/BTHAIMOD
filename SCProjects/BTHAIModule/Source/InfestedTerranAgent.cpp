#include "InfestedTerranAgent.h"
#include "NavigationAgent.h"
#include "TargetingAgent.h"

InfestedTerranAgent::InfestedTerranAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "InfestedTerranAgent";
	//Broodwar->printf("InfestedTerranAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void InfestedTerranAgent::computeActions()
{
	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
