#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"
using namespace physx;
#include "../checkMemLeaks.h"

class Particle {
public:
	// Tamaño, forma y color
	struct visual {
		float size = 1.0f;
		physx::PxGeometry* geometry = nullptr;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		PxMaterial* material = nullptr;
		bool clone = false;
	};

	// Físicas iniciales
	struct physics {
		Vector3 pos = { 0, 0, 0 },	// Posición
			vel = { 0, 0, 0 },		// Dirección/velocidad (vector)
			acc = { 0, 0, 0 };		// Aceleración (vector) en m/s^2
		float damp = 0.998f,		// Rozamiento (magnitud)
			  mass = 1 / 5.0f;		// Masa (magnitud) en kg. Se utiliza para guardar el inverso de la masa al aplicar fuerzas

		// Grupos de colisión (solo para rigidos)
		PxU32 colGrp = 0, colMask = 0xFFFFFFFF;
	};

protected:
	physx::PxTransform* tr_;		// Transform de la esfera (Su posición se va actualizando)
	RenderItem* renderItem_;		// Objeto renderizable

	visual vis_;
	physics phys_;

	// Físicas que se actualizan luego (porque se simulan o integran)
	Vector3 vel_, acc_;					
	float mass_;
	Vector3 accumForce_;

	float maxLifetime_;
	float lifetime_;
	bool alive_;


	virtual void init(visual vis, physics phys, float maxLifetime = 1.0f);
	

public:
	Particle(bool default = false, float maxLifetime = 1.0f);
	Particle(visual vis, physics phys, float maxLifetime = 1.0f);
	virtual ~Particle();
	virtual void onDeath() { };

	virtual void update(double t);

	virtual void clearForce();
	virtual void addForce(const Vector3& f);

	virtual Particle* clone();


	// Obtener o cambiar posición (inicial o actual)
	inline void setInitPos(Vector3 pos) { phys_.pos = pos; }
	inline Vector3 getInitPos() { return phys_.pos; }

	inline void setPos(Vector3 pos) { tr_->p = pos; }
	inline virtual Vector3 getPos() { return tr_->p; }


	// Obtener o cambiar velociad (inicial o actual/simulada)
	inline void setInitVel(Vector3 v) { phys_.vel = v; }
	inline Vector3 getInitVel() { return phys_.vel; }

	inline virtual void setVel(Vector3 v) { vel_ = v; }
	inline Vector3 getVel() { return vel_; }


	// Obtener o cambiar velocidad (inicial o actual/simulada)
	void setInitAcc(Vector3 a) { phys_.acc = a; }
	inline Vector3 getInitAcc() { return phys_.acc; }
	
	void setAcc(Vector3 a) { acc_ = a; }
	inline Vector3 getAcc() { return acc_; }

	inline bool isAlive() { return alive_; }
	inline void changeLifetime(float t) { maxLifetime_ = t; }


	// ASUMIENDO QUE LO QUE SE GUARDA COMO MASA ES SU INVERSO
	inline virtual void setInvMass(float m) {
		phys_.mass = m; 
		mass_ = m;
	}

	inline virtual float getMass() { return 1 /phys_.mass; }
	inline virtual float getInvMass() { return phys_.mass; }

	inline float getSize() { return vis_.size; }

	inline virtual void setDamp(float d) { phys_.damp = d; }
	inline float getDamp() { return phys_.damp; }

	inline physx::PxTransform& getTransform() { return *tr_; }
	inline void setRot(PxQuat rot) { tr_->q = rot; }
};

