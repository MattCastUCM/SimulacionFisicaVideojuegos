#pragma once

#include "../Particle.h"

class SRigidBody : public Particle {
protected:
	PxPhysics* gPhysics_;
	PxScene* gScene_;
	PxRigidActor* rigidActor_;
	PxRigidStatic* rigid_;
	PxShape* shape_;

	virtual void init(visual vis, physics phys, float maxLifetime = 1.0f);


public:
	SRigidBody(bool default = false, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	SRigidBody(visual vis, physics phys, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	virtual ~SRigidBody();

	virtual void update(double t);

	virtual Particle* clone();


	inline void setPos(Vector3 pos) { 
		tr_->p = pos;
		rigid_->setGlobalPose(*tr_);
	}
	inline Vector3 getPos() { return rigid_->getGlobalPose().p; }


	inline PxRigidActor* getRigidActor() { return rigidActor_; }
	inline PxShape* getShape() { return shape_; }

};

