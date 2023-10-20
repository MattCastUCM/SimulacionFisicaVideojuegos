#pragma once

#include "../P1/Particle.h"

#include <list>
#include "ParticleGenerator.h"
#include "CircleGenerator.h"

class Firework : public Particle {
protected:
	int gen_;
	unsigned type_;
	CircleGenerator* circleGen_;


public:
	Firework(const Vector3& g, unsigned type, CircleGenerator* circleGen, double lifetime = 1.0f) :
		Particle(), type_(type), circleGen_(circleGen) 
	{
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
			//new physx::PxBoxGeometry(v.size, v.size, v.size);
		if (type_ == 0) v.color = { 235.0f / 255, 64.0f / 255, 52.0f / 255, 1.0f };
		else if (type_ == 1) v.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//...

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

	// Cuando la partícula muere, tiene que 
	~Firework() {

	}

	

};