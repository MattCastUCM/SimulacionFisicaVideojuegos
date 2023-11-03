#pragma once

#include "Particle.h"
#include "ParticleGenerator.h"

#include <unordered_map>

class ParticleSystem {
protected:
	Vector3 gravity_;

	std::vector<Particle*> particles_;
	std::unordered_map<std::string, ParticleGenerator*> generators_;

	inline void refresh() {
		for (auto p : particles_) {
			particles_.erase(
				remove_if(particles_.begin(), particles_.end(), [](Particle* p) {
					if (p->isAlive() /* || p->outOfBounds */) return false;
					else {
						p->onDeath();
						delete p;
						return true;
					}
				}), particles_.end()
			);
		}
	}

public:
	// Se usa -10.0f como gravedad por defecto
	ParticleSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : particles_(), gravity_(g) { };
	virtual ~ParticleSystem() {
		for (auto p : particles_) delete p;
		for (auto g : generators_) delete g.second;
	};
	
	inline virtual void update(double t) {
		// Recorre la lista de partículas para llamar a su update. 
		// El update de cada partícula actualiza el tiempo que 
		// sigue viva y actualiza si ha muerto o no
		for (auto p : particles_) {
			p->update(t);
		}
		// Elimina las partículas muertas
		refresh();

		// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
		for (auto pg : generators_) {
			// El update se encarga de generar las partículas
			// según el tiempo de generación de cada generador
			auto parts = pg.second->update(t);
			for (auto p : parts)
				particles_.push_back(p);
		}
	}


	inline ParticleGenerator* getParticleGenerator(const std::string& name) {
		if (generators_.find(name) != generators_.end()) return generators_[name];
		else return nullptr;
	}


};

