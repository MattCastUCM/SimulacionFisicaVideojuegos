#pragma once

#include "../P1/Particle.h"

#include <list>
#include "ParticleGenerator.h"

#include <iostream>
class Firework : public Particle {
protected:
	// Type
	unsigned type_;
	std::list<std::shared_ptr<ParticleGenerator>> generators_;


public:
	Firework(const Vector3& g, double lifetime = 1.0f, Vector4 color = { 235.0f / 255, 64.0f / 255, 52.0f / 255, 1.0f }) : Particle() {
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
		//new physx::PxBoxGeometry(v.size, v.size, v.size);
		v.color = color;

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, -20.0f, -100.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		p.vel *= 75;
		p.acc = g;
		p.mass = 5.4f;
		p.simSpd = 70.0f;

		init(v, p, lifetime);
	}

	void update(double t) override {
		if (lifetime_ >= maxLifetime_ && alive_) explode();
	}

	// The firework generates more fireworks when exploding -->  they should be gathered by the System
	std::list<Particle*> explode() {
		std::cout << "BOOM\n";
		return { };
	};
	void addGenerator(ParticleGenerator* p);
	Particle* clone() const;


};