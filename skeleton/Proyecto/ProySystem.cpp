#include "ProySystem.h"

#include "../Practicas/Particles/SRigidBody.h"
#include "../Practicas/Particles/DRigidBody.h"
#include "RigidSpringForceGenerator.h"
#include "ImpulseForceGenerator.h"

#include "../Practicas/ParticleGenerators/GaussianParticleGenerator.h"
#include "Pin.h"

#include <iostream>
using namespace std;

ProySys::ProySys(PxPhysics* gPhysics, PxScene* gScene) : ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100), gPhysics_(gPhysics), gScene_(gScene) {
	camera_ = GetCamera();

	partForceReg_ = new ParticleForceRegistry();
	

	createBall();
	resetBall();
	//createFloors();
	createMap();
	createSprings();


	camera_->setPos({ 0, 10, 5 });

	auto pin = new Pin(gPhysics, gScene, { 0, 3.0f, -5 });
	particles_.push_back(pin);
	

	impulse_ = new ImpulseForceGenerator({ 0,0,1 }, 20);
	forces_.insert(impulse_);
	partForceReg_->addForce(impulse_, ball_);

	impulse_->setActive(false);
	/*throw_->setActive(false);*/
}



void ProySys::createBall() {
	Particle::visual v;
	v.size = ballSize;

	v.geometry = new physx::PxSphereGeometry(v.size);
	v.color = { 46 / 255.0f, 163 / 255.0f, 57 / 255.0f, 1.0f };
	v.material = gPhysics_->createMaterial(ballFr, ballFr, ballRestit);


	Particle::physics p;
	p.pos = { 0, 2.0f, 0 };
	//p.pos = { 0, 2.0f, -restingDepth + wallSize * 2 };

	// MASS GUARDA LA DENSIDAD
	// Para que pese alrededor de 8kg
	p.mass = 1.105f;
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


void ProySys::createMap() {

	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(floorW, floorH, 50);
	v.color = floorColor;
	v.material = gPhysics_->createMaterial(floorFr, floorFr, floorRestit);

	Particle::physics p;
	p.pos = { 0, 0, -40 };

	SRigidBody* floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);


	//Paredes
	// Atras
	v.geometry = new physx::PxBoxGeometry(floorW + wallW, wallH, wallW);
	v.color = bordersColor;
	v.material = gPhysics_->createMaterial(wallFr, wallFr, wallRestit);

	p.pos.z = 10;
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);

	// Lados
	v.geometry = new physx::PxBoxGeometry(wallW, wallH, 50);

	//Izquierdo
	p.pos.x = -floorW;
	p.pos.z = -40;
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	// Derecho
	p.pos.x = floorW;
	floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);
}

void ProySys::createSprings() {
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(ballSize * 3, wallW, wallW);
	v.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Particle::physics p;
	p.mass = 10;
	p.pos = ball_->getPos() + Vector3(0, 0, springOffset.z);
	p.pos.y = floorH + wallW;

	spring_ = new DRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(spring_);

	v.geometry = new physx::PxBoxGeometry(wallW / 2, wallW / 2, wallW / 2);
	//p.pos += Vector3(0, wallW * 20, 0);
	p.pos += Vector3(0, 0, wallW * 4);
	anchor_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(anchor_);


	// No hace falta añadir la partícula estática a las fuerzas
	throw_ = new RigidSpringForceGenerator(k, restingLength, anchor_);
	forces_.insert(throw_);
	partForceReg_->addForce(throw_, spring_);
	throw_->setActive(true);




	
}






void ProySys::keyPress(unsigned char key) {
	
	switch (tolower(key)) {
	// Solo
	case ' ':
		throw_->setActive(true);
		//spring_->setPos(spring_->getPos() - Vector3(0, 0, -0.01f));
		//impulse_->setActive(true);
		break;
	case 'a':
		rot_++;
		rot_ %= 360;
		camera_->rotate(false);
		break;
	case 'd':
		rot_--;
		rot_ %= 360;
		camera_->rotate(true);
		break;

	default: break;
	}
}

void ProySys::update(double t) {
	auto ballP = ball_->getPos();

	if (ballP.y < -5) resetBall();

	//updateCamera(ballP);
	
	
	//if (delta > 0 && delta < 7) {
	//	spring_->setPos(spring_->getPos() - Vector3(0, 0, 0.01f));
	//	cout << "A\n";
	//}
	//else {
	//	spring_->setPos(spring_->getPos() + Vector3(0, 0, 0.01f));
	//	cout << "B\n";
	//}

	ParticleForceSystem::update(t);

}

void ProySys::generateParticles(double t) {
}

