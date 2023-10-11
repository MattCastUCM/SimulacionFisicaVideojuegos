#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const Vector3& g) : particles_(), gravity_(g), time(0) {
	Particle::visual v;
	v.size = 1.0f;
	v.geometry = &physx::PxBoxGeometry(2.0f, 2.0f, 2.0f);
	v.color = { 0.0f, 0.0f, 1.0f, 1.0f };
	
	Particle::physics p;
	p.damp = 0.998f;
	p.pos = { 0, 0, 0 };
	p.vel = { 0,0,0 };
	p.acc = { 0, 0, 0};
	p.mass = 5.0f;
	p.simSpd = 50.0f;
	Particle* part = new Particle(v, p);


	ParticleGenerator* gen = new ParticleGenerator("fuente");
	gen->changeModelPart(part);
	gen->setOrigin({ 0,0,0 });
	gen->setVel({ 0, 10, 0 });
	gen->changeLifetime(PART_TIME_);
	generators_.push_back(gen);
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

	if(time >= PART_TIME_){
		// Recorrer generadores ( generar partículas nuevas y añadirlas a la lista)
		for (auto pg : generators_) {
			auto parts = pg->generateParticles();

			for (auto p : parts)
				particles_.push_back(p);
		}

		time = 0;
	}
	

	time += t;
}




