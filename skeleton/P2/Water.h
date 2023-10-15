#pragma once

#include "../P1/Particle.h"
#include <iostream>

class Water : public Particle {
public:
	Water(const Vector3& g, double lifetime = 1.0f) : Particle()
	{
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
		//new physx::PxBoxGeometry(v.size, v.size, v.size);
		v.color = { 81.0f / 255, 165.0f / 255, 219.0f / 255, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, -100.0f };
		p.vel = { 0.4f, 1.0f, -0.4f };
		p.vel *= 300;
		p.acc = g;
		p.mass = 5.4f;
		p.simSpd = 100.0f;

		init(v, p, lifetime);
	}

};