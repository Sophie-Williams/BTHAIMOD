#include "QGreedy.h"
#include "MetaLearning.h"

QGreedy* QGreedy::_instance = NULL;

QGreedy* QGreedy::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new QGreedy();
	}
	return _instance;
}

QGreedy::QGreedy(){}

QGreedy::~QGreedy()
{
	_instance = NULL;
}

MatchState * QGreedy::select(vector<MatchState> * states){
	MatchState * max = &states->front();
	for(int i = 1; i < states->size(); i++){
		MatchState * s = &states->at(1);
		if(s->qValue > max->qValue)
			max = s;
	}

	double r = (double)rand() / RAND_MAX;

	if(r > MetaLearning::getInstance()->getLearningRate()){
		return max;
	}
	else{
		int index = rand() % states->size();
		MatchState * any = &states->at(index);
		return any;
	}
}