#include "VultureAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Commander.h"
#include "TargetingAgent.h"

VultureAgent::VultureAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "VultureAgent";
	//Broodwar->printf("VultureAgent created (%s)", unit->getType().getName().c_str());
	mineDropFrame = 0;

	goal = TilePosition(-1, -1);
}

void VultureAgent::computeActions()
{
	int eCnt = enemyGroundAttackingUnitsWithinRange(unit->getTilePosition(), 320);
	if (eCnt > 0)
	{
		int framesSinceDrop = Broodwar->getFrameCount() - mineDropFrame;
		if (unit->getSpiderMineCount() > 0 && framesSinceDrop >= 100)
		{
			//Broodwar->printf("[%d] dropped spider mine", unitID);
			unit->useTech(TechTypes::Spider_Mines, unit->getPosition());
			mineDropFrame = Broodwar->getFrameCount();
			return;
		}
	}

	defensive = false;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}
