#include "QLearning.h"
#include "MetaLearning.h"
#include "Logger.h"
#include <sstream>

QLearning* QLearning::_instance = NULL;

QLearning::QLearning()
{
	_learningRate = 0.5;
}

QLearning* QLearning::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new QLearning();
	}
	return _instance;
}

QLearning::~QLearning()
{
	_instance = NULL;
}

void QLearning::updateQvalue(MatchState* state, double reward){
	_learningRate = MetaLearning::getInstance()->calculateLearningRate(state, reward);
	state->qValue = state->qValue + _learningRate * reward;

	Logger::getInstance()->Log(state->qValue, Logger::getInstance()->Count("LearningRate"), state->strategyId);
}

string QLearning::getLearningFilename()
{
	stringstream ss;
	ss << "bwapi-data\\AI\\BTHAI\\";
	ss << "LearningStats.csv";
	
	return ss.str();
}