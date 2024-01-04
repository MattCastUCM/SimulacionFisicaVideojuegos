#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Water.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"
#include "../../checkMemLeaks.h"

class WaterSystem : public ParticleSystem {
public:
	WaterSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		Water* w = new Water(g, 3.0);
		GaussianParticleGenerator* fountain = new GaussianParticleGenerator(0.1, 0, 0.08, 2);
		fountain->changeModelPart(w);
		fountain->changeGenerateN(5);
		generators_.insert({ "fountain", fountain });

		fountain->setOrigin({ 0.0f, -20.0f, -100.0f });

		delete w;
	}
};

