#pragma once

#include "../Practicas/ParticleForceSystem.h"
#include <string>
class DRigidBody;
class SRigidBody;
class Pin;

class ProySys : public ParticleForceSystem {
protected:
	// Convertir a radianes
	inline float rads(float deg) { return 0.017453292 * deg; }
	inline double rads(double deg) { return 0.017453292 * deg; }

	// Pelota
	const float BALLSIZE_ = 1.2f,
				BALLFR_ = 0.7f,
				BALLRESTIT_ = 0.0f,
				BALLMASS_ = 1.105f;		// Para que pese alrededor de 8kg
	const Vector4 BALLCOLOR_ = { 46 / 255.0f, 163 / 255.0f, 57 / 255.0f, 1.0f };
	const Vector3 INITPOS_ = { 0, 0, -10 };

	// Suelo y paredes
	const float FLOORH_ = 1,
		FLOORW_ = 10,
		WALLH_ = 3,
		WALLW_ = 1,

		FLOORFR_ = 0.7f,
		FLOORRESTIT_ = 0.0f,
		WALLFR_ = 1.0f,
		WALLRESTIT_ = 0.2f;
	const Vector4 FLOORCOLOR_ = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f },
				  BORDERSCOLOR_ = { 0.6f, 0.6f, 0.6f, 1.0f };

	// Muelle y velocidades
	const float K_ = 50000,
		RESTINGLENGTH_ = WALLW_ * 4,
		MINVEL_ = 0.1f,
		MINDIST_ = 0.5f;
	const Vector4 SPRINGSSCOLOR_ = { 0.9f, 0.9f, 0.9f, 1.0f };


	const Vector3 CAMOFFSET_ = { 18, 10, 18 },
				  ANCHOROFFSET_ = { 7, FLOORH_ + WALLW_, 7 };
	const int INITROT_ = 90;
	
	enum colisions { ball = 0, spring, floor, walls, items };
	std::vector<PxU32> colGroups;
	std::vector<PxU32> colMasks;

	
	PxPhysics* gPhysics_;
	PxScene* gScene_;

	DRigidBody* ball_, *spring_;
	SRigidBody* anchor_;
	Camera* camera_;

	ForceGenerator* throw_, *impulse_;

	Vector3 lastPos_;
	int rot_ = INITROT_;
	float maxSpd_, timeToStop_;

	bool pushing_, rotating_, shot_;
	
	int score_;




public:
	ProySys(PxPhysics* gPhysics, PxScene* gScene);

	void keyPress(unsigned char key);
	void update(double t);

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);


protected:
	void createBall();
	void resetBall();
	
	void createSprings();

	void createMap();
	void createFloor(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createWall(Vector3 dims, Vector3 pos, PxQuat rot = PxQuat(0, Vector3(1, 0, 0)));
	void createPin(Vector3 pos);

	void followBall(Vector3 ballP);
	void rotateSpring(Vector3 ballP);
};

