#ifndef __QLEARNING_H__
#define __QLEARNING_H__

#include "MatchState.h"

class QLearning {

private:
	static QLearning* _instance;
	QLearning();

	double _learningRate;

public:
	/** Returns the instance of the class. */
	static QLearning* getInstance();

	/** Destructor */
	~QLearning();

	void updateQvalue(MatchState* state, double reward);

	string getLearningFilename();
};

#endif