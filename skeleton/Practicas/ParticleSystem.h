#pragma once

#include "Particle.h"
#include "ParticleGenerator.h"
#include "../checkMemLeaks.h"

#include <unordered_map>
class ParticleSystem {
protected:
	Vector3 gravity_;
	bool active_;

	std::list<Particle*> particles_;
	std::list<Particle*> remove_;
	std::unordered_map<std::string, ParticleGenerator*> generators_;
	
	int maxParts_;

	inline virtual void refresh() {
		for (auto p : particles_) {
			if (!p->isAlive()) {
				p->onDeath();
				remove_.push_back(p);
			}
		}

		for (auto it = remove_.begin(); it != remove_.end(); ) {
			particles_.remove(*it);
			delete* it;
			it = remove_.erase(it);
		}

	}

	inline virtual void generateParticles(double t) {
		// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
		for (auto pg : generators_) {
			// El update se encarga de generar las partículas
			// según el tiempo de generación de cada generador
			auto parts = pg.second->update(t);
			for (auto p : parts)
				particles_.push_back(p);
		}
	}


public:
	// Se usa -10.0f como gravedad por defecto
	ParticleSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }, int maxParts = 1000) : particles_(), gravity_(g), active_(true), maxParts_(maxParts)
	{ };
	
	virtual ~ParticleSystem() {
		for (auto p : particles_) delete p;
		for (auto g : generators_) delete g.second;
	};
	
	inline virtual void update(double t) {
		if (active_) {
			// Recorre la lista de partículas para llamar a su update. 
			// El update de cada partícula actualiza el tiempo que 
			// sigue viva y actualiza si ha muerto o no
			for (auto p : particles_) {
				p->update(t);
			}
			// Elimina las partículas muertas
			refresh();

			if(particles_.size() < maxParts_) generateParticles(t);
		}
		
	}


	inline ParticleGenerator* getParticleGenerator(const std::string& name) {
		if (generators_.find(name) != generators_.end()) return generators_[name];
		else return nullptr;
	}


	inline virtual void setActive(bool active) {
		active_ = active;
	}

	inline virtual void keyPress(unsigned char key) {
		for (auto pg : generators_) pg.second->keyPress(key);
	};
};

