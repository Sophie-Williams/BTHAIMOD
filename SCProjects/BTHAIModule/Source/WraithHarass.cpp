#include "WraithHarass.h"
#include "BuildplanEntry.h"
#include "AgentManager.h"
#include "ExplorationSquad.h"
#include "RushSquad.h"
#include "BuildingPlacer.h"
#include "ExplorationManager.h"

WraithHarass::WraithHarass()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;
	
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 6));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 8));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 20));
	buildplan.push_back(BuildplanEntry(TechTypes::Tank_Siege_Mode, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 23));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, 25));
	buildplan.push_back(BuildplanEntry(TechTypes::Cloaking_Field, 26));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 30));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 31));

	mainSQ = new Squad(1, Squad::OFFENSIVE, "MainSquad", 8);
	mainSQ->addSetup(UnitTypes::Terran_Marine, 14);
	mainSQ->setRequired(true);
	mainSQ->setBuildup(true);
	squads.push_back(mainSQ);

	groundSQ2 = new Squad(2, Squad::OFFENSIVE, "GroundSQ2", 9);
	groundSQ2->setRequired(false);
	groundSQ2->setBuildup(false);
	squads.push_back(groundSQ2);

	airSQ = new Squad(3, Squad::OFFENSIVE, "AirSQ1", 9);
	airSQ->addSetup(UnitTypes::Terran_Wraith, 5);
	airSQ->setRequired(false);
	airSQ->setBuildup(false);

	sc1 = new RushSquad(4, "ScoutingSquad1", 8);
	sc1->setRequired(false);
	
	sc2 = new ExplorationSquad(5, "ScoutingSquad", 8);
	sc2->setRequired(false);
	sc2->setActivePriority(9);
	squads.push_back(sc2);

	noWorkers = 12;
	noWorkersPerRefinery = 2;
}

WraithHarass::~WraithHarass()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void WraithHarass::computeActions()
{
	computeActionsBase();

	noWorkers = 12 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Command_Center) + 2 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Refinery);
	if (noWorkers > 30) noWorkers = 30;

	int cSupply = Broodwar->self()->supplyUsed() / 2;
	int min = Broodwar->self()->minerals();
	int gas = Broodwar->self()->gas();

	if (stage == 0 && cSupply >= 20)
	{
		mainSQ->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 4);
		mainSQ->addSetup(UnitTypes::Terran_SCV, 1);
		mainSQ->addSetup(UnitTypes::Terran_Medic, 4);
		
		stage++;
	}
	if (stage == 1 && Broodwar->self()->hasResearched(TechTypes::Cloaking_Field))
	{	
		mainSQ->addSetup(UnitTypes::Terran_Wraith, 5);
		mainSQ->setBuildup(false);

		squads.push_back(airSQ);

		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, cSupply+2));

		sc1->addSetup(UnitTypes::Terran_SCV, 1);
		sc1->setPriority(1);
		sc1->setActivePriority(1000);
		squads.push_back(sc1);

		stage++;
	}
	if (stage == 2 && min > 500)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, cSupply));

		stage++;
	}
	if (stage == 3 && AgentManager::getInstance()->countNoBases() >= 2)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, cSupply));
		
		stage++;
	}
	if (stage == 4 && min > 200 && gas > 150)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Armory, cSupply));

		groundSQ2->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 2);
		groundSQ2->addSetup(UnitTypes::Terran_Goliath, 6);
		groundSQ2->addSetup(UnitTypes::Terran_Firebat, 6);
		groundSQ2->addSetup(UnitTypes::Terran_Medic, 3);
		groundSQ2->setBuildup(false);
		
		stage++;
	}
	if (stage == 5 && min > 250 && gas > 100)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Science_Facility, cSupply));
		
		stage++;
	}
	if (stage == 6 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Facility) > 0)
	{
		mainSQ->addSetup(UnitTypes::Terran_Science_Vessel, 1);

		sc2->addSetup(UnitTypes::Terran_Wraith, 1);
		sc2->setBuildup(false);

		stage++;
	}
	if (stage == 7 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Vessel) > 0)
	{
		if (Broodwar->enemy()->getRace().getID() == Races::Protoss.getID())
		{
			buildplan.push_back(BuildplanEntry(TechTypes::EMP_Shockwave, cSupply));
			groundSQ2->addSetup(UnitTypes::Terran_Science_Vessel, 1);
		}
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Ship_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Ship_Plating, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Ship_Weapons, cSupply));
		
		stage++;
	}
}
