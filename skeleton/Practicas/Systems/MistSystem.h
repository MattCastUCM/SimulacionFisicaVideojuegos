#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Mist.h"
#include "../ParticleGenerators/MistGenerator.h"
#include "../../checkMemLeaks.h"

class MistSystem : public ParticleSystem {
public:
	MistSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		Particle* part = new Mist(g, 1);

		MistGenerator* mist = new MistGenerator(0.1, 0, 0.01, 100, false, true, true, true);
		mist->changeModelPart(part);
		mist->changeGenerateN(100);
		generators_.insert({ "mist", mist });

		mist->setOrigin({ 0.0f, 0.0f, 0.0f });

		delete part;
	}
};

