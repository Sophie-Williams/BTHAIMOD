#include "ScourgeAgent.h"
#include "NavigationAgent.h"
#include "TargetingAgent.h"

ScourgeAgent::ScourgeAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ScourgeAgent";
	//Broodwar->printf("ScourgeAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ScourgeAgent::computeActions()
{
	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
