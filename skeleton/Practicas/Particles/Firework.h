#pragma once

#include "../Particle.h"
#include "../ParticleGenerator.h"

class Firework : public Particle {
protected:
	unsigned type_;		// del 0 al número de generadores que hay, siendo el último el que van a desaparecer al final
	int nGens_;
	std::vector<ParticleGenerator*> generators_;


public:
	Firework(const Vector3& g, unsigned type, int nGens = 0, double lifeTime = 1.0f, float size = 1.0f) : Particle(), type_(type), nGens_(nGens) {
		Particle::visual v;
		v.size = { size, size, size};
		v.type = Particle::geomType::geomSphere;
			

		if (type_ < nGens_) {
			if (type_ == 0)
				v.color = { 235.0f / 255, 64.0f / 255, 52.0f / 255, 1.0f };
			else if (type_ == 1)
				v.color = { 66.0f / 255, 135.0f / 255, 245.0f / 255, 1.0f };
			else /*if (type_ == 2)*/
				v.color = { 24.0f / 255, 168.0f / 255, 79.0f / 255, 1.0f };
			// ...
		}
		// último fuego
		else v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		Particle::physics p;
		p.damp = 0.998f;
		p.pos = { 0.0f, 0.0f, 0.0f };
		p.vel = { 1.0f, 1.0f, 1.0f };
		p.vel *= 40;
		p.acc = g;
		p.mass = 5.4f;

		init(v, p, lifeTime);
	}


	// Explode
	inline void onDeath() {
		if (type_ < nGens_) {
			// Por si no hay tantos generadores como tipos de fuego
			int safeIndex = type_ % nGens_;

			type_ = nGens_;
			auto model = clone();

			// Si el tipo es igual al número de generadores y hay al menos 1,
			// ya es del tipo que se genera al morir
			if (type_ == nGens_ && nGens_ > 0) {
				auto name = generators_[safeIndex]->getName().substr(0, 9);
				if (name == "circleGen") {
					model->phys_.vel = { 1.0f, 0.5f, 1.0f };
					model->phys_.vel *= 10;
					model->vel_ = model->phys_.vel;
				}
			}

			generators_[safeIndex]->setActive(true);
			generators_[safeIndex]->changeModelPart(model);

			delete model;
		}
	}


	// Se clona con un tipo aleatorio del 0 al número de generadores de la lista - 1
	inline Firework* clone() {
		unsigned rndType = nGens_;
		if(type_ != nGens_) rndType = rand() % nGens_;

		auto fw = new Firework(phys_.acc, rndType, nGens_, maxLifeTime_);
		fw->addGenerators(generators_);

		// Hay que cambiar manualmente la posición y velocidad
		// porque la constructora de Firework no los recoge
		fw->phys_.pos = tr_->p;


		return fw;
	}


	// Añade un generador al vector de generadores
	inline void addGenerator(ParticleGenerator* p) {
		generators_.push_back(p);
		p->setActive(false);
		nGens_++;
	}
	// "Copia" el vector de generadores
	inline void addGenerators(std::vector<ParticleGenerator*> g) {
		generators_ = g;
		nGens_ = g.size();
	}
};