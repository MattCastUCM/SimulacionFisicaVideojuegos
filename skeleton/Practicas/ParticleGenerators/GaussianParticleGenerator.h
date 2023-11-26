#pragma once

#include "../ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	//Vector3 velWidth_, posWidth_;
	std::normal_distribution<float> normDistr_{ 0, 1 };
	int offset_;

public:
	// Media y desviación estándar
	GaussianParticleGenerator(double genTime, double mean, double dev, int offset = 0, 
		bool autoInactive = false, bool changeX = true, bool changeY = false, bool changeZ = true)
		: ParticleGenerator(genTime, autoInactive, changeX, changeY, changeZ), offset_(offset)
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


	inline virtual void setVelocities(Particle* p) {
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
		p->setVel(vel);
	}
};