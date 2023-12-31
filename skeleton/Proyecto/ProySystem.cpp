#include "ProySystem.h"

#include "../Practicas/Particles/SRigidBody.h"
#include "../Practicas/Particles/DRigidBody.h"
#include "ThrowForceGenerator.h"
#include "HolePushForceGenerator.h"
#include "../Practicas/ParticleGenerators/GaussianParticleGenerator.h"
#include "Pin.h"

#include <iostream>
using namespace std;

ProySys::ProySys(PxPhysics* gPhysics, PxScene* gScene) : ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100), gPhysics_(gPhysics), gScene_(gScene) {
	camera_ = GetCamera();


	partForceReg_ = new ParticleForceRegistry();

	throw_ = new ThrowForceGenerator();
	forces_.insert(throw_);

	holePush_= new HolePushForceGenerator();
	forces_.insert(holePush_);


	createBall();
	resetBall();
	createFloors();
	camera_->setPos({ 0, 3, 1 });

	auto pin = new Pin(gPhysics, gScene, { 0, 3.0f, -5 });

	
}

void ProySys::keyPress(unsigned char key) {
	
	switch (tolower(key)) {
	// Solo
	case ' ':
		partForceReg_->addForce(throw_, ball_);
		if (!shot_) {
			
			shot_ = true;
		}
		break;
	case 'a':
		if (rot_ > 30) {
			rot_--;
			camera_->rotate(key);
		}
		break;
	case 'd':
		if (rot_ < 150) {
			rot_++;
			camera_->rotate(key);
		}
		break;
	default: break;
	}
}

void ProySys::update(double t) {
	auto ballP = ball_->getPos();

	if (ballP.y < -5) resetBall();

	//updateCamera(ballP);
	


	ParticleForceSystem::update(t);

	// Elimina la pelota del generador de empuje 
	// para que solo sea empujada 1 vez
	if (shot_) {
		partForceReg_->removeForce(throw_, ball_);
	}
}

inline void ProySys::generateParticles(double t) {
	//// Recorrer generadores (generar partículas nuevas y añadirlas a la lista)
	//for (auto pg : generators_) {
	//	// El update se encarga de generar las partículas
	//	// según el tiempo de generación de cada generador
	//	auto parts = pg.second->update(t);
	//	for (auto p : parts) {
	//		particles_.push_back(p);
	//		
	//		if(explAct_) partForceReg_->addForce(expl_, p);
	//	}
	//}
}

void ProySys::onCollision(physx::PxActor* actor1, physx::PxActor* actor2){
	if (actor1 == ball_->getRigidActor() || actor2 == ball_->getRigidActor()) {
		if (actor1 == hole_->getRigidActor() || actor2 == hole_->getRigidActor()) {
			cout << "ball in hole\n";
			resetBall();
		}
		else if (!inBorders_ && (actor1 == borders_->getRigidActor() || actor2 == borders_->getRigidActor())) {
			inBorders_ = true;
			cout << "ball in borders \n";
			partForceReg_->addForce(holePush_, ball_);

		}
	}
}




void ProySys::createBall() {
	Particle::visual v;
	v.size = 0.12f;

	v.geometry = new physx::PxSphereGeometry(v.size);
	v.color = { 46 / 255.0f, 163 / 255.0f, 57 / 255.0f, 1.0f };
	v.material = gPhysics_->createMaterial(ballFr, ballFr, ballRestit);


	Particle::physics p;
	p.pos = { 0, 2.0f, -restingDepth + 0.5f };

	//p.pos = { 0, 2, -restAreaDepth + 0.5f };

	// MASS GUARDA LA DENSIDAD
	// una bola de bolos de media pesa 8kg y tiene 12cm de radio => D = 11.05kg/m3
	p.mass = 11.05f * 100;
	p.damp = 0;

	ball_ = new DRigidBody(v, p, -1, gPhysics_, gScene_);
	ball_->setPos(p.pos);

	particles_.push_back(ball_);
}

