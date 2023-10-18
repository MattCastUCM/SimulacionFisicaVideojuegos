#include "ParticleSystem.h"

#include "Water.h"
#include "Firework.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem(const Vector3& g) : particles_(), gravity_(g), time(0) {
	/*Water* part = new Water(g, PART_LIFETIME);
	GaussianParticleGenerator* gen = new GaussianParticleGenerator("fountain", 0, 0.08);
	gen->changeModelPart(part);
	gen->changeGenerateN(1);
	generators_.push_back(gen);*/

	Firework* part = new Firework(g, PART_LIFETIME);
	GaussianParticleGenerator* gen = new GaussianParticleGenerator("fireworks", 0, 0.08);
	gen->changeModelPart(part);
	gen->changeGenerateN(1);
	generators_.push_back(gen);

	delete part;
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) delete p;
	for (auto g : generators_) delete g;
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
	if (time >= PART_SPAWN_TIME_) {
		generateParticles();
		time = 0;
	}
	
	time += t;
}

ParticleGenerator* ParticleSystem::getParticleGenerator(const std::string& name) {
	for (auto g : generators_) {
		if (g->getName() == name) return g;
	}
	return nullptr;
}




