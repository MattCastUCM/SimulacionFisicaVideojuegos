#pragma once

#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"

class ParticleForceSystem : public ParticleSystem {
protected:
	ParticleForceRegistry* partForceReg_;

	virtual inline void refresh() {
		for (auto it = particles_.begin(); it != particles_.end(); ) {
			if (!(*it)->isAlive()) {
				(*it)->onDeath();
				partForceReg_->deleteParticleRegistry(*it);

				delete* it;
				it = particles_.erase(it);
			}
			else ++it;
		}
	}


public:
	// Se usa -10.0f como gravedad por defecto
	ParticleForceSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g), partForceReg_(nullptr) { };
	
	virtual ~ParticleForceSystem() {
		if(partForceReg_ != nullptr) delete partForceReg_;
	};
	
	inline virtual void update(double t) {
		if(partForceReg_ != nullptr) partForceReg_->updateForces(t);
		
		ParticleSystem::update(t);
	}

	virtual inline void setActive(bool active) {
		ParticleSystem::setActive(active);

		if (!active_) {
			for (auto p : particles_) {
				partForceReg_->deleteParticleRegistry(p);
				delete p;
			}
			particles_.clear();
		}
	}
};

