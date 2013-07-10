#include "ZergCommander.h"
#include "BuildplanEntry.h"
#include "AgentManager.h"
#include "ExplorationSquad.h"

ZergCommander::ZergCommander()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;
	
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Spawning_Pool, 4));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Hatchery, 5));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Extractor, 7));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Creep_Colony, 7));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Creep_Colony, 7));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Spire, 8));
	buildplan.push_back(BuildplanEntry(UnitTypes::Zerg_Evolution_Chamber, 20));

	buildplan.push_back(BuildplanEntry(UpgradeTypes::Zerg_Flyer_Attacks, 40));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Zerg_Flyer_Carapace, 42));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Zerg_Flyer_Attacks, 50));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Zerg_Flyer_Carapace, 52));


	mainGroundSquad = new Squad(1, Squad::OFFENSIVE, "GroundSquad", 10);
	mainGroundSquad->setRequired(true);
	mainGroundSquad->setBuildup(true);
	squads.push_back(mainGroundSquad);
	
	mainAirSquad = new Squad(2, Squad::OFFENSIVE, "AirSquad", 10);
	mainAirSquad->setRequired(true);
	mainAirSquad->setBuildup(true);
	squads.push_back(mainAirSquad);
	
	backupSquad = new Squad(3, Squad::OFFENSIVE, "BackupSquad", 11);
	backupSquad->setRequired(false);
	backupSquad->setBuildup(true);
	squads.push_back(backupSquad);

	scoutSquad = new ExplorationSquad(4, "ScoutingSquad", 10);
	scoutSquad->setRequired(false);
	scoutSquad->setBuildup(true);
	squads.push_back(scoutSquad);

	noWorkers = 5;
	noWorkersPerRefinery = 3;
}

ZergCommander::~ZergCommander()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void ZergCommander::computeActions()
{
	computeActionsBase();

	noWorkers = AgentManager::getInstance()->countNoBases() * 5 + AgentManager::getInstance()->countNoUnits(UnitTypes::Zerg_Extractor) * 3;

	int cSupply = Broodwar->self()->supplyUsed() / 2;

	if (cSupply >= 13 && stage == 0)
	{
		mainAirSquad->addSetup(UnitTypes::Zerg_Mutalisk, 15);
		mainAirSquad->addSetup(UnitTypes::Zerg_Overlord, 1);
		mainAirSquad->setBuildup(false);

		stage++;
	}
	if (AgentManager::getInstance()->hasBuilding(UnitTypes::Zerg_Lair) && stage == 1)
	{
		mainGroundSquad->addSetup(UnitTypes::Zerg_Zergling, 20);
		stage++;
	}
	if (cSupply >= 23 && stage == 2)
	{
		mainGroundSquad->addSetup(UnitTypes::Zerg_Zergling, 10);
		mainGroundSquad->setBuildup(false);

		stage++;
	}
	if (cSupply >= 40 && stage == 3)
	{
		backupSquad->addSetup(UnitTypes::Zerg_Mutalisk, 12);
		backupSquad->setBuildup(false);

		scoutSquad->addSetup(UnitTypes::Zerg_Overlord, 1);
		scoutSquad->setBuildup(false);

		stage++;
	}
}
