#include "TemplarRush.h"
#include "BuildplanEntry.h"
#include "ExplorationSquad.h"
#include "ExplorationManager.h"
#include "RushSquad.h"
#include "AgentManager.h"

TemplarRush::TemplarRush()
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
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Citadel_of_Adun, 20));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Templar_Archives, 24));
	
	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->setBuildup(true);
	mainSquad->setRequired(true);
	squads.push_back(mainSquad);

	rushSquad = new RushSquad(2, "TemplarRush", 11);
	rushSquad->setActivePriority(1000);
	rushSquad->setRequired(false);
	rushSquad->setBuildup(true);
	squads.push_back(rushSquad);

	sc1 = new RushSquad(3, "ScoutingSquad1", 11);
	sc1->setRequired(false);
	
	sc2 = new ExplorationSquad(4, "ScoutingSquad2", 11);
	sc2->setRequired(false);
	
	noWorkers = 16;
	noWorkersPerRefinery = 3;
}

TemplarRush::~TemplarRush()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void TemplarRush::computeActions()
{
	computeActionsBase();

	int cSupply = Broodwar->self()->supplyUsed() / 2;

	if (cSupply >= 15 && stage == 0 && AgentManager::getInstance()->countNoUnits(UnitTypes::Protoss_Cybernetics_Core) > 0)
	{
		mainSquad->addSetup(UnitTypes::Protoss_Zealot, 8);
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

		//If more than two-player map, use an additional
		//scouting squad.
		if (no > 1)
		{
			sc2->addSetup(UnitTypes::Protoss_Probe, 1);
			sc2->setPriority(1);
			sc2->setActivePriority(1000);
			squads.push_back(sc2);
		}
		stage++;
	}
	if (cSupply >= 30 && stage == 2)
	{
		rushSquad->addSetup(UnitTypes::Protoss_Dark_Templar, 4);
		rushSquad->setBuildup(false);
		rushSquad->setPriority(2);

		stage++;
	}
	if (stage == 3 && rushSquad->isActive())
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Photon_Cannon, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Photon_Cannon, cSupply));

		mainSquad->addSetup(UnitTypes::Protoss_Dragoon, 15);
		mainSquad->addSetup(UnitTypes::Protoss_Zealot, 5);
		mainSquad->addSetup(UnitTypes::Protoss_High_Templar, 4);
		mainSquad->setBuildup(false);

		buildplan.push_back(BuildplanEntry(TechTypes::Psionic_Storm, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Singularity_Charge, cSupply));

		stage++;
	}
	
	int min = Broodwar->self()->minerals();
	int gas = Broodwar->self()->gas();

	if (stage == 4 && min > 400)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		stage++;
	}
	if (stage == 5 && min > 400 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Protoss_Gateway) >= 3)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Robotics_Facility, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Observatory, cSupply));

		detectorSquad = new Squad(5, Squad::OFFENSIVE, "DetectorSquad", 10);
		detectorSquad->setRequired(false);
		detectorSquad->addSetup(UnitTypes::Protoss_Observer, 2);
		squads.push_back(detectorSquad);

		buildplan.push_back(BuildplanEntry(UpgradeTypes::Protoss_Ground_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Protoss_Plasma_Shields, cSupply));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Protoss_Ground_Weapons, cSupply));
		stage++;
	}
	if (stage == 6 && min > 550)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Nexus, cSupply));
		noWorkers += 8;
		stage++;
	}
	if (stage == 7 && min > 1200)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Nexus, cSupply));
		stage++;
	}
}
