#pragma once

#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"

class ParticleForceSystem : public ParticleSystem {
protected:
	ParticleForceRegistry* partForceReg_;
	std::list<ForceGenerator*> forces_;

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
	ParticleForceSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g), partForceReg_(nullptr), forces_() { };
	
	virtual ~ParticleForceSystem() {
		if(partForceReg_ != nullptr) delete partForceReg_;
		for (auto f : forces_) delete f;
	};
	
	inline virtual void update(double t) {
		if(partForceReg_ != nullptr) partForceReg_->updateForces(t);
		
		ParticleSystem::update(t);
	}

	inline void clearParts() {
		for (auto p : particles_) {
			partForceReg_->deleteParticleRegistry(p);
			delete p;
			p = nullptr;
		}
		particles_.clear();
	}
	inline void clearForces() {
		for (auto f : forces_) {
			partForceReg_->deleteForceRegistry(f);
			delete f;
			f = nullptr;
		}
		forces_.clear();
	}
	inline void clearPartForces() {
		clearParts();
		clearForces();
	}


	inline virtual void setActive(bool active) {
		ParticleSystem::setActive(active);
		if (!active_) clearParts();
	}


	


};

