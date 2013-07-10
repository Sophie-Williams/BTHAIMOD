#include "StrategySelector.h"
#include "ReaverDrop.h"
#include "ReaverDrop1B.h"
#include "TemplarRush.h"
#include "TerranDefensive.h"
#include "TerranDefensiveFB.h"
#include "WraithHarass.h"
#include "TerranPush.h"
#include "MarineRush.h"
#include "LurkerRush.h"
#include "HydraMuta.h"
#include <fstream>
#include <sstream>
#include "Normalizer.h"
#include "QGreedy.h"

StrategySelector* StrategySelector::instance = NULL;

StrategySelector::StrategySelector()
{
	strategies.push_back(Strategy(Races::Protoss, ReaverDrop::getStrategyId()));
	strategies.push_back(Strategy(Races::Protoss, ReaverDrop1B::getStrategyId()));
	strategies.push_back(Strategy(Races::Protoss, TemplarRush::getStrategyId()));
	strategies.push_back(Strategy(Races::Terran, TerranDefensive::getStrategyId()));
	strategies.push_back(Strategy(Races::Terran, TerranDefensiveFB::getStrategyId()));
	strategies.push_back(Strategy(Races::Terran, TerranPush::getStrategyId()));
	strategies.push_back(Strategy(Races::Terran, MarineRush::getStrategyId()));
	strategies.push_back(Strategy(Races::Terran, WraithHarass::getStrategyId()));
	strategies.push_back(Strategy(Races::Zerg, LurkerRush::getStrategyId()));
	strategies.push_back(Strategy(Races::Zerg, HydraMuta::getStrategyId()));

	loadStats();

	loadLearnedMatchs();
}

StrategySelector* StrategySelector::getInstance()
{
	if (instance == NULL)
	{
		instance = new StrategySelector();
	}
	return instance;
}

StrategySelector::~StrategySelector()
{
	instance = NULL;
}

void StrategySelector::selectStrategyWithRL(){
	MatchState * selected = QGreedy::getInstance()->select(&matchs);

	currentStrategyId = selected->strategyId;
	Broodwar->printf("Strategy selected: %s (Q-Value: %d)", currentStrategyId.c_str(), selected->qValue);
	return;

	/*vector<double> * qValues = new vector<double>();
	
	double max;
	double min = max = matchs.front().qValue;

	for (int i = 0; i < matchs.size(); i++)
	{
		double q = matchs.at(i).qValue;
		if(q < min) min = q;
		if(q > max) max = q;
	}

	//TODO: Tratar Matchs vaziu!

	Normalizer * math = new Normalizer(min, max);
	for (int i = 0; i < matchs.size(); i++)
	{
		double normalized = math->normalize(matchs.at(i).qValue);
		qValues->push_back(normalized);
	}

	math->probability(qValues);

	double r = (double)rand() / RAND_MAX;
	int qIndex;
	for (int i = 0; i < qValues->size(); i++)
	{
		if(r <= qValues->at(i)){
			qIndex = i;
			break;
		}
	}
	
	currentStrategyId = matchs.at(qIndex).strategyId;
	Broodwar->printf("Strategy selected: %s (Roll: %d Prob: %d)", currentStrategyId.c_str(), r, qValues->at(qIndex));*/
}

