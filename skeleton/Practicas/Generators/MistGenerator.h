#pragma once

#include "GaussianParticleGenerator.h"

class MistGenerator : public GaussianParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::normal_distribution<float> normDistr_{ 0, 1 };
	

public:
	// Media y desviación estándar
	MistGenerator(double genTime, double mean, double dev, int offset, bool autoInactive = false, bool changeX = true, bool changeY = false, bool changeZ = true)
		: GaussianParticleGenerator(genTime, mean, dev, offset, autoInactive, changeX, changeY, changeZ)
	{
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


	virtual inline void setVelocities(Particle* p) {
		auto pos = origin_;
		p->setInitPos(origin_);
		p->setPos(origin_);

		if (changeX_) pos.x += normDistr_(mt_) * offset_;
		if (changeY_) pos.y += normDistr_(mt_) * offset_;
		if (changeZ_) pos.z += normDistr_(mt_) * offset_;
		p->setInitPos(pos);
		p->setPos(pos);

		auto velMagn = vel_.magnitude();
		auto vel = vel_;
		if (changeX_) vel.x = normDistr_(mt_) * velMagn;
		if (changeY_) vel.y = normDistr_(mt_) * velMagn;
		if (changeZ_) vel.z = normDistr_(mt_) * velMagn;
		p->setInitVel(vel);
		p->setVel(vel);
	}
};