#include "UltraliskAgent.h"
#include "NavigationAgent.h"
#include "TargetingAgent.h"

UltraliskAgent::UltraliskAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "UltraliskAgent";
	//Broodwar->printf("UltraliskAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void UltraliskAgent::computeActions()
{
	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
