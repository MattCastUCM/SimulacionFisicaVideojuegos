#include "ProySystem.h"
#include <iostream>
using namespace std;
#include "../checkMemLeaks.h"

ProySys::ProySys(PxPhysics* gPhysics, PxScene* gScene)
	: ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100), gPhysics_(gPhysics), gScene_(gScene), barrels_()
{
	finish_ = false;

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

	createBall();


	partForceReg_ = new ParticleForceRegistry();

	impulse_ = new ImpulseForceGenerator({ 0,0,1 }, IMPULSEK_);
	forces_.insert(impulse_);
	partForceReg_->addForce(impulse_, ball_);

	createSprings();
	createExpl();


	impulse_->setActive(false);
	throw_->setActive(false);
	expl_->setActive(false);
	explPartGen_->setActive(false);
	
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
			if (!shot_) lastPos_ = ball_->getPos();
			rotating_ = false;
			spring_->setVel({ 0, 0, 0 });
			throw_->setActive(true);
		}
	}
	
	// Rotar
	if (k == 'd') {
		rot_++;
		rot_ %= 360;
		camera_->rotate(false);
		rotating_ = true;
	}
	else if (k == 'a') {
		rot_--;
		rot_ %= 360;
		camera_->rotate(true);
		rotating_ = true;
	}

}

void ProySys::update(double t) {
	auto ballP = ball_->getPos();
	if (ballP.y < -5) resetBall();

	// La cámara solo sigue a la pelota si está desactivado
	// el movimiento de la cámara con las teclas
#ifndef CamaraTeclas
	followBall(ballP);
#endif
	// Se rota el muelle si no se ha disparado la pelota
	if (rotating_ && !shot_) rotateSpring(ballP);

	// Si el muelle toca la pelota, se le añade la fuerza y se indica que se ha disparado
	if ((spring_->getPos() - ball_->getPos()).normalize() < BALLSIZE_ + WALLW_ / 2) {
		ball_->addForce(-throw_->getForce());
		shot_ = true;
	}

	// Si la pelota ha sido disparada y la velocidad es inferior
	// a la mínima, comienza el temporizador para ver si se ha
	// Frenado completamente o si se ha frenado por una cuesta y 
	// luego va a seguir moviéndose
	if (shot_ && ball_->getVel().normalize() < MINVEL_) {
		stop_ = true;
		stopTimer_ += t;
	}
	else {
		stopTimer_ = 0;
		stop_ = false;
	}
	// Si se ha frenado completamente, se vuelve
	// a activar el muelle y el lanzamiento
	if (stop_ && stopTimer_ >= TIMETOSTOP_) {
		ball_->setVel({ 0, 0, 0 });
		ball_->setAngularVel({ 0, 0, 0 });

		stop_ = shot_ = false;
		rotating_ = true;
	}


	ParticleForceSystem::update(t);
}


