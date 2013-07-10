#include "MutaliskAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Squad.h"
#include "Commander.h"
#include "TargetingAgent.h"

MutaliskAgent::MutaliskAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "MutaliskAgent";
	defensive = false;
	offensiveFrame = Broodwar->getFrameCount();
	//Broodwar->printf("MutaliskAgent created (%s)", unit->getType().getName().c_str());
	
	//Broodwar->printf("[%d] Mutalisk created", Broodwar->getFrameCount());

	goal = TilePosition(-1, -1);
}

void MutaliskAgent::computeActions()
{
	if (AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Greater_Spire) > 0)
	{
		Squad* sq = Commander::getInstance()->getSquad(squadID);
		if (sq != NULL)
		{
			if (sq->morphsTo().getID() == UnitTypes::Zerg_Devourer.getID())
			{
				if (enemyUnitsVisible())
				{
					if (Broodwar->canMake(unit, UnitTypes::Zerg_Devourer))
					{
						unit->morph(UnitTypes::Zerg_Devourer);
						return;
					}
				}
			}

			if (sq->morphsTo().getID() == UnitTypes::Zerg_Guardian.getID())
			{
				if (enemyUnitsVisible())
				{
					if (Broodwar->canMake(unit, UnitTypes::Zerg_Guardian))
					{
						unit->morph(UnitTypes::Zerg_Guardian);
						return;
					}
				}
			}
		}
	}

	defensive = false;
	if (TargetingAgent::getNoAttackers(this) > 1) defensive = true;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}
