#pragma once

#include "PxPhysics.h"
#include "../core.hpp"
#include "../RenderUtils.hpp"
using namespace physx;

class Particle {
public:
	// Tamaño, forma y color
	struct visual {
		float size = 1.0f;
		physx::PxGeometry* geometry;
		Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	};

	// Físicas iniciales
	struct physics {
		Vector3 pos,	// Posición
			vel,		// Dirección/velocidad (vector)
			acc;		// Aceleración (vector) en m/s^2
		float damp,		// Rozamiento (magnitud)
			  mass;		// Masa (magnitud) en kg. Se utiliza para guardar el inverso de la masa al aplicar fuerzas
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
	inline Vector3 getPos() { return tr_->p; }


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

	inline float getMass() { return 1 /phys_.mass; }
	inline float getInvMass() { return phys_.mass; }

	inline float getSize() { return vis_.size; }

	inline virtual void setDamp(float d) { phys_.damp = d; }
	inline float getDamp() { return phys_.damp; }


};

