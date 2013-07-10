#ifndef __QGREEDY_H__
#define __QGREEDY_H__

#include <vector>
#include "MatchState.h"

using namespace std;

class QGreedy {

private:
	static QGreedy* _instance;
	QGreedy();

public:
	/** Returns the instance of the class. */
	static QGreedy* getInstance();

	/** Destructor */
	~QGreedy();

	MatchState * select(vector<MatchState> * states);
};

#endif