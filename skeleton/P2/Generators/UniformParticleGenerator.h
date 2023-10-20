#pragma once

#include "ParticleGenerator.h"
class UniformParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::uniform_real_distribution<double> unifDistr_{ 0, 1 };


public:
	UniformParticleGenerator(double genTime, double min, double max, bool changeX = true, bool changeY = false, bool changeZ = true) 
		: ParticleGenerator(genTime, changeX, changeY, changeZ) 
	{
		unifDistr_.param(std::uniform_real_distribution<double>::param_type(min, max));
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
		if (changeX_) posX = unifDistr_(mt_) + p->getInitPos().x;
		if (changeY_) posY = unifDistr_(mt_) + p->getInitPos().y;
		if (changeZ_) posZ = unifDistr_(mt_) + p->getInitPos().z;
		p->setInitPos({ posX, posY, posZ });


		auto velMagn = p->getInitVel().magnitude();
		float velX = p->getInitVel().x,
			velY = p->getInitVel().y,
			velZ = p->getInitVel().z;
		if (changeX_) velX = unifDistr_(mt_) * velMagn;
		if (changeY_) velY = unifDistr_(mt_) * velMagn;
		if (changeZ_) velZ = unifDistr_(mt_) * velMagn;
		p->setVel({ velX, velY, velZ });
	}
};