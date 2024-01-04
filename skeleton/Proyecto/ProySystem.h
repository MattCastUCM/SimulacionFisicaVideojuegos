#pragma once

#include "../Practicas/ParticleForceSystem.h"
#include <string>

#include "../Practicas/Particles/SRigidBody.h"
#include "../Practicas/Particles/DRigidBody.h"

#include "RotatedSpringForceGenerator.h"
#include "ImpulseForceGenerator.h"
#include "RigidExplosionForceGenerator.h"

#include "Pin.h"
#include "Booster.h"
#include "Barrel.h"
#include "Smoke.h"

#include "../Practicas/ParticleGenerators/GaussianParticleGenerator.h"

class ProySys : public ParticleForceSystem {
protected:
	// Convertir a radianes
	inline float rads(float deg) { return 0.017453292 * deg; }
	inline double rads(double deg) { return 0.017453292 * deg; }


	// Pelota
	const float BALLSIZE_ = 1.2f,
				BALLFR_ = 1,
				BALLRESTIT_ = 0.0f,
				BALLMASS_ = 10.0f;
	const Vector4 BALLCOLOR_ = { 46 / 255.0f, 163 / 255.0f, 57 / 255.0f, 1.0f };
	const Vector3 INITPOS_ = { 0, 0, -10 };
	//const Vector3 INITPOS_ = { -3, 3.0f, -20 };


	// Suelo y paredes
	const float FLOORH_ = 1,
		FLOORW_ = 10,
		WALLH_ = 3,
		WALLW_ = 1,

		FLOORFR_ = 1,
		FLOORRESTIT_ = 0.0f,
		WALLFR_ = 1.0f,
		WALLRESTIT_ = 0.2f;
	const Vector4 FLOORCOLOR_ = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f },
				  BORDERSCOLOR_ = { 0.6f, 0.6f, 0.6f, 1.0f };


	// Fuerzas y frenado
	const float SPRINGMASS_ = 1 / 300.0f,
				K_ = 30000,
				RESTINGLENGTH_ = WALLW_ * 4,

				IMPULSEK_ = 1000,

				EXPLK_ = 10000,
				EXPLR_ = 50,
				EXPLT_ = 1,
				EXPLGENTIME_ = 0,
				EXPLGENMEAN_ = 0,
				EXPLGENDEV_ = 0.1,
				EXPLGENOFF_ = 0,

				MINVEL_ = 1.8f,
				MINDIST_ = 2.0f,
				MINEXPLVEL_ = 5.0f;

	const Vector4 SPRINGSSCOLOR_ = { 0.9f, 0.9f, 0.9f, 1.0f };

	const Vector3 CAMOFFSET_ = { 18, 10, 18 },
				  ANCHOROFFSET_ = { 6.5f, FLOORH_ + WALLW_, 6.5f },
				  EXPLRND_ = Vector3(5, -5, 5);


	const int INITROT_ = 90;
	const float TIMETOSTOP_ = 2.0f;


	enum colisions { ball = 0, spring, floor, walls, items };
	std::vector<PxU32> colGroups;
	std::vector<PxU32> colMasks;

	
	PxPhysics* gPhysics_;
	PxScene* gScene_;

	DRigidBody* ball_;
	SRigidBody* end_;
	Particle* spring_, *anchor_;
	Camera* camera_;

	RotatedSpringForceGenerator* throw_;
	ImpulseForceGenerator* impulse_;
	RigidExplosionForceGenerator* expl_;

	Vector3 lastPos_;
	int rot_ = INITROT_;
	
	float stopTimer_;
	bool pushing_, rotating_, shot_, stop_, finish_;
	
	int score_;

	GaussianParticleGenerator* explPartGen_;
	std::vector<Barrel*> barrels_;

public:
	ProySys(PxPhysics* gPhysics, PxScene* gScene);

	void keyPress(unsigned char key);
	void update(double t);

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

protected:
	void createBall();
	void resetBall();
	
	void createSprings();
	void createExpl();


	void createMap();
	SRigidBody* createFloor(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createWall(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createPin(Vector3 pos);
	void createBarrel(Vector3 pos);
	void createBooster(Vector3 pos, Vector3 dir, PxQuat rot = PxQuat(PxHalfPi / 2, Vector3(0, 1, 0)));



	void followBall(Vector3 ballP);
	void rotateSpring(Vector3 ballP);

};

