/*#include "Normalizer.h"

Normalizer::Normalizer(double min, double max)
{
	_base = 0;
	_super = 1;

	_min = min;
	_max = max;

	_mod = _base - min;
	_ratio = _super / (max + _mod);
}

Normalizer::Normalizer(double min, double max, double base, double super)
{
	_base = base;
	_super = super;

	_min = min;
	_max = max;

	_mod = _base - min;
	_ratio = _super / (max + _mod);
}

double Normalizer::normalize(double value)
{
	return value * _ratio;
}

void Normalizer::probability(vector<double> * vectors){
	double sum = 0;
	for(int i = 0; i < vectors->size(); i++){
		sum += vectors->at(i);
	}

	Normalizer * aux = new Normalizer(0, sum);

	for(int i = 0; i < vectors->size(); i++){
		double at = vectors->at(i);
		vectors->erase(vectors->begin() + i);
		double normalized = aux->normalize(at);
		vectors->insert(vectors->begin() + i, normalized);
	}
}*/