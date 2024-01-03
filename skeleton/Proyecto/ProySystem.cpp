#include "ProySystem.h"

#include "../Practicas/Particles/SRigidBody.h"
#include "../Practicas/Particles/DRigidBody.h"

#include "RigidSpringForceGenerator.h"
#include "ImpulseForceGenerator.h"


#include "Pin.h"


#include <iostream>
using namespace std;

ProySys::ProySys(PxPhysics* gPhysics, PxScene* gScene) 
	: ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100), gPhysics_(gPhysics), gScene_(gScene) 
{
	// Matrices de colisión
	colGroups = {
		colisions::ball,
		colisions::spring,
		colisions::floor,
		colisions::walls,
		colisions::items
	};

	colMasks = {
		0,
		/*0xFFFFFFFF,	*/														// (Pelota) colisiona con todo
		((PxU32)1 << colGroups[floor]) | ((PxU32)1 << colGroups[ball]),		// (Muelle) colisiona solo con suelo y pelota
		0xFFFFFFFF,															// (Suelos) colisionan con todo
		~((PxU32)1 << colGroups[spring]),									// (Paredes) colisionan con todo menos el muelle
		~((PxU32)1 << colGroups[spring])									// (Objetos) colisionan con todo menos el muelle
	};

	score_ = 0;
	camera_ = GetCamera();
	camera_->setPos({ 0, 10, 5 });


	partForceReg_ = new ParticleForceRegistry();

	impulse_ = new ImpulseForceGenerator({ 0,0,1 }, 20);
	forces_.insert(impulse_);
	partForceReg_->addForce(impulse_, ball_);

	createSprings();

	impulse_->setActive(false);
	throw_->setActive(false);


	createBall();
	resetBall();
	
	createMap();
}
	


void ProySys::keyPress(unsigned char key) {
	auto k = tolower(key);

	if (!shot_) {
		// Cargar
		if (k == 's') {
			rotating_ = false;
			throw_->setActive(false);
			spring_->setVel({ 0, 0, 0 });

			auto posDif = (spring_->getPos() - anchor_->getPos()).normalize();
			auto dir = camera_->getDir();
			dir.y = 0;
			auto movement = 0.1f;
			if (posDif > 1.5 && pushing_) {
				spring_->setPos(spring_->getPos() - dir * movement);
				if (posDif <= 1.6) pushing_ = false;
			}
			else if (!pushing_) {
				spring_->setPos(spring_->getPos() + dir * movement);
				if (posDif > RESTINGLENGTH_ / 1.3) pushing_ = true;
			}
		}
		// Lanzar
		else if (k == ' ') {
			if(rotating_) lastPos_ = ball_->getPos();
			rotating_ = false;

			throw_->setActive(true);

			/*auto d = camera_->getDir();
			ball_->addForce(d * 1000);*/

		}

		// Rotar
		else if (k == 'd') {
			rot_++;
			rot_ %= 360;
			camera_->rotate(false);

			rotating_ = true;
			throw_->setActive(false);
		}
		else if (k == 'a') {
			rot_--;
			rot_ %= 360;
			camera_->rotate(true);

			rotating_ = true;
			throw_->setActive(false);
		}
	}


}

void ProySys::update(double t) {
	auto ballP = ball_->getPos();
	if (ballP.y < -5) resetBall();

#ifndef CamaraTeclas
	followBall(ballP);
	if (rotating_ && !shot_) rotateSpring(ballP);
#endif
	spring_->setAngularVel({ 0, 0, 0 });

	float posDif = (ball_->getPos() - lastPos_).normalize();
	maxSpd_ = max(maxSpd_, ball_->getVel().normalize());
	//cout << maxSpd_ << ' ' << ball_->getVel().normalize() << '\n';
	if (shot_ && ball_->getVel().normalize() < maxSpd_ / 5 && posDif > MINDIST_) {
		maxSpd_ = 0;
		ball_->setVel({ 0, 0, 0 });
		ball_->setAngularVel({ 0, 0, 0 });

		shot_ = false;
		rotating_ = true;
	}

	ParticleForceSystem::update(t);
}


void ProySys::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	if (actor1 == ball_->getRigidActor() || actor2 == ball_->getRigidActor()) {

		if (!shot_ && (actor1 == spring_->getRigidActor() || actor2 == spring_->getRigidActor())) {
			shot_ = true;
		}
	}
}




void ProySys::createBall() {
	Particle::visual v;
	v.size = BALLSIZE_;
	v.geometry = new physx::PxSphereGeometry(v.size);
	v.color = BALLCOLOR_;
	v.material = gPhysics_->createMaterial(BALLFR_, BALLFR_, BALLRESTIT_);


	Particle::physics p;
	p.pos = INITPOS_;

	// MASS GUARDA LA DENSIDAD
	p.mass = BALLMASS_;
	p.damp = 0;

	p.colGrp = colGroups[colisions::ball];
	p.colMask = colMasks[colisions::ball];

	ball_ = new DRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(ball_);

	lastPos_ = p.pos;
}

void ProySys::resetBall() {
	shot_ = false;
	pushing_ = rotating_ = true;
	throw_->setActive(false);

	ball_->setPos(lastPos_);
	ball_->setVel({ 0,0,0 });
	ball_->setAngularVel({ 0,0,0 });
	maxSpd_ = 0;
	timeToStop_ = 0;

	rot_ = INITROT_;
	camera_->resetRot();
	
	spring_->setVel({ 0, 0, 0 });

}


void ProySys::createSprings() {
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(WALLW_ / 2, WALLW_ / 4, WALLW_ / 4);
	v.color = SPRINGSSCOLOR_;
	v.material = gPhysics_->createMaterial(0, 0, 0);

	Particle::physics p;
	p.pos = INITPOS_ + Vector3(0, 0, ANCHOROFFSET_.z);
	p.pos.y = FLOORH_ + WALLW_;

	anchor_ = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(anchor_);


	v.geometry = new physx::PxBoxGeometry(BALLSIZE_ * 2, WALLW_, WALLW_);

	p.damp = 1;
	p.mass = 15;
	p.pos += Vector3(0, 0, RESTINGLENGTH_);
	p.colGrp = colGroups[colisions::spring];
	p.colMask = colMasks[colisions::spring];

	spring_ = new DRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(spring_);


	// No hace falta añadir la partícula estática a las fuerzas
	throw_ = new RigidSpringForceGenerator(K_, RESTINGLENGTH_, anchor_);
	forces_.insert(throw_);
	partForceReg_->addForce(throw_, spring_);
	throw_->setActive(true);

}



void ProySys::createMap() {
	Vector3 nextPos = { 0,0,0 };
	nextPos.z += -40;
	createFloor({ FLOORW_, FLOORH_, 50 }, nextPos);
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, { 0, 0, 10 });
	createWall({ WALLW_, WALLH_, 50 }, nextPos + Vector3(-FLOORW_, 0, 0));
	createWall({ WALLW_, WALLH_, 50 }, nextPos + Vector3(FLOORW_, 0, 0));

	nextPos.z += -50;
	createFloor({ FLOORW_, FLOORH_, 20 }, nextPos, PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(-FLOORW_, 0, 0), PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(FLOORW_, 0, 0), PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));

	nextPos.y += 9.85f;
	nextPos.z += -36.9f;
	createFloor({ FLOORW_, FLOORH_, 20 }, nextPos);
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(-FLOORW_, 0, 0));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(FLOORW_, 0, 0));




	createPin({ 0, 3.0f, -20 });
}

void ProySys::createFloor(Vector3 dims, Vector3 pos, PxQuat rot) {
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(dims);
	v.color = FLOORCOLOR_;
	v.material = gPhysics_->createMaterial(FLOORFR_, FLOORFR_, FLOORRESTIT_);

	Particle::physics p;
	p.pos = pos;
	p.colGrp = colGroups[colisions::floor];
	p.colMask = colMasks[colisions::floor];

	SRigidBody* floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	PxTransform tr = PxTransform(floor->getRigidActor()->getGlobalPose().p, rot);
	floor->getRigidActor()->setGlobalPose(tr);
}

void ProySys::createWall(Vector3 dims, Vector3 pos, PxQuat rot) {
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(dims);
	v.color = BORDERSCOLOR_;
	v.material = gPhysics_->createMaterial(WALLFR_, WALLFR_, WALLRESTIT_); 

	Particle::physics p;
	p.pos = pos;
	p.colGrp = colGroups[colisions::walls];
	p.colMask = colMasks[colisions::walls];

	SRigidBody* wall = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(wall);

	PxTransform tr = PxTransform(wall->getRigidActor()->getGlobalPose().p, rot);
	wall->getRigidActor()->setGlobalPose(tr);
}




void ProySys::createPin(Vector3 pos) {
	auto pin = new Pin(gPhysics_, gScene_, pos, colGroups[colisions::items], colMasks[colisions::items], [&](Pin* cb) {
		// Callback: cuando un bolo llama a esta función, se añade a la 
		// lista de partículas a eliminar y se actualiza la puntuación
		remove_.push_back((cb));
		score_++;
		changeScore(score_);
	});

	particles_.push_back(pin);
}









void ProySys::followBall(Vector3 ballP) {
	Vector3 rotatedPos;
	float rotation = rot_;
	rotatedPos.x = physx::PxCos(rads(rotation)) * CAMOFFSET_.x;
	rotatedPos.y = CAMOFFSET_.y;
	rotatedPos.z = physx::PxSin(rads(rotation)) * CAMOFFSET_.z;

	if (rot_ == 90) rotatedPos.x = 0;
	camera_->setPos(ballP + rotatedPos);
}

void ProySys::rotateSpring(Vector3 ballP) {
	Vector3 anchorPos;
	
	float rotation = rot_;

	anchorPos.x = physx::PxCos(rads(rotation)) * ANCHOROFFSET_.x;
	anchorPos.y = 0;
	anchorPos.z = physx::PxSin(rads(rotation)) * ANCHOROFFSET_.z;
	
	anchor_->setPos(ballP + anchorPos);

	PxTransform rotatedAnchor(anchor_->getRigidActor()->getGlobalPose().p, PxQuat(3 * PxHalfPi + rads(rotation), Vector3(0, -1, 0)));
	anchor_->getRigidActor()->setGlobalPose(rotatedAnchor);


	Vector3 springPos;
	springPos.x = physx::PxCos(rads(rotation));
	springPos.y = 0;
	springPos.z = physx::PxSin(rads(rotation));
	spring_->setPos(anchor_->getPos() + springPos * -RESTINGLENGTH_);

	PxTransform rotatedSpring(spring_->getRigidActor()->getGlobalPose().p, PxQuat(3 * PxHalfPi + rads(rotation), Vector3(0, -1, 0)));
	spring_->getRigidActor()->setGlobalPose(rotatedSpring);
}
