#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Water.h"
#include "../Generators/GaussianParticleGenerator.h"

class WaterSystem : public ParticleSystem {
public:
	WaterSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		Water* w = new Water(g, 2.0);
		GaussianParticleGenerator* fountain = new GaussianParticleGenerator(0.1, 0, 0.08);
		fountain->changeModelPart(w);
		fountain->changeGenerateN(5);
		generators_.insert({ "fountain", fountain });

		delete w;
	}
};

