#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const Vector3& g) : particles_(), gravity_(g), time(0) {
	Particle::visual v;
	v.size = 1.0f;
	v.geometry = &physx::PxSphereGeometry(v.size);
	v.color = { 0.0f, 0.0f, 1.0f, 1.0f };
	
	Particle::physics p;
	p.damp = 0.998f;
	p.pos = { 0.0f, 0.0f, -30.0f };
	p.vel = { 1.0f, 1.0f, -1.0f };
	p.vel *= 350;
	p.acc = { 0.0f, -0.25f, 0.0f };
	p.mass = 5.4f;
	p.simSpd = 40.0f;
	Particle part(v, p);
	//particles_.push_back(part);

	ParticleGenerator* gen = new ParticleGenerator("fuente");
	gen->changeGenerateN(1);
	gen->changeModelPart(&part);
	//gen->setOrigin({ 0,0,0 });
	//gen->setVel({ 10, 10, 10 });
	//gen->setAcc(g);
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
				if (p->isAlive()) return false;
				else {
					delete p;
					return true;
				}
			}), particles_.end()
		);
	}
}
void ParticleSystem::generateParticles() {
	for (auto pg : generators_) {
		auto parts = pg->generateParticles();
		for (auto p : parts)
			particles_.push_back(p);
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
	if (time >= PART_TIME_) {
		generateParticles();
		time = 0;
	}
	

	time += t;
}




