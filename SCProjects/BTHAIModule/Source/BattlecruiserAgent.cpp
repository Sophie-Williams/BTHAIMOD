#include "BattlecruiserAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

BattlecruiserAgent::BattlecruiserAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "BattlecruiserAgent";
	//Broodwar->printf("BattlecruiserAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
	lastUseFrame = Broodwar->getFrameCount();
}

void BattlecruiserAgent::computeActions()
{
	if (Broodwar->getFrameCount() - lastUseFrame >= 80)
	{
		TechType gun = TechTypes::Yamato_Gun;
		if (Broodwar->self()->hasResearched(gun))
		{
			if (unit->getEnergy() >= gun.energyUsed())
			{
				int range = gun.getWeapon().maxRange();
				
				Unit* target = TargetingAgent::findHighprioTarget(this, range, true, true);
				if (target != NULL)
				{
					//Broodwar->printf("[%d] Yamato Gun used on %s", unitID, target->getType().getName().c_str());
					unit->useTech(gun, target);
					lastUseFrame = Broodwar->getFrameCount();
					return;
				}
			}
		}
	}

	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}
