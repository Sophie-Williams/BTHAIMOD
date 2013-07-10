#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <BWAPI.h>

using namespace std;

class Logger {

private:
	static Logger* _instance;
	Logger();

	string _commonFilename;
	string addPath(string filename);
	string readFile(string filename);
	void writeFile(string content, string filename);
public:
	/** Returns the instance of the class. */
	static Logger* getInstance();

	/** Destructor */
	~Logger();

	int Count(string filename);

	/** Logs */
	void Log(string msg);

	void Log(string msg, string filename);

	void Log(double value, string filename);

	void Log(double value, double when, string filename);
};

#endif