#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Firework.h"
#include "../Generators/GaussianParticleGenerator.h"

class FireworksSystem : public ParticleSystem {
public:
	FireworksSystem (const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		fireworks_ = new CircleGenerator(0, 0, 0.1);
		fireworks_->changeGenerateN(10);
		generators_.insert({ "fireworks", fireworks_ });


		Particle* shotF = new Firework(g, 0, 3.0, fireworks_);
		GaussianParticleGenerator* fireworkShooter = new GaussianParticleGenerator(5.0, 0, 0.08);
		fireworkShooter->changeModelPart(shotF);
		
		fireworkShooter->changeGenerateN(1);
		generators_.insert({ "fireworkShooter", fireworkShooter });

		
		delete shotF;
	}

	

private:
	CircleGenerator* fireworks_;

};

