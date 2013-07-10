#ifndef __METALEARNING_H__
#define __METALEARNING_H__

#include "MatchState.h"

class MetaLearning {

private:
	static MetaLearning* _instance;
	MetaLearning();

	double _baseLearningRate;
	double _metaLearningRate;

	string getMetaLearningFilename();
	
	void saveLearningRate(double value);
	double updateLearningRate(double s, double reward);

public:
	/** Returns the instance of the class. */
	static MetaLearning* getInstance();

	/** Destructor */
	~MetaLearning();

	double getLearningRate();

	double calculateLearningRate(MatchState* state, double reward);
};

#endif