void ProySys::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	if (actor1 == ball_->getRigidActor() || actor2 == ball_->getRigidActor()) {
		// Va comprobando por cada barril si ha colisionado con la pelota
		for (auto b : barrels_) {
			if (actor1 == b->getRigidActor() || actor2 == b->getRigidActor()) {
				// Si la pelota ha chocado con el barril y supera la velocidad mínima,
				// el barril explota, se borra, y disminuye la puntuación en 1
				if(ball_->getVel().normalize() >= MINEXPLVEL_){
					ball_->setVel({ 0,0,0 });

					expl_->setOrigin(b->getPos() + explPartGen_->genRandomVec(EXPLRND_));
					expl_->resetTime();
					expl_->setActive(true);

					remove_.push_back(b);
					score_--;
					changeScore(score_);

					explPartGen_->setOrigin(b->getPos());
					// Genera las N partículas solo al activar el generador, no hace 
					// falta ir generando más conforme vaya pasando el tiempo.
					// Luego las añade a la lista de partículas y al registro
					// de fuerzas y se les asigna una masa inversa aleatoria
					// entre 1 y 1/15
					auto parts = explPartGen_->generateParticles();
					for (auto part : parts) {
						particles_.push_back(part);
						//partForceReg_->addForce(expl_, part);

						int rndMass = rand() % 15 + 1;
						part->setInvMass(1.0f / rndMass);
					}
				}
			}
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
	p.damp = 0.998;

	p.colGrp = colGroups[colisions::ball];
	p.colMask = colMasks[colisions::ball];

	ball_ = new DRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(ball_);

	lastPos_ = p.pos;
}

void ProySys::resetBall() {
	shot_ = stop_ = false;
	pushing_ = rotating_ = true;

	ball_->setPos(lastPos_);
	ball_->setVel({ 0,0,0 });
	ball_->setAngularVel({ 0,0,0 });

	stopTimer_ = 0;
	
	rot_ = INITROT_;
	camera_->resetRot();
	
	spring_->setVel({ 0, 0, 0 });
	impulse_->setActive(false);
	throw_->setActive(false);
	expl_->setActive(false);

}


void ProySys::createSprings() {
	Particle::visual v;
	v.geometry = new physx::PxBoxGeometry(WALLW_ / 2, WALLW_ / 4, WALLW_ / 4);
	v.color = SPRINGSSCOLOR_;
	v.material = gPhysics_->createMaterial(0, 0, 0);

	Particle::physics p;
	p.pos = INITPOS_ + Vector3(0, 0, ANCHOROFFSET_.z);
	p.pos.y = FLOORH_ + WALLW_;

	anchor_ = new Particle(v, p, -1);
	particles_.push_back(anchor_);


	v.geometry = new physx::PxBoxGeometry(BALLSIZE_ * 2, WALLW_, WALLW_);

	p.damp = 0.5f;
	p.mass = SPRINGMASS_;
	p.pos += Vector3(0, 0, RESTINGLENGTH_);
	p.colGrp = colGroups[colisions::spring];
	p.colMask = colMasks[colisions::spring];

	spring_ = new Particle(v, p, -1);
	particles_.push_back(spring_);


	// No hace falta añadir la partícula estática a las fuerzas
	throw_ = new RotatedSpringForceGenerator(K_, RESTINGLENGTH_, anchor_);
	forces_.insert(throw_);
	partForceReg_->addForce(throw_, spring_);
	throw_->setActive(true);

}

void ProySys::createExpl() {
	expl_ = new RigidExplosionForceGenerator(EXPLK_, EXPLR_, EXPLT_, { 0, 0, 0 });
	forces_.insert(expl_);
	partForceReg_->addForce(expl_, ball_);

	
	Particle* p = new Smoke();
	explPartGen_ = new GaussianParticleGenerator(EXPLGENTIME_, EXPLGENMEAN_, EXPLGENDEV_, EXPLGENOFF_, true, true, true, true);
	explPartGen_->changeModelPart(p);
	explPartGen_->changeGenerateN(50);
	explPartGen_->setName("explPartGen");
	generators_.insert({ "explPartGen", explPartGen_ });
	delete p;

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
	
	//createBooster(nextPos + Vector3(0, 5, -7), {0, 0, -1}, PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));


	nextPos.y += 9.85f;
	nextPos.z += -36.9f;
	createFloor({ FLOORW_, FLOORH_, 20 }, nextPos);
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(-FLOORW_, 0, 0));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(FLOORW_, 0, 0));

	nextPos.z += -FLOORW_ * 3;
	createFloor({ FLOORW_, FLOORH_, FLOORW_ }, nextPos);
	createWall({ WALLW_, WALLH_, FLOORW_ }, nextPos + Vector3(FLOORW_, 0, 0));
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, -FLOORW_));

	nextPos.x += -FLOORW_ * 3;
	createFloor({ FLOORW_ * 2, FLOORH_, FLOORW_ }, nextPos);
	createWall({ FLOORW_ * 2 + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, -FLOORW_));
	createWall({ FLOORW_ * 2 + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, +FLOORW_));

	nextPos.x += -FLOORW_ * 3;
	createFloor({ FLOORW_, FLOORH_, FLOORW_ }, nextPos);
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, -FLOORW_));
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, +FLOORW_));

	createPin(nextPos);
	createPin(nextPos + Vector3(-2, 0.01f, 2));
	createPin(nextPos + Vector3(-2, 0.01f, -2));
	createPin(nextPos + Vector3(-4, 0.01f, 0));
	createPin(nextPos + Vector3(-4, 0.01f, 4));
	createPin(nextPos + Vector3(-4, 0.01f, -4));
	createPin(nextPos + Vector3(-6, 0.01f, 2));
	createPin(nextPos + Vector3(-6, 0.01f, -2));
	createPin(nextPos + Vector3(-6, 0.01f, 6));
	createPin(nextPos + Vector3(-6, 0.01f, -6));

	nextPos.x += -FLOORW_ * 2;
	float height = 7;
	nextPos.y += -height;
	end_ = createFloor({ FLOORW_, FLOORH_, FLOORW_ }, nextPos);
	createWall({ FLOORW_ + WALLW_, WALLH_ + height * 1.5f, WALLW_ }, nextPos + Vector3(0, height * 1.5f, -FLOORW_));
	createWall({ FLOORW_ + WALLW_, WALLH_ + height * 1.5f, WALLW_ }, nextPos + Vector3(0, height * 1.5f, +FLOORW_));
	createWall({ WALLW_ , WALLH_ + height * 1.5f, FLOORW_ }, nextPos + Vector3(-FLOORW_, height * 1.5f, 0));
	createWall({ WALLW_ , WALLH_, FLOORW_ }, nextPos + Vector3(FLOORW_, height / 2.0f, 0));

	nextPos.y += height * 3;
	createFloor({ FLOORW_, FLOORH_, FLOORW_ }, nextPos);


	createBarrel({ 0, 3.0f, -20 });
	//createBarrel({ 0, 3.0f, -50 });

}


