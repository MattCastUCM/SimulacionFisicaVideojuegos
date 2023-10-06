#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const Vector3& g) : particles_(), gravity_(g) {
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) delete p;
}


void ParticleSystem::refresh() {
	for (auto p : particles_) {
		particles_.erase(
			remove_if(particles_.begin(), particles_.end(), [](Particle* p) {
				if (p->isAlive() /* || p->outOfBounds(bounds)*/) return false;
				else {
					delete p;
					return true;
				}
			}), particles_.end()
		);
	}
}



void ParticleSystem::update(double t) {
	// Recorre la lista de partículas para llamar a su update. 
	// El update de cada partícula actualiza el tiempo que 
	// sigue viva y actualiza si ha muerto o no
	for (auto p : particles_) {
		p->update(t);
	}
	// Elimina las partículas muertas
	refresh();


	// Recorrer generadores ( generar partículas nuevas y añadirlas a la lista)
}




