#include "WindSystem.h"

WindSystem::WindSystem(const Vector3& g) : ParticleForceSystem(g), windActive_(false) {
	partForceReg_ = new ParticleForceRegistry();

	wind_ = new WindForceGenerator({ 100, 0, 0 }, 0.5f, 0.0/*, { -100, 0, -200 }, { -20, 50, 100 }*/);
	tornado_ = new TornadoForceGenerator({ 0, 0, -100 }, { 0.1, 0, 0 }, 10, 0.5f);

	forces_.insert(wind_);
	forces_.insert(tornado_);

	activateWind();
}

void WindSystem::keyPress(unsigned char key) {
	switch (tolower(key)) {
	case 'o':
		activateTornado();
		break;
	case 'p':
		activateWind();
		break;
	case 'k':
	case 'l':
		wind_->keyPress(key);
		break;

	default: break;
	}
}

void WindSystem::generateParticles(double t) {
	// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
	for (auto pg : generators_) {
		// El update se encarga de generar las partículas
		// según el tiempo de generación de cada generador
		auto parts = pg.second->update(t);
		for (auto p : parts) {
			p->setInvMass(1 / 5.0f);
			particles_.push_back(p);
			if (windActive_) partForceReg_->addForce(wind_, p);
			else partForceReg_->addForce(tornado_, p);
		}
	}
}

void WindSystem::activateWind() {
	windActive_ = true;
	generators_.erase("gen");

	Particle* p = new Particle(true, 4);
	p->setInitVel({ 5, 10, 0 });

	GaussianParticleGenerator* pg = new GaussianParticleGenerator(0.1, 0, 0.4, 10, false, true, true, true);
	pg->changeModelPart(p);
	pg->changeGenerateN(3);
	pg->setOrigin({ -30, 0, -100 });

	generators_.insert({ "gen", pg });

	delete p;
}

void WindSystem::activateTornado() {
	windActive_ = false;
	generators_.erase("gen");

	Particle* p = new Particle(true, 10);

	GaussianParticleGenerator* pg = new GaussianParticleGenerator(0.1, 0, 0.8, 5, false, true, true, true);
	pg->changeModelPart(p);
	pg->changeGenerateN(1);
	pg->setOrigin({ 0, 0, -100 });

	generators_.insert({ "gen", pg });

	delete p;
}