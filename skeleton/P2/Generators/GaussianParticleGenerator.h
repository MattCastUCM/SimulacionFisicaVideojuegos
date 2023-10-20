#pragma once

#include "ParticleGenerator.h"
class GaussianParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::normal_distribution<float> normDistr_{ 0, 1 };


public:
	// Media y desviación estándar
	GaussianParticleGenerator(double genTime, double mean, double dev, bool changeX = true, bool changeY = false, bool changeZ = true) 
		: ParticleGenerator(genTime, changeX, changeY, changeZ) {
		normDistr_.param(std::normal_distribution<float>::param_type(mean, dev));
	};

	inline std::list<Particle*> generateParticles() override {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			setVelocities(p);

			generated.push_back(p);
		}

		return generated;
	};


	inline void setVelocities(Particle* p) {
		auto pos = p->getInitPos();
		float posX = p->getInitPos().x,
			posY = p->getInitPos().y, 
			posZ = p->getInitPos().z;
		if (changeX_) posX = normDistr_(mt_) + p->getInitPos().x;
		if (changeY_) posY = normDistr_(mt_) + p->getInitPos().y;
		if (changeZ_) posZ = normDistr_(mt_) + p->getInitPos().z;
		p->setInitPos({posX, posY, posZ});


		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			 velY = p->getInitVel().y,
			 velZ = p->getInitVel().z;
		if (changeX_) velX = normDistr_(mt_) * velMagn;
		if (changeY_) velY = normDistr_(mt_) * velMagn;
		if (changeZ_) velZ = normDistr_(mt_) * velMagn;
		p->setVel({ velX, velY, velZ});
	}
};