SRigidBody* ProySys::createFloor(Vector3 dims, Vector3 pos, PxQuat rot) {
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

	return floor;
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
		remove_.push_back(cb);
		score_++;
		changeScore(score_);
	});

	particles_.push_back(pin);
}

void ProySys::createBarrel(Vector3 pos) {
	auto barrel = new Barrel(gPhysics_, gScene_, pos);
	particles_.push_back(barrel);
	barrels_.push_back(barrel);
}

void ProySys::createBooster(Vector3 pos, Vector3 dir, PxQuat rot) {
	Booster* boost = new Booster(gPhysics_, gScene_, pos, [=]() {
			impulse_->setDir(dir);
			impulse_->setActive(true);
			cout << "impulsed\n";
		}
	);
	particles_.push_back(boost);

	PxTransform tr = PxTransform(boost->getRigidActor()->getGlobalPose().p, rot);
	boost->getRigidActor()->setGlobalPose(tr);
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
	auto quat = PxQuat(3 * PxHalfPi + rads(rotation), Vector3(0, -1, 0));

	anchorPos.x = physx::PxCos(rads(rotation)) * ANCHOROFFSET_.x;
	anchorPos.y = 0;
	anchorPos.z = physx::PxSin(rads(rotation)) * ANCHOROFFSET_.z;
	
	if (rotation == 90) anchorPos.x = 0;
	
	anchor_->setPos(ballP + anchorPos);
	anchor_->setRot(quat);

	Vector3 springPos;
	springPos.x = physx::PxCos(rads(rotation));
	springPos.y = 0;
	springPos.z = physx::PxSin(rads(rotation));
	
	spring_->setPos(anchor_->getPos() + springPos * -RESTINGLENGTH_);
	spring_->setRot(quat);

}
