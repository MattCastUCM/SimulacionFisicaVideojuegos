#pragma once

#include "ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 desvEstpos_, desvEstvel_;
	double desvEstT_;
	std::normal_distribution<double> d { 0, 1 };

public:
	GaussianParticleGenerator(const std::string& name) : ParticleGenerator(name) { };

	virtual std::list<Particle*> generateParticles() override;
	//{
	//	std::list<Particle*> generated;
	//	Particle* p;
	//	for (int i = 0; i < generateN_; i++) {
	//		p = modelPart_->clone();
	//		generated.push_back(p);
	//	}

	//	return generated;
	//};

	
};