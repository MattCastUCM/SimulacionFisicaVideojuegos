#pragma once

#include "../Particle.h"

class Water : public Particle {
public:
	Water(const Vector3& g, double lifetime = 1.0f) : Particle() {
		Particle::visual v;
		v.size = { 1.0f, 1.0f, 1.0f };
		v.type = Particle::geomType::geomSphere;
		
		v.color = { 81.0f / 255, 165.0f / 255, 219.0f / 255, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, 0.0f };
		p.vel = { 0.7f, 1.0f, 0.7f };
		p.vel *= 20;
		p.acc = g;
		p.mass = 1.0f / 1000;

		init(v, p, lifetime);
	}

};