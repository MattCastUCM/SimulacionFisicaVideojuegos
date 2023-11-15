#pragma once

#include "../ParticleForceSystem.h"
//#include "../ForceGenerators/WindForceGenerator.h"
#include "../ForceGenerators/TornadoForceGenerator.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"

class WindSystem : public ParticleForceSystem {
protected:
	WindForceGenerator* wind_;


public:
	WindSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleForceSystem(g) {
		partForceReg_ = new ParticleForceRegistry();

		activateWind();
	}

	inline void keyPress(unsigned char key) { 
		switch (tolower(key)) {
			case 'o':
				activateTornado();
				break;
			case 'p':
				activateWind();
				break;
			case 'k':
				increaseWindVel({ 0, -10, 0 });
				break;
			case 'l':
				increaseWindVel({ 0, 10, 0 }); 
				break;

			default: break;
		}
	};
	

	inline void generateParticles(double t) override {
		// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
		for (auto pg : generators_) {
			// El update se encarga de generar las partículas
			// según el tiempo de generación de cada generador
			auto parts = pg.second->update(t);
			for (auto p : parts) {
				particles_.push_back(p);
				partForceReg_->addForce(wind_, p);
			}
		}
	}

	inline void activateWind() {
		generators_.erase("gen");

		Particle* p = new Particle(true, 4);
		wind_ = new WindForceGenerator({ 100, 0, 0 }, 0.5f, 0.0); //{ -100, 0, -200 }, { -20, 50, 100 }
		p->setInitVel({ 5, 10, 0 });

		GaussianParticleGenerator* pg = new GaussianParticleGenerator(0.1, 0, 0.4, 10, false, true, true, true);
		pg->changeModelPart(p);
		pg->changeGenerateN(3);
		pg->setOrigin({ -30, 0, -100 });

		generators_.insert({ "gen", pg });

		delete p;
	}


	inline void activateTornado() {
		generators_.erase("gen");

		Particle* p = new Particle(true, 10);
		wind_ = new TornadoForceGenerator({ 0, 0, -100 }, { 0.1, 0, 0 }, 10, 0.5f);

		GaussianParticleGenerator* pg = new GaussianParticleGenerator(0.1, 0, 0.8, 5, false, true, true, true);
		pg->changeModelPart(p);
		pg->changeGenerateN(10);
		pg->setOrigin({ 0, 0, -100 });

		generators_.insert({ "gen", pg });

		delete p;
	}

	inline void increaseWindVel(Vector3 vel) {
		wind_->setVel(wind_->getVel() + vel);
	}

};

