#pragma once

#include "Particle.h"
#include "ParticleForceRegistry.h"

class ForceSystem {
protected:

	std::vector<Particle*> particles_;
	ParticleForceRegistry* partForceReg_;

	inline void refresh() {
		for (auto p : particles_) {
			if (!p->isAlive()) { partForceReg_->deleteParticleRegistry(p); }

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
	ForceSystem() : particles_() {
		partForceReg_ = new ParticleForceRegistry();
	};

	virtual ~ForceSystem() {
		for (auto p : particles_) delete p;
		delete partForceReg_;
	};
	
	inline virtual void update(double t) {
		partForceReg_->updateForces(t);


		// Recorre la lista de partículas para llamar a su update. 
		// El update de cada partícula actualiza el tiempo que 
		// sigue viva y actualiza si ha muerto o no
		for (auto p : particles_) {
			p->update(t);
		}
		// Elimina las partículas muertas
		refresh();

	}


};