void StrategySelector::selectStrategy()
{
	int totWon = 0;
	int totPlay = 0;
	for (int i = 0; i < (int)stats.size(); i++)
	{
		string mOwnRace = Broodwar->self()->getRace().getName();
		
		if (stats.at(i).matches())
		{
			totWon += stats.at(i).won;
			totPlay += stats.at(i).total;
		}
	}
	if (totPlay == 0) totPlay = 1; //To avoid division by zero

	//Random probability select one strategy
	bool found = false;
	int i = 0;
	while (!found)
	{
		i = rand() % (int)stats.size();
		
		//Entry matches
		if (stats.at(i).matches())
		{
			//Calculate probability for this entry.
			int chance = stats.at(i).won * 100 / stats.at(i).getTotal();
			chance = chance * totWon / totPlay;

			//Have 75% chance to try a strategy that
			//hasn't been tested much yet.
			if (stats.at(i).total <= 2) chance = 75;

			//Set a max/min so all strategies have a chance
			//to be played.
			if (chance < 15) chance = 15;
			if (chance > 85) chance = 85;

			//Make the roll!
			int roll = rand() % 100;
			if (roll <= chance)
			{
				currentStrategyId = stats.at(i).strategyId;
				Broodwar->printf("Strategy selected: %s (Roll: %d Prob: %d)", currentStrategyId.c_str(), roll, chance);
				found = true;
				return;
			}
		}		
	}
}

Commander* StrategySelector::getStrategy()
{
	int tot = 0;
	for (int i = 0; i < (int)stats.size(); i++)
	{
		if (stats.at(i).matches()) tot++;
	}
	
	if (tot > 0)
	{
		//Select a strategy among the tested
		//ones.
		selectStrategy();
		selectStrategyWithRL();
	}
	else
	{
		//No strategy has been tested for this combo.
		//Return one of the available strategies.
		if (Broodwar->self()->getRace().getID() == Races::Terran.getID()) currentStrategyId = "TerranDefensive";
		if (Broodwar->self()->getRace().getID() == Races::Protoss.getID()) currentStrategyId = "ReaverDrop1B";
		if (Broodwar->self()->getRace().getID() == Races::Zerg.getID()) currentStrategyId = "HydraMuta";
	}

	//TODO: Fix Trategy to tests
	//return new WraithHarass();

	//Get Commander for strategy
	if (currentStrategyId == "ReaverDrop") return new ReaverDrop();
	if (currentStrategyId == "ReaverDrop1B") return new ReaverDrop1B();
	if (currentStrategyId == "TemplarRush") return new TemplarRush();
	if (currentStrategyId == "TerranDefensive") return new TerranDefensive();
	if (currentStrategyId == "TerranDefensiveFB") return new TerranDefensiveFB();
	if (currentStrategyId == "TerranPush") return new TerranPush();
	if (currentStrategyId == "MarineRush") return new MarineRush();
	if (currentStrategyId == "WraithHarass") return new WraithHarass();
	if (currentStrategyId == "LurkerRush") return new LurkerRush();
	if (currentStrategyId == "HydraMuta") return new HydraMuta();

	return NULL;
}

void StrategySelector::printInfo()
{
	
}

void StrategySelector::loadStats()
{
	string filename = getFilename();

	ifstream inFile;
	inFile.open(filename.c_str());
	if (!inFile)
	{
		//No file found.
		return;
	}
	else
	{
		string line;
		char buffer[256];
		while (!inFile.eof())
		{
			inFile.getline(buffer, 256);
			if (buffer[0] != ';')
			{	
				stringstream ss;
				ss << buffer;
				line = ss.str();
				addEntry(line);
			}
		}
		inFile.close();
	}
}

void StrategySelector::loadLearnedMatchs()
{
	string filename = QLearning::getInstance()->getLearningFilename();

	ifstream inFile;
	inFile.open(filename.c_str());
	if (!inFile)
	{
		//No file found.
		return;
	}
	else
	{
		string line;
		char buffer[256];
		while (!inFile.eof())
		{
			inFile.getline(buffer, 256);
			if (buffer[0] != ';')
			{	
				stringstream ss;
				ss << buffer;
				line = ss.str();
				addLearnedEntry(line);
			}
		}
		inFile.close();
	}
}

