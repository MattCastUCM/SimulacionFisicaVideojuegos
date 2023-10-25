#pragma once

#include "../Particle.h"

#include <list>
#include "../ParticleGenerator.h"
#include "../Generators/CircleGenerator.h"
#include <time.h>       /* time */


#include <iostream>
class Firework : public Particle {
protected:
	unsigned type_;		// del 0 al 4, siendo el 4 los que van a desaparecer al final
	CircleGenerator* gen_;

public:
	Firework(const Vector3& g, unsigned type, double lifetime = 1.0f, CircleGenerator* gen = nullptr) : Particle(), type_(type), gen_(gen) {
		srand(time(NULL));


		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
			//new physx::PxBoxGeometry(v.size, v.size, v.size);

		if (type_ == 0) 
			v.color = { 235.0f / 255, 64.0f / 255, 52.0f / 255, 1.0f };
		else if (type_ == 1)
			v.color = { 66.0f / 255, 135.0f / 255, 52.0f / 255, 1.0f };
		else if (type_ == 2)
			v.color = { 24.0f / 255, 168.0f / 255, 79.0f / 255, 1.0f };
		// último fuego
		else v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, -40.0f, -100.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		p.vel *= 75;
		p.acc = g;
		p.mass = 5.4f;
		p.simSpd = 95.0f;

		init(v, p, lifetime);
	}

	// Explode
	void onDeath() {
		if (type_ < 3) {
			gen_->setActive(true);
			auto model = clone();
			model->type_ = 3;

			// if(type == 0), ...

			gen_->changeModelPart(model);
			delete model;
		}
	}


	inline Firework* clone() {
		unsigned rndType = 3;
		if(type_ != 3) rndType = rand() % 3;

		auto fw = new Firework(phys_.acc, rndType, maxLifetime_, gen_);
		fw->phys_.pos = tr_->p;
		return fw;
	}

};