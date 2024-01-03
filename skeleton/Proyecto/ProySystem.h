#pragma once

#include "../Practicas/ParticleForceSystem.h"
class DRigidBody;
class SRigidBody;

class ProySys : public ParticleForceSystem {
private:
	// Convertir a radianes
	inline float rads(float deg) { return 0.017453292 * deg; }
	inline double rads(double deg) { return 0.017453292 * deg; }


protected:
	// CONST cuando estén bien ajustadas
	/*float ballSize = 1.2f,
		floorHeight = 1,
		laneWidth = 10,
		restingWidth = 15,
		laneDepth = 100,
		restingDepth = 45 / 2.0f,
		wallSize = 1,
		endDepth = 20,*/

	float ballSize = 1.2f,
		floorH = 1,
		floorW = 10,
		wallH = 3,
		wallW = 1,

		floorFr = 0.04f,
		floorRestit = 0.0f,
		wallFr = 0.5f,
		wallRestit = 0.5f,
		ballFr = 0.45f,
		ballRestit = 0.0f,

		k = 200,
		restingLength = wallW * 2;

	
	Vector4 floorColor = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f },
			bordersColor = { 0.4f, 0.4f, 0.4f, 1.0f };
	Vector3 camOffset = { 15, 7, 15 }, springOffset = { 3, 0, 3 };
	int initRot_ = 90;

	int rot_ = initRot_;

	PxPhysics* gPhysics_;
	PxScene* gScene_;

	DRigidBody* ball_, *spring_;
	SRigidBody* anchor_;
	Camera* camera_;

	ForceGenerator* throw_, *impulse_;
	bool shot_, inBorders_;


	void createBall();
	void resetBall();

	void createMap();
	void createFloors();

	void createSprings();

	void updateCamera(Vector3 ballP);
	

public:
	ProySys(PxPhysics* gPhysics, PxScene* gScene);

	void keyPress(unsigned char key);
	void update(double t);

	virtual void generateParticles(double t);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

};