void StrategySelector::addLearnedEntry(string line){
	if (line == "") return;

	MatchState s = MatchState();
	int i;
	string t;

	i = line.find(";");
	t = line.substr(0, i);
	s.strategyId = t;
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.ownRace = t;
	line = line.substr(i + 1, line.length());

	//i = line.find(";");
	//t = line.substr(0, i);
	//s.opponentRace = t;
	//line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	char* endptr;
	s.qValue = toDouble(t);
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.mapName = t;
	line = line.substr(i + 1, line.length());
	
	i = line.find(";");
	t = line.substr(0, i);
	s.mapHash = t;
	line = line.substr(i + 1, line.length());

	matchs.push_back(s);
}

void StrategySelector::addEntry(string line)
{
	if (line == "") return;

	StrategyStats s = StrategyStats();
	int i;
	string t;

	i = line.find(";");
	t = line.substr(0, i);
	s.strategyId = t;
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.ownRace = t;
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.opponentRace = t;
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.won = toInt(t);
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.lost = toInt(t);
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.total = toInt(t);
	line = line.substr(i + 1, line.length());

	i = line.find(";");
	t = line.substr(0, i);
	s.mapName = t;
	line = line.substr(i + 1, line.length());
	
	i = line.find(";");
	t = line.substr(0, i);
	s.mapHash = t;
	line = line.substr(i + 1, line.length());

	stats.push_back(s);
}

double StrategySelector::toDouble(string &str){
	stringstream ss(str);
	double n;
	ss >> n;
	return n;
}

int StrategySelector::toInt(string &str)
{
	stringstream ss(str);
	int n;
	ss >> n;
	return n;
}

string StrategySelector::getFilename()
{
	stringstream ss;
	ss << "bwapi-data\\AI\\BTHAI\\";
	ss << "StrategyStats.csv";
	
	return ss.str();
}

void StrategySelector::addLearnedResult(bool won){
	string opponentRace = Broodwar->enemy()->getRace().getName();
	string mapHash = Broodwar->mapHash();

	//Check if we have the match already
	for (int i = 0; i < (int)matchs.size(); i++)
	{
		//if (mapHash == matchs.at(i).mapHash && opponentRace == matchs.at(i).opponentRace && currentStrategyId == matchs.at(i).strategyId)
		if (mapHash == matchs.at(i).mapHash && currentStrategyId == matchs.at(i).strategyId)
		{
			MatchState * state = &(matchs.at(i));
			
			QLearning::getInstance()->updateQvalue(state, won ? 1 : -1);
			return;
		}
	}

	MatchState * s = &MatchState();
	s->strategyId = currentStrategyId;
	s->mapHash = mapHash;
	s->mapName = Broodwar->mapFileName();
	s->ownRace = Broodwar->self()->getRace().getName();
	//s->opponentRace = opponentRace;

	QLearning::getInstance()->updateQvalue(s, won ? 1 : -1);

	matchs.push_back(*s);
}

void StrategySelector::addResult(bool won)
{	
	string opponentRace = Broodwar->enemy()->getRace().getName();
	string mapHash = Broodwar->mapHash();

	//Check if we have the entry already
	for (int i = 0; i < (int)stats.size(); i++)
	{
		if (mapHash == stats.at(i).mapHash && opponentRace == stats.at(i).opponentRace && currentStrategyId == stats.at(i).strategyId)
		{
			stats.at(i).total++;
			if (won) stats.at(i).won++;
			else stats.at(i).lost++;
			return;
		}
	}

	StrategyStats s = StrategyStats();
	s.total++;
	if (won) s.won++;
	else s.lost++;
	s.strategyId = currentStrategyId;
	s.mapHash = mapHash;
	s.mapName = Broodwar->mapFileName();
	s.ownRace = Broodwar->self()->getRace().getName();
	s.opponentRace = opponentRace;
	stats.push_back(s);
}

