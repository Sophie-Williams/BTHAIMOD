#include "Logger.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

Logger* Logger::_instance = NULL;

Logger* Logger::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new Logger();
	}
	return _instance;
}

Logger::Logger(){
	_commonFilename = "Log";
}

Logger::~Logger()
{
	_instance = NULL;
}

string Logger::addPath(string filename)
{
	stringstream ss;
	ss << "bwapi-data\\AI\\BTHAI\\Logs\\";
	ss << filename;
	ss << ".csv";
	
	return ss.str();
}

string Logger::readFile(string filename){
	string uri = addPath(filename);

	stringstream out;
	
	ifstream inFile;
	inFile.open(uri.c_str());
	if (inFile)
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

				if(line != ""){
					out << "\n";
					out << line;
				}
			}
		}
		inFile.close();
	}

	return out.str();
}

void Logger::writeFile(string content, string filename){
	string uri = addPath(filename);

	ofstream outFile;
	outFile.open(uri.c_str());
	if (outFile)
	{
		outFile << content;
		
		outFile.close();
	}
}

int Logger::Count(string filename){
	std::ifstream inFile(addPath(filename).c_str()); 
	return std::count(std::istreambuf_iterator<char>(inFile), 
             std::istreambuf_iterator<char>(), '\n');
}

void Logger::Log(string msg){
	Log(msg, _commonFilename);
}

void Logger::Log(string msg, string filename){
	string currentContent = readFile(filename);

	stringstream s2;
	s2 << currentContent;
	s2 << "\n";
	s2 << msg;
	s2 << ";";

	writeFile(s2.str(), filename);
}

void Logger::Log(double value, string filename){
	string currentContent = readFile(filename);

	stringstream s2;
	s2 << currentContent;
	s2 << "\n";
	s2 << value;
	s2 << ";";

	writeFile(s2.str(), filename);
}

void Logger::Log(double value, double when, string filename){
	string currentContent = readFile(filename);

	stringstream s2;
	s2 << currentContent;
	s2 << "\n";
	s2 << value;
	s2 << ";";
	s2 << when;
	s2 << ";";

	writeFile(s2.str(), filename);
}