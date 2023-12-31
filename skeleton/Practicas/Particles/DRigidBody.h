#pragma once

#include "../Particle.h"

class DRigidBody : public Particle {
protected:
	PxPhysics* gPhysics_;
	PxScene* gScene_;
	PxRigidActor* rigidActor_;
	PxRigidDynamic* rigid_;
	PxShape* shape_;

	virtual void init(visual vis, physics phys, float maxLifetime = 1.0f);


public:
	DRigidBody(bool default = false, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	DRigidBody(visual vis, physics phys, float maxLifetime = 1.0f, PxPhysics* gPhys = nullptr, PxScene* gScene = nullptr);
	virtual ~DRigidBody();

	virtual void update(double t);

	virtual void addForce(const Vector3& f, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autoawake = true);
	//virtual void addTorque(const Vector3& t, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autoawake = true);

	virtual Particle* clone();


	inline void setPos(Vector3 pos) { 
		tr_->p = pos;
		rigid_->setGlobalPose(*tr_);
	}
	inline Vector3 getPos() { return rigid_->getGlobalPose().p; }

	inline void setVel(Vector3 v) { 
		vel_ = v; 
		rigid_->setLinearVelocity(vel_);
		rigid_->setAngularVelocity(vel_);
	}
	
	// ASUMIENDO QUE LO QUE SE GUARDA COMO MASA ES LA DENSIDAD
	inline void setInvMass(float m) { 
		phys_.mass = m; 
		mass_ = m;
		PxRigidBodyExt::updateMassAndInertia(*rigid_, mass_);
	}
	inline float getMass() { return rigid_->getMass(); }
	inline float getInvMass() { return 1 / rigid_->getMass(); }


	inline void setDamp(float d) { 
		phys_.damp = d;
		rigid_->setLinearDamping(phys_.damp);
	}

	inline PxRigidActor* getRigidActor() { return rigidActor_; }
	inline PxShape* getShape() { return shape_; }

};

