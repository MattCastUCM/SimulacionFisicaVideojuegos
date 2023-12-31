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
	float laneWidth = 1.0f,
		restingWidth = 1.5f,
		laneDepth = 10.0f,
		restingDepth = 4.5f / 2,
		floorFr = 0.04f,
		floorRestit = 0.0f,
		ballFr = 0.45f,
		ballRestit = 0.0f;

	Vector4 bordersColor = { 0.4f, 0.4f, 0.4f, 1.0f };
	int initRot_ = 90;
	int rot_ = initRot_;


	PxPhysics* gPhysics_;
	PxScene* gScene_;

	DRigidBody* ball_;
	SRigidBody* borders_, * hole_;
	Camera* camera_;

	ForceGenerator* throw_, * holePush_;
	bool shot_, inBorders_;

	
public:
	ProySys(PxPhysics* gPhysics, PxScene* gScene);

	void keyPress(unsigned char key);
	void update(double t);

	virtual void generateParticles(double t);
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);


private:
	void createBall();
	void resetBall();

	void createFloors();

	void updateCamera(Vector3 ballP);
};

