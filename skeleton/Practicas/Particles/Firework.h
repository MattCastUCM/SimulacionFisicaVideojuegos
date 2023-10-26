#pragma once

#include "../Particle.h"

#include <list>
#include "../ParticleGenerator.h"

class Firework : public Particle {
protected:
	unsigned type_;		// del 0 al 3, siendo el 3 los que van a desaparecer al final
	std::vector<std::shared_ptr<ParticleGenerator>> generators_;


public:
	Firework(const Vector3& g, unsigned type, double lifetime = 1.0f) : Particle(), type_(type) {
		Particle::visual v;
		v.size = 1.0f;
		v.geometry = new physx::PxSphereGeometry(v.size);
			//new physx::PxBoxGeometry(v.size, v.size, v.size);

		if (type_ == 0) 
			v.color = { 235.0f / 255, 64.0f / 255, 52.0f / 255, 1.0f };
		else if (type_ == 1)
			v.color = { 66.0f / 255, 135.0f / 255, 245.0f / 255, 1.0f };
		else if (type_ == 2)
			v.color = { 24.0f / 255, 168.0f / 255, 79.0f / 255, 1.0f };
		// último fuego
		else v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, 0.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		p.vel *= 75;
		p.acc = g;
		p.mass = 5.4f;
		p.simSpd = 95.0f;

		init(v, p, lifetime);
	}


	// Explode
	inline void onDeath() {
		if (type_ < 3) {
			// Por si no hay tantos generadores como tipos de fuego
			int safeIndex = type_ % generators_.size();

			// Ajusta las velocidades
			if (generators_[safeIndex]->getName() == "circleGen") {
				phys_.vel = { 1.0f, 1.0f, 1.0f };
				phys_.vel *= 200.0f;
				phys_.simSpd = 30.0f;
				simulatePhys();
			}
			else if (generators_[safeIndex]->getName() == "sphereGen") {
				phys_.vel = { 1.0f, 1.0f, 1.0f };
				phys_.vel *= 75;
				phys_.simSpd = 100.0f;
				simulatePhys();
			}


			auto model = clone();
			model->type_ = 3;

			generators_[safeIndex]->setActive(true);
			generators_[safeIndex]->changeModelPart(model);

			delete model;
		}
	}


	// Se clona con un tipo aleatorio del 0 al 2 (el 3 es el último)
	inline Firework* clone() {
		unsigned rndType = 3;
		if(type_ != 3) rndType = rand() % 3;

		auto fw = new Firework(phys_.acc, rndType, maxLifetime_);

		// Hay que cambiar manualmente la posición y velocidad
		// porque la constructora de Firework no los recoge
		fw->phys_.pos = tr_->p;
		fw->setSimSpd(phys_.simSpd);
		fw->addGenerators(generators_);

		return fw;
	}


	// Añade un generador al vector de generadores
	inline void addGenerator(ParticleGenerator* p) {
		generators_.push_back((std::shared_ptr<ParticleGenerator>) p);
		p->setActive(false);
	}
	// "Copia" el vector de generadores
	inline void addGenerators(std::vector<std::shared_ptr<ParticleGenerator>> g) {
		generators_ = g;
	}
};