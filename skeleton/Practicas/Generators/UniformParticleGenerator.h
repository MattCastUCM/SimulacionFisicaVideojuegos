#pragma once

#include "../ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::uniform_real_distribution<double> unifDistr_{ 0, 1 };


public:
	UniformParticleGenerator(double genTime, double min, double max, bool autoInactive = false, bool changeX = true, bool changeY = false, bool changeZ = true)
		: ParticleGenerator(genTime, autoInactive, changeX, changeY, changeZ) 
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
		auto pos = origin_;

		p->setInitPos(origin_);
		p->setPos(origin_);

		if (changeX_) pos.x += unifDistr_(mt_);
		if (changeY_) pos.y += unifDistr_(mt_);
		if (changeZ_) pos.z += unifDistr_(mt_);
		p->setInitPos(pos);


		auto velMagn = vel_.magnitude();
		auto vel = vel_;
		if (changeX_) vel.x = unifDistr_(mt_) * velMagn;
		if (changeY_) vel.y = unifDistr_(mt_) * velMagn;
		if (changeZ_) vel.z = unifDistr_(mt_) * velMagn;
		p->setVel(vel);
	}
};