void ProySys::resetBall() {
	shot_ = inBorders_ = false;
	ball_->setPos(ball_->getInitPos());
	ball_->setVel({ 0,0,0 });
	rot_ = initRot_;
	camera_->resetRot();
	
	partForceReg_->deleteParticleRegistry(ball_);
}

void ProySys::createFloors() {
	// Pista
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(laneWidth, 0.5f, laneDepth + restingDepth);
	v.color = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f };
	v.material = gPhysics_->createMaterial(floorFr, floorFr, floorRestit);

	Particle::physics p;
	p.pos = { 0, 0, -(laneDepth + restingDepth) };

	SRigidBody* floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);


	// Zona de "descanso"
	v.geometry = new physx::PxBoxGeometry(restingWidth, 0.5f, restingDepth);
	p.pos = { 0, 0, 0 };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);


	// Linea del medio (partícula norma, sin colisiones)
	v.geometry = new physx::PxBoxGeometry(1.0635f, 0.5f, 0.1);
	v.color = { 1, 0, 0, 1.0f };
	p.pos = { 0, 0.001f, -restingDepth + 0.1f };
	auto line = new Particle(v, p, -1);
	particles_.push_back(line);



	// Huecos
	v.geometry = new physx::PxBoxGeometry(restingWidth, 0.5f, laneDepth + restingDepth);
	v.color = bordersColor;
	p.pos = { 0, -0.25f, -(laneDepth + restingDepth) };
	borders_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(borders_);


	// Paredes huecos
	// Izquierdo
	v.geometry = new physx::PxBoxGeometry(0.1f, 0.5f, laneDepth + restingDepth);
	p.pos = { -restingWidth, 0.1f, -(laneDepth + restingDepth * 2) };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Derecho
	p.pos = { restingWidth, 0.1f, -(laneDepth + restingDepth * 2) };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);



	// Paredes zona de "descanso"
	// Izquierdo delante
	v.geometry = new physx::PxBoxGeometry((restingWidth - laneWidth) / 2, 0.5f, 0.1f);
	p.pos = { -(laneWidth + (restingWidth - laneWidth) / 2), 0.25f, -restingDepth + 0.1f };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Derecho delante
	p.pos = { laneWidth + (restingWidth - laneWidth) / 2, 0.25f, -restingDepth + 0.1f };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Atrás
	v.geometry = new physx::PxBoxGeometry(restingWidth + (restingWidth - laneWidth) / 2, 0.5f, 0.1f);
	p.pos = { 0, 0.25f, restingDepth };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Izquierdo
	v.geometry = new physx::PxBoxGeometry(0.1f, 0.5f, restingDepth);
	p.pos = { -(restingWidth + 0.1f), 0.25f, 0 };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Derecho
	p.pos = { restingWidth + 0.1f, 0.25f, 0 };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);



	// Cosos del final
	// Base
	v.geometry = new physx::PxBoxGeometry(restingWidth, 0.5f, 2);
	v.color = bordersColor;
	p.pos = { 0, -0.75, -(laneDepth + restingDepth) * 2};
	hole_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(hole_);

	// Pared
	v.geometry = new physx::PxBoxGeometry(restingWidth, 2, 0.1f);
	p.pos = { 0, 0,  -((laneDepth + restingDepth) * 2 + 2) };
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);
}

void ProySys::updateCamera(Vector3 ballP) {
	Vector3 offset(3, 1, 3);

	Vector3 rotatedPos;
	float rotation = rot_;
	rotatedPos.x = physx::PxCos(rads(rotation)) * offset.x;
	rotatedPos.y = offset.y;
	rotatedPos.z = physx::PxSin(rads(rotation)) * offset.z;

	if (rot_ == 90) rotatedPos.x = 0;
	camera_->setPos(ballP + rotatedPos);
}