void StrategySelector::saveLearnedMatchs(){
	//Fill entries in stats file for combinations that have
	//not yet been played.
	string mapHash = Broodwar->mapHash();
	string opponentRace = Broodwar->enemy()->getRace().getName();
	string ownRace = Broodwar->self()->getRace().getName();

	for (int i = 0; i < (int)strategies.size(); i++)
	{
		bool found = false;
		for (int s = 0; s < (int)stats.size(); s++)
		{
			if (strategies.at(i).strategyId == stats.at(s).strategyId && mapHash == stats.at(s).mapHash && opponentRace == stats.at(s).opponentRace)
			{
				//Matches
				found = true;
				break;
			}
		}

		if (!found)
		{
			//Only fill in the strategies for
			//the same race
			if (ownRace == strategies.at(i).race.getName())
			{
				//Add entry
				MatchState s = MatchState();
				s.mapHash = mapHash;
				s.mapName = Broodwar->mapFileName();
				//s.opponentRace = opponentRace;
				s.ownRace = strategies.at(i).race.getName();
				s.strategyId = strategies.at(i).strategyId;

				matchs.push_back(s);
			}
		}
	}

	//Save the file
	string filename = QLearning::getInstance()->getLearningFilename();

	ofstream outFile;
	outFile.open(filename.c_str());
	if (!outFile)
	{
		Broodwar->printf("Error writing to stats file!", filename.c_str());
	}
	else
	{
		for (int i = 0; i < (int)matchs.size(); i++)
		{
			stringstream s2;
			s2 << matchs.at(i).strategyId;
			s2 << ";";
			s2 << matchs.at(i).ownRace;
			s2 << ";";
			//s2 << matchs.at(i).opponentRace;
			//s2 << ";";
			s2 << matchs.at(i).qValue;
			s2 << ";";
			s2 << matchs.at(i).mapName;
			s2 << ";";
			s2 << matchs.at(i).mapHash;
			s2 << ";\n";

			outFile << s2.str();
		}
		outFile.close();
	}
}

void StrategySelector::saveStats()
{
	//Fill entries in stats file for combinations that have
	//not yet been played.
	string mapHash = Broodwar->mapHash();
	string opponentRace = Broodwar->enemy()->getRace().getName();
	string ownRace = Broodwar->self()->getRace().getName();

	for (int i = 0; i < (int)strategies.size(); i++)
	{
		bool found = false;
		for (int s = 0; s < (int)stats.size(); s++)
		{
			if (strategies.at(i).strategyId == stats.at(s).strategyId && mapHash == stats.at(s).mapHash && opponentRace == stats.at(s).opponentRace)
			{
				//Matches
				found = true;
				break;
			}
		}

		if (!found)
		{
			//Only fill in the strategies for
			//the same race
			if (ownRace == strategies.at(i).race.getName())
			{
				//Add entry
				StrategyStats s = StrategyStats();
				s.mapHash = mapHash;
				s.mapName = Broodwar->mapFileName();
				s.opponentRace = opponentRace;
				s.ownRace = strategies.at(i).race.getName();
				s.strategyId = strategies.at(i).strategyId;

				stats.push_back(s);
			}
		}
	}

	//Save the file
	string filename = getFilename();

	ofstream outFile;
	outFile.open(filename.c_str());
	if (!outFile)
	{
		Broodwar->printf("Error writing to stats file!", filename.c_str());
	}
	else
	{
		for (int i = 0; i < (int)stats.size(); i++)
		{
			stringstream s2;
			s2 << stats.at(i).strategyId;
			s2 << ";";
			s2 << stats.at(i).ownRace;
			s2 << ";";
			s2 << stats.at(i).opponentRace;
			s2 << ";";
			s2 << stats.at(i).won;
			s2 << ";";
			s2 << stats.at(i).lost;
			s2 << ";";
			s2 << stats.at(i).total;
			s2 << ";";
			s2 << stats.at(i).mapName;
			s2 << ";";
			s2 << stats.at(i).mapHash;
			s2 << ";\n";

			outFile << s2.str();
		}
		outFile.close();
	}
}
