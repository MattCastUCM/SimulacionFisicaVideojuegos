#pragma once

#include "../Particle.h"

class DRigidBody : public Particle {
protected:
	PxPhysics* gPhysics_;
	PxScene* gScene_;
	PxRigidActor* rigidActor_;
	PxRigidDynamic* rigid_;

	virtual void init(visual vis, physics phys, float maxLifetime = 1.0f);


public:
	DRigidBody(bool default = false, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	DRigidBody(visual vis, physics phys, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	virtual ~DRigidBody();

	virtual void update(double t);

	virtual void addForce(const Vector3& f);

	virtual Particle* clone();


	inline void setPos(Vector3 pos) { 
		tr_->p = pos;
		rigid_->setGlobalPose(*tr_);
	}

	inline void setVel(Vector3 v) { 
		vel_ = v; 
		rigid_->setLinearVelocity(vel_);
	}
	
	// ASUMIENDO QUE LO QUE SE GUARDA COMO MASA ES SU INVERSO
	inline void setInvMass(float m) { 
		phys_.mass = m; 
		mass_ = m;
		rigid_->setMass(1 / mass_);
	}

	inline void setDamp(float d) { 
		phys_.damp = d;
		rigid_->setLinearDamping(phys_.damp);
	}

	PxRigidActor* getRigidActor() { return rigidActor_; }

};

