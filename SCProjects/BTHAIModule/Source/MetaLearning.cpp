#include "MetaLearning.h"
#include "Logger.h"
#include <sstream>
#include <fstream>
#include <sstream>

MetaLearning* MetaLearning::_instance = NULL;

MetaLearning::MetaLearning()
{
	_baseLearningRate = 0.5;
	_metaLearningRate = 0.05;
}

MetaLearning* MetaLearning::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new MetaLearning();
	}
	return _instance;
}

MetaLearning::~MetaLearning()
{
	_instance = NULL;
}

double MetaLearning::updateLearningRate(double s, double reward){
	double alpha = s - (_metaLearningRate * reward);

	if(alpha < 0)
		alpha = 0;
	if(alpha > 1)
		alpha = 1;

	return alpha;
}

double MetaLearning::calculateLearningRate(MatchState* state, double reward){
	double rate = getLearningRate();
	
	rate = updateLearningRate(rate, reward);

	saveLearningRate(rate);

	Logger::getInstance()->Log(rate, "LearningRate");

	return rate;
}

void MetaLearning::saveLearningRate(double value){
	string filename = getMetaLearningFilename();

	ofstream outFile;
	outFile.open(filename.c_str());
	if (!outFile)
	{
		//TODO: tratar falta de arquivo!
		//Broodwar->printf("Error writing to stats file!", filename.c_str());
	}
	else
	{
		stringstream s2;
		s2 << value;
		s2 << ";";
		outFile << s2.str();
		outFile.close();
	}
}

double MetaLearning::getLearningRate(){
	string filename = getMetaLearningFilename();
	ifstream inFile;
	inFile.open(filename.c_str());
	if (!inFile)
	{
		//No file found.
		return _baseLearningRate;
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

				int i = line.find(";");
				string t = line.substr(0, i);
				
				stringstream sToDoub(t);
				double learningRate;
				sToDoub >> learningRate;
				return learningRate;
			}
		}
		inFile.close();
	}
	return _baseLearningRate;
}


string MetaLearning::getMetaLearningFilename()
{
	stringstream ss;
	ss << "bwapi-data\\AI\\BTHAI\\";
	ss << "MetaLearningStats.csv";
	
	return ss.str();
}