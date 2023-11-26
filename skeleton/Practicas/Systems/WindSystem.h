#pragma once

#include "../ParticleForceSystem.h"
//#include "../ForceGenerators/WindForceGenerator.h"
#include "../ForceGenerators/TornadoForceGenerator.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"

class WindSystem : public ParticleForceSystem {
protected:
	WindForceGenerator* wind_, *tornado_;
	bool windActive_;

public:
	WindSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f });

	void keyPress(unsigned char key);
	void generateParticles(double t) override;

private:
	void activateWind();
	 void activateTornado();
};

