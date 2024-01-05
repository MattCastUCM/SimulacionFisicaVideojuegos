#pragma once

#include "../Practicas/ParticleForceSystem.h"
#include <string>

#include "../Practicas/Particles/SRigidBody.h"
#include "../Practicas/Particles/DRigidBody.h"

#include "../Practicas/ForceGenerators/GravityForceGenerator.h"
#include "../Practicas/ForceGenerators/SpringForceGenerator.h"
#include "ImpulseForceGenerator.h"
#include "../Practicas/ForceGenerators/ExplosionForceGenerator.h"

#include "Pin.h"
#include "Booster.h"
#include "Barrel.h"
#include "Debris.h"
#include "Smoke.h"

#include "../Practicas/ParticleGenerators/GaussianParticleGenerator.h"
#include "../Practicas/Particles/Firework.h"
#include "../Practicas/ParticleGenerators/GaussianParticleGenerator.h"
#include "../Practicas/ParticleGenerators/CircleGenerator.h"
#include "../Practicas/ParticleGenerators/HeartGenerator.h"
#include <time.h>

class ProySys : public ParticleForceSystem {
protected:
	// Convertir a radianes
	inline float rads(float deg) { return 0.017453292 * deg; }
	inline double rads(double deg) { return 0.017453292 * deg; }


	// Pelota
	const float BALLSIZE_ = 1.2f,
				BALLDAMP_ = 0.3f,
				BALLFR_ = 0.5f,
				BALLRESTIT_ = 0.0f,
				BALLMASS_ = 1.105f;
	const Vector4 BALLCOLOR_ = { 46 / 255.0f, 163 / 255.0f, 57 / 255.0f, 1.0f };
	const Vector3 INITPOS_ = { 0, 0, -5 }, 
				  FINALPOS_ = { 50, 50, 0 };


	// Suelo y paredes
	const float FLOORH_ = 1,
				FLOORW_ = 10,
				WALLH_ = 3,
				WALLW_ = 1,

				FLOORFR_ = 0.1f,
				FLOORRESTIT_ = 0.0f,
				WALLFR_ = 1.0f,
				WALLRESTIT_ = 0.2f;
	const Vector4 FLOORCOLOR_ = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f },
				  BORDERSCOLOR_ = { 0.6f, 0.6f, 0.6f, 1.0f };


	// Generadores de fuerzas
	GravityForceGenerator* gr_;
	ImpulseForceGenerator* impulse_;
	SpringForceGenerator* throw_;
	ExplosionForceGenerator* expl_;

	const float G_ = -9.81f,
				SPRINGMASS_ = 1 / 300.0f,
				K_ = 30000,
				RESTINGLENGTH_ = WALLW_ * 4,

				IMPULSEVEL_ = 150,

				EXPLK_ = 5000,
				EXPLR_ = 5,
				EXPLT_ = 0.1f;

	const Vector3 ANCHOROFFSET_ = { 6.3f, FLOORH_ + WALLW_, 6.3f },
				  EXPLRND_ = { 2, -2, 2 };
	const Vector4 SPRINGSSCOLOR_ = { 0.9f, 0.9f, 0.9f, 1.0f };


	// Generadores de partículas
	GaussianParticleGenerator* debrisGen_, *smokeGen_;
	
	std::vector<Barrel*> barrels_;
	const float DEBRGENTIME_ = 0.1,
				DEBRGENMEAN_ = 0,
				DEBRGENDEV_ = 1,
				DEBRGENOFF_ = 2,
		
				SMOKEGENTIME_ = 0.1,
				SMOKEGENMEAN_ = 0,
				SMOKEGENDEV_ = 0.08,
				SMOKEGENOFF_ = 2,

				PARTLIFETIME_ = 5;

	const int DEBRGENN_ = 20, SMOKEGENN_ = 1;
	float smokeTime_;
	bool genDebris_;


	std::vector<ParticleGenerator*> fireworkShooters_;
	const float FIREWORKSIZE = 0.05f,
				FIREWORKLIFETIME_ = 3,
				SPHEREGENTIME_ = 0,
				SPHEREMEAN_ = 0,
				SPHEREDEV_ = 0.1f,
				SPHEREOFF_ = 0,

				CIRCLEGENTIME_ = 0,
				CIRCLEMEAN_ = 0,
				CIRCLEDEV_ = 0.05f,
				CIRCLEOFF_ = 0,

				HEARTGENTIME_ = 0,
				HEARTMEAN_ = 0,
				HEARTDEV_ = 0.05f,

				SHOOTERGENTIME_ = 5.0f,
				SHOOTERMEAN_ = 0,
				SHOOTERDEV_ = 0.1f,
				SHOOTEROFF_ = 5;
	const std::vector<Vector3> SHOOTERSPOS_ = {
		{ -100, -30, -50}, { -100, 0, -200}, {20, -40, -200}
	};


	// Límites y valores iniciales
	const float INITROT_ = 90,
				TIMETOSTOP_ = 2.0f, 
				CAMROTSPD_ = 2.0f,
				MINVEL_ = 1.8f,
				MINDIST_ = 2.0f,
				MINEXPLVEL_ = 5.0f;


	const Vector3 CAMOFFSET_ = { 18, 10, 18 };
	Camera* camera_;


	PxPhysics* gPhysics_;
	PxScene* gScene_;

	DRigidBody* ball_;
	SRigidBody* end_;
	Particle* spring_, *anchor_;
	

	Vector3 lastPos_;
	float rot_, stopTimer_, finishTimer_;
	bool pushing_, rotating_, shot_, stop_, finish_, pinsFalling_;
	int score_;

	std::function<void()> callback_;

public:
	ProySys(PxPhysics* gPhysics, PxScene* gScene);

	void keyPress(unsigned char key);
	void update(double t);

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	inline void setCallback(std::function<void()>funct) { callback_ = funct; };

protected:
	void createBall();
	void resetBall();
	
	void createSprings();
	void setupExpl();

	ParticleGenerator* setupFireworks(Vector3 pos, int i);


	void createMap();
	SRigidBody* createFloor(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createWall(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createPin(Vector3 pos);
	void createBarrel(Vector3 pos);
	void createBooster(Vector3 pos, Vector3 dir, PxQuat rot = PxQuat(PxHalfPi / 2, Vector3(0, 1, 0)));
	void createDebris();


	void followBall(Vector3 ballP);
	void rotateSpring(Vector3 ballP);

	
};

