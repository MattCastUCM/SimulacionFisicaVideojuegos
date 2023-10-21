#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Firework.h"
#include "../Generators/GaussianParticleGenerator.h"

class FireworksSystem : public ParticleSystem {
public:
	FireworksSystem (const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		CircleGenerator* fireworks = new CircleGenerator(0, 0, 0.1);
		fireworks->changeGenerateN(10);
		generators_.insert({ "fireworks", fireworks });
		fireworks->setActive(false);



		Particle* shotF = new Firework(g, 0, 3.0, fireworks);
		GaussianParticleGenerator* fireworkShooter = new GaussianParticleGenerator(2.0, 0, 0.05);
		fireworkShooter->changeModelPart(shotF);
		
		fireworkShooter->changeGenerateN(1);
		generators_.insert({ "fireworkShooter", fireworkShooter });

		shotF->setPos({ -1000, -1000, -1000 });
		delete shotF;
	}


};

