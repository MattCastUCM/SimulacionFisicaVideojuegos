#pragma once

#include "../Particle.h"

class Mist : public Particle {
public:
	Mist(const Vector3& g, double lifetime = 1.0f) : Particle() {
		Particle::visual v;
		v.size = 0.05f;
		v.geometry = new physx::PxSphereGeometry(v.size);
		v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, 0.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		p.acc = g;
		p.mass = 1.0f / 1000;

		init(v, p, lifetime);
	}

};