void ProySys::onCollision(physx::PxActor* actor1, physx::PxActor* actor2){
	if (actor1 == ball_->getRigidActor() || actor2 == ball_->getRigidActor()) {
		/*if (actor1 == hole_->getRigidActor() || actor2 == hole_->getRigidActor()) {
			cout << "ball in hole\n";
			resetBall();
		}
		else if (!inBorders_ && (actor1 == borders_->getRigidActor() || actor2 == borders_->getRigidActor())) {
			inBorders_ = true;
			cout << "ball in borders \n";
			partForceReg_->addForce(holePush_, ball_);

		}*/
	}
}






void ProySys::createFloors() {
	//// Pista
	//Particle::visual v;
	//v.geometry = new physx::PxBoxGeometry(laneWidth, floorHeight, laneDepth + restingDepth);
	//v.color = { 181 / 255.0f, 131 / 255.0f, 78 / 255.0f, 1.0f };
	//v.material = gPhysics_->createMaterial(floorFr, floorFr, floorRestit);

	//Particle::physics p;
	//p.pos = { 0, 0, -(laneDepth + restingDepth) };

	//SRigidBody* floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);


	//// Zona de "descanso"
	//v.geometry = new physx::PxBoxGeometry(restingWidth, floorHeight, restingDepth);
	//p.pos = { 0, 0, 0 };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);


	//// Linea del medio (partícula norma, sin colisiones)
	//v.geometry = new physx::PxBoxGeometry(laneWidth, floorHeight, wallSize);
	//v.color = { 1, 0, 0, 1.0f };
	//p.pos = { 0, 0.01f, -restingDepth + wallSize };
	//auto line = new Particle(v, p, -1);
	//particles_.push_back(line);



	//// Huecos
	//v.geometry = new physx::PxBoxGeometry(laneWidth + ballSize * 3, floorHeight, laneDepth + restingDepth);
	//v.color = bordersColor;
	//p.pos = { 0, -floorHeight * 2, -(laneDepth + restingDepth) };
	//borders_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(borders_);


	//// Paredes huecos
	//v.geometry = new physx::PxBoxGeometry(wallSize, floorHeight * 4, laneDepth + restingDepth);

	//// Izquierdo
	//p.pos = { -(laneWidth + wallSize + ballSize * 3), -floorHeight, -(laneDepth + restingDepth * 2) };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);

	//// Derecho
	//p.pos = { laneWidth + wallSize + ballSize * 3, -floorHeight, -(laneDepth + restingDepth * 2) };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);



	//// Paredes zona de "descanso"
	//v.geometry = new physx::PxBoxGeometry((restingWidth - laneWidth) / 2, floorHeight * 2, wallSize);

	//// Izquierdo delante
	//p.pos = { -(laneWidth + (restingWidth - laneWidth) / 2), floorHeight, -restingDepth + wallSize };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);

	//// Derecho delante
	//p.pos = { laneWidth + (restingWidth - laneWidth) / 2, floorHeight, -restingDepth + wallSize };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);

	//// Atrás
	//v.geometry = new physx::PxBoxGeometry(restingWidth + (restingWidth - laneWidth) / 2, floorHeight * 2, wallSize);
	//p.pos = { 0, floorHeight, restingDepth };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);

	//// Izquierdo
	//v.geometry = new physx::PxBoxGeometry(wallSize, floorHeight * 2, restingDepth);
	//p.pos = { -(restingWidth + wallSize), floorHeight, 0 };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);

	//// Derecho
	//p.pos = { restingWidth + wallSize, floorHeight, 0 };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);



	//// Cosos del final
	//// Base
	//v.geometry = new physx::PxBoxGeometry(restingWidth, floorHeight, endDepth);
	//v.color = bordersColor;
	//p.pos = { 0, -floorHeight * 4, -(laneDepth + restingDepth) * 2};
	//hole_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(hole_);

	//// Pared
	//v.geometry = new physx::PxBoxGeometry(restingWidth, floorHeight * 10, wallSize);
	//p.pos = { 0, 0,  -((laneDepth + restingDepth) * 2 + endDepth) };
	//floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	//particles_.push_back(floor);
}



void ProySys::updateCamera(Vector3 ballP) {
	Vector3 rotatedPos;
	float rotation = rot_;
	rotatedPos.x = physx::PxCos(rads(rotation)) * camOffset.x;
	rotatedPos.y = camOffset.y;
	rotatedPos.z = physx::PxSin(rads(rotation)) * camOffset.z;

	if (rot_ == 90) rotatedPos.x = 0;
	camera_->setPos(ballP + rotatedPos);
}
