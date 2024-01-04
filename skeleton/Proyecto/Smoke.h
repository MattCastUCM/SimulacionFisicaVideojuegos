#pragma once

#include "../Practicas/Particle.h"

class Smoke : public Particle {
public:
	Smoke(double lifetime = 1.0f) : Particle() {
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);

		v.color = { 81.0f / 255, 165.0f / 255, 219.0f / 255, 1.0f };

		Particle::physics p;
		p.mass = 1.0f / 1000;

		init(v, p, lifetime);
	}

	inline virtual Particle* clone() {
		Particle::visual v;
		v.size = vis_.size;
		v.geometry = vis_.geometry;
		v.color = vis_.color;
		v.clone = true;

		return new Particle(v, phys_, maxLifetime_);
	};
};