#include "MarineAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Commander.h"
#include "Squad.h"
#include "TargetingAgent.h"

MarineAgent::MarineAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "MarineAgent";
	//Broodwar->printf("MarineAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void MarineAgent::computeActions()
{
	if (!unit->isLoaded())
	{
		Squad* sq = Commander::getInstance()->getSquad(squadID);
		if (sq != NULL)
		{
			if (sq->isBunkerDefend())
			{
				vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
				for (int i = 0; i < (int)agents.size(); i++)
				{
					if (agents.at(i)->isAlive() && agents.at(i)->isOfType(UnitTypes::Terran_Bunker))
				{
						if (agents.at(i)->getUnit()->getLoadedUnits().size() < 4)
				{
							unit->rightClick(agents.at(i)->getUnit());
							return;
						}
					}
				}
			}
		}
	}

	if (Broodwar->self()->hasResearched(TechTypes::Stim_Packs))
	{
		if (!unit->isStimmed() && unit->getHitPoints() >= 20)
		{
			if (unit->isAttacking())
			{
				unit->useTech(TechTypes::Stim_Packs);
				//Broodwar->printf("[%d] Using stim packs", unitID);
				return;
			}
		}
	}

	defensive = false;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}
