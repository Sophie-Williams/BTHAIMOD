#include "ReaverDrop1B.h"
#include "BuildplanEntry.h"
#include "ExplorationSquad.h"
#include "ExplorationManager.h"
#include "RushSquad.h"
#include "AgentManager.h"
#include "BuildingPlacer.h"

ReaverDrop1B::ReaverDrop1B()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;

	builder = Constructor::getInstance();

	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 8));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Forge, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Assimilator, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Cybernetics_Core, 15));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Robotics_Facility, 20));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Robotics_Support_Bay, 28));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Photon_Cannon, 28));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 31));

	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->setBuildup(true);
	mainSquad->setRequired(true);
	squads.push_back(mainSquad);

	drop1 = new Squad(2, Squad::OFFENSIVE, "DropSquad1", 10);
	drop1->addSetup(UnitTypes::Protoss_Shuttle, 1);
	drop1->addSetup(UnitTypes::Protoss_Reaver, 2);
	drop1->setBuildup(false);
	drop1->setRequired(true);
	
	drop2 = new Squad(3, Squad::OFFENSIVE, "DropSquad2", 10);
	drop2->addSetup(UnitTypes::Protoss_Shuttle, 1);
	drop2->addSetup(UnitTypes::Protoss_Reaver, 2);
	drop2->setBuildup(false);
	drop2->setRequired(true);
	
	sc1 = new RushSquad(4, "ScoutingSquad2", 11);
	sc1->setRequired(false);

	sc2 = new ExplorationSquad(5, "ScoutingSQ2", 10);
	sc2->setRequired(false);
	
	noWorkers = 16;
	noWorkersPerRefinery = 3;
}

ReaverDrop1B::~ReaverDrop1B()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void ReaverDrop1B::computeActions()
{
	computeActionsBase();

	noWorkers = 12 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Protoss_Nexus) + 3 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Protoss_Assimilator);
	if (noWorkers > 30) noWorkers = 30;

	int cSupply = Broodwar->self()->supplyUsed() / 2;
	int min = Broodwar->self()->minerals();
	int gas = Broodwar->self()->gas();

	if (cSupply >= 15 && stage == 0 && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Cybernetics_Core) > 0)
	{
		mainSquad->addSetup(UnitTypes::Protoss_Zealot, 6);
		mainSquad->addSetup(UnitTypes::Protoss_Dragoon, 6);
		stage++;
	}
	if (cSupply >= 24 && stage == 1)
	{
		int no = 1;
		if (Broodwar->getStartLocations().size() > 2) no++;

		sc1->addSetup(UnitTypes::Protoss_Probe, 1);
		sc1->setPriority(1);
		sc1->setActivePriority(1000);
		squads.push_back(sc1);

		stage++;
	}
	if (cSupply >= 26 && stage == 2)
	{
		squads.push_back(drop1);
		squads.push_back(drop2);

		stage++;
	}
	if (stage == 3 && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Robotics_Support_Bay) > 0)
	{	
		mainSquad->addSetup(UnitTypes::Protoss_Dragoon, 10);
		mainSquad->addSetup(UnitTypes::Protoss_Zealot, 4);
		mainSquad->setBuildup(false);

		buildplan.push_back(BuildplanEntry(UpgradeTypes::Singularity_Charge, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Reaver_Capacity, cSupply));
	
		stage++;
	}
	if (stage == 4 && min > 400 && gas > 400)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Observatory, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Robotics_Facility, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		
		mainSquad->addSetup(UnitTypes::Protoss_Observer, 1);

		sc2->addSetup(UnitTypes::Protoss_Observer, 1);
		squads.push_back(sc2);
		
		stage++;
	}
	if (stage == 5 && min > 400)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Nexus, 15));
		stage++;
	}
	if (stage == 6 && min > 400 && gas > 300 && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Observer) > 0)
	{
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Protoss_Ground_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Protoss_Ground_Weapons, cSupply));
		stage++;
	}
	if (stage == 7 && min > 1000)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		stage++;
	}
}
