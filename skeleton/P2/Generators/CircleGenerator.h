#pragma once

#include "GaussianParticleGenerator.h"
class CircleGenerator : public GaussianParticleGenerator {
private:
	bool generate_;

public:
	// Media y desviación estándar
	CircleGenerator(double genTime, double mean, double dev) : GaussianParticleGenerator(genTime, mean, dev), generate_(false) { };

	inline std::list<Particle*> generateParticles() override {
		if (generate_) {
			std::list<Particle*> generated;
			Particle* p;
			for (int i = 0; i < generateN_; i++) {
				p = modelPart_->clone();
				setVelocities(p);

				generated.push_back(p);
			}
			generate_ = false;
			return generated;
		}
		else return { };
	};

	inline void activateGenerator() { generate_ = true; }
};