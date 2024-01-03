#pragma once

#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"

class ParticleForceSystem : public ParticleSystem {
protected:
	ParticleForceRegistry* partForceReg_;
	std::unordered_set<ForceGenerator*> forces_;

	inline virtual void refresh() {
		for (auto p : particles_) {
			if (!p->isAlive()) {
				p->onDeath();
				remove_.push_back(p);
				partForceReg_->deleteParticleRegistry(p);
			}
		}

		for (auto it = remove_.begin(); it != remove_.end(); ) {
			particles_.remove(*it);
			delete* it;
			it = remove_.erase(it);
		}

	}


public:
	// Se usa -10.0f como gravedad por defecto
	ParticleForceSystem(const Vector3& g = { 0.0f, -10.0f, 0.0f }, int maxParts = 1000) : ParticleSystem(g, maxParts), partForceReg_(nullptr), forces_() { };
	
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
		}
		particles_.clear();
	}
	inline void clearForces() {
		for (auto f : forces_) {
			partForceReg_->deleteForceRegistry(f);
			delete f;
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


	inline virtual void keyPress(unsigned char key) {
		for (auto pg : generators_) pg.second->keyPress(key);
		for (auto fg : forces_) fg->keyPress(key);
	};


};

