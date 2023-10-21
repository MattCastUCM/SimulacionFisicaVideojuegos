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

	void setActive(bool gen) { generate_ = gen; }


	inline void setVelocities(Particle* p) {
		auto pos = p->getInitPos();
		float posX = p->getInitPos().x,
			posY = p->getInitPos().y,
			posZ = p->getInitPos().z;
		if (changeX_) posX = normDistr_(mt_) + p->getInitPos().x;
		if (changeY_) posY = normDistr_(mt_) + p->getInitPos().y;
		if (changeZ_) posZ = normDistr_(mt_) + p->getInitPos().z;
		p->setInitPos({ posX, posY, posZ });


		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			 velZ = p->getInitVel().z;
		if (changeX_) velX = normDistr_(mt_) * velMagn;
		if (changeZ_) velZ = normDistr_(mt_) * velMagn;
		p->setVel({ velX, 0.0f, velZ });
	}
};