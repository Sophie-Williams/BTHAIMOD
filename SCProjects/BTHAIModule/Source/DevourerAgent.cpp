#include "DevourerAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

DevourerAgent::DevourerAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "DevourerAgent";
	//Broodwar->printf("DevourerAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void DevourerAgent::computeActions()
{
	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
