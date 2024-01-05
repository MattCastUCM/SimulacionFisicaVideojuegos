#pragma once

#include "../Practicas/Particle.h"

class Smoke : public Particle {
public:
	Smoke(float maxLifetime) : Particle() {
		Particle::visual v;
		v.size = { 0.5f, 0.5f, 0.5f };
		v.type = Particle::geomType::geomSphere;
		v.color = { 0.1f, 0.1f, 0.1f, 1.0f };

		Particle::physics p;
		p.mass = 1.0f;
		p.vel = { 0.7f, 1.0f, 0.7f };

		init(v, p, maxLifetime);
	}
};