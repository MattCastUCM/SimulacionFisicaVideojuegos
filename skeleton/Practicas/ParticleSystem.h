#pragma once

#include "Particle.h"
#include "ParticleGenerator.h"
#include "ParticleForceRegistry.h"

#include <unordered_map>

class ParticleSystem {
protected:
	Vector3 gravity_;

	std::list<Particle*> particles_;
	std::unordered_map<std::string, ParticleGenerator*> generators_;

	ParticleForceRegistry* partForceReg_;


	virtual inline void refresh() {
		for (auto it = particles_.begin(); it != particles_.end(); ) {
			if (!(*it)->isAlive()) {
				(*it)->onDeath();
				partForceReg_->deleteParticleRegistry(*it);
				delete* it;
				it = particles_.erase(it);
			}
			else ++it;
		}
	}

	virtual inline void generateParticles() {
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
	ParticleSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : particles_(), gravity_(g), partForceReg_(nullptr) { };
	
	virtual ~ParticleSystem() {
		for (auto p : particles_) delete p;
		for (auto g : generators_) delete g.second;
		if(partForceReg_ != nullptr) delete partForceReg_;
	};
	
	inline virtual void update(double t) {
		if(partForceReg_ != nullptr) partForceReg_->updateForces(t);
		
		// Recorre la lista de partículas para llamar a su update. 
		// El update de cada partícula actualiza el tiempo que 
		// sigue viva y actualiza si ha muerto o no
		for (auto p : particles_) {
			p->update(t);
		}
		// Elimina las partículas muertas
		refresh();

		generateParticles();
	}


	inline ParticleGenerator* getParticleGenerator(const std::string& name) {
		if (generators_.find(name) != generators_.end()) return generators_[name];
		else return nullptr;
	}


};

