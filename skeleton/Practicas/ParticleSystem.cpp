#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const Vector3& g) : particles_(), gravity_(g) { }

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) delete p;
	for (auto g : generators_) delete g.second;
}


void ParticleSystem::refresh() {
	for (auto p : particles_) {
		particles_.erase(
			remove_if(particles_.begin(), particles_.end(), [](Particle* p) {
				if (p->isAlive() /* || p->outOfBounds */) return false;
				else {
					delete p;
					return true;
				}
			}), particles_.end()
		);
	}
}


void ParticleSystem::update(double t) {
	// Recorre la lista de part�culas para llamar a su update. 
	// El update de cada part�cula actualiza el tiempo que 
	// sigue viva y actualiza si ha muerto o no
	for (auto p : particles_) {
		p->update(t);
	}
	// Elimina las part�culas muertas
	refresh();

	// Recorrer generadores (generar part�culas nuevas y a�adirlas a la lista)
	for (auto pg : generators_) {
		// El update se encarga de generar las part�culas
		// seg�n el tiempo de generaci�n de cada generador
		auto parts = pg.second->update(t);
		for (auto p : parts)
			particles_.push_back(p);
	}
}


ParticleGenerator* ParticleSystem::getParticleGenerator(const std::string& name) {
	if (generators_.find(name) != generators_.end()) return generators_[name];
	else return nullptr;
}




