#include "ProySystem.h"
#include <iostream>
using namespace std;
#include "../checkMemLeaks.h"

ProySys::ProySys(PxPhysics* gPhysics, PxScene* gScene)
	: ParticleForceSystem({ 0.0f, -9.8f, 0.0f }, 100), gPhysics_(gPhysics), gScene_(gScene), barrels_()
{
	callback_ = nullptr;
	genDebris_ = finish_ = pinsFalling_ = false;
	score_ = finishTimer_ = 0;
	rot_ = INITROT_;

	camera_ = GetCamera();
	camera_->setPos({ 0, 10, 5 });

	createBall();


	partForceReg_ = new ParticleForceRegistry();

	gr_ = new GravityForceGenerator({ 0, G_ , 0 });
	forces_.insert(gr_);

	impulse_ = new ImpulseForceGenerator({ 0,0,1 }, IMPULSEVEL_);
	forces_.insert(impulse_);
	partForceReg_->addForce(impulse_, ball_);

	createSprings();
	setupExpl();

	impulse_->setActive(false);
	throw_->setActive(false);
	expl_->setActive(false);
	debrisGen_->setActive(false);
	smokeGen_->setActive(false);

	resetBall();
	createMap();
	
}
	


void ProySys::keyPress(unsigned char key) {
	auto k = tolower(key);
	if (!finish_) {
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
			int newRot = rot_ += CAMROTSPD_;
			newRot %= 360;
			rot_ = newRot;
			camera_->rotate(false, CAMROTSPD_);
			rotating_ = true;
		}
		else if (k == 'a') {
			int newRot = rot_ -= CAMROTSPD_;
			newRot %= 360;
			rot_ = newRot;
			camera_->rotate(true, CAMROTSPD_);
			rotating_ = true;
		}

	}
	else if (finishTimer_ >= TIMETOSTOP_) {
		if (k == 'p') {
			callback_();
		}
	}

}

void ProySys::update(double t) {
	if (!finish_ && finishTimer_ < TIMETOSTOP_) {
		auto ballP = ball_->getPos();

		// La cámara solo sigue a la pelota si está desactivado
		// el movimiento de la cámara con las teclas
#ifndef CamaraTeclas
		followBall(ballP);
#endif

		if (ballP.y < -5) resetBall();

		// Se rota el muelle si no se ha disparado la pelota
		if (rotating_ && !shot_ && !finish_) rotateSpring(ballP);

		// Si el muelle toca la pelota, se le añade la fuerza y se indica que se ha disparado
		auto posDif = (spring_->getPos() - ball_->getPos()).normalize();
		if (posDif < BALLSIZE_ + WALLW_ / 2) {
			ball_->addForce(-throw_->getForce());
			shot_ = true;
		}
		// Si la pelota se ha alejado lo suficiente, se envía
		// el muelle lejos para que parezca que ha desaparecido
		if (shot_ && posDif >= MINDIST_) {
			spring_->setPos({ 1000, 1000, 1000 });
			anchor_->setPos({ 1000, 1000, 1000 });
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


		if (genDebris_) {
			genDebris_ = false;
			createDebris();
		}

		if (smokeGen_->isActive()) {
			smokeTime_ += t;
			if (smokeTime_ >= PARTLIFETIME_ * 2) {
				smokeGen_->setActive(false);
			}
		}

		
	}

	else if (finishTimer_ < TIMETOSTOP_ && !pinsFalling_) {
		finishTimer_ += t;
		
		if (finishTimer_ >= TIMETOSTOP_) {
			camera_->setPos({ 50, 50, 0 });
			camera_->resetRot();
			camera_->rotate(true, 45);

			setFinish(true);
		}
	}
	ParticleForceSystem::update(t);
}


void ProySys::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	if (!finish_) {
		if (actor1 == ball_->getRigidActor() || actor2 == ball_->getRigidActor()) {
			if (actor1 == end_->getRigidActor() || actor2 == end_->getRigidActor()) {
				finish_ = true;
			}
		}
		// Va comprobando por cada barril si ha colisionado con algún dynamic rigid body
		for (auto b : barrels_) {
			if (actor1 == b->getRigidActor() || actor2 == b->getRigidActor()) {
				auto other = actor1;
				if (actor1 == b->getRigidActor()) other = actor2;

				if (other->is<PxRigidDynamic>()) {
					// Si supera la velocidad mínima, el barril explota, se borra, y disminuye la puntuación en 1
					if (((DRigidBody*)other)->getVel().normalize() >= MINEXPLVEL_) {
						ball_->setVel({ 0,0,0 });

						expl_->setOrigin(b->getPos() + Vector3(0, -FLOORH_, 0) + debrisGen_->genRandomVec(EXPLRND_));
						expl_->resetTime();
						expl_->setActive(true);
						gr_->setActive(true);

						remove_.push_back(b);
						score_--;
						changeScore(score_);

						debrisGen_->setOrigin(b->getPos() + Vector3(0, FLOORH_, 0));
						genDebris_ = true;

						smokeTime_ = 0;
						smokeGen_->setActive(true);
						smokeGen_->setOrigin(b->getPos() + Vector3(0, FLOORH_ / 2, 0));

					}
				}


			}
		}
	}
	
}


void ProySys::createBall() {
	Particle::visual v;
	v.size = { BALLSIZE_, BALLSIZE_, BALLSIZE_ };
	v.type = Particle::geomType::geomSphere;
	v.color = BALLCOLOR_;
	v.material = gPhysics_->createMaterial(BALLFR_, BALLFR_, BALLRESTIT_);


	Particle::physics p;
	p.pos = INITPOS_;

	// MASS GUARDA LA DENSIDAD
	p.mass = BALLMASS_;
	p.damp = BALLDAMP_;

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
	v.size = { WALLW_ / 2, WALLW_ / 4, WALLW_ / 4 };
	v.type = Particle::geomType::geomBox;
	v.color = SPRINGSSCOLOR_;
	v.material = gPhysics_->createMaterial(0, 0, 0);

	Particle::physics p;
	p.pos = INITPOS_ + Vector3(0, 0, ANCHOROFFSET_.z);
	p.pos.y = FLOORH_ + WALLW_;

	anchor_ = new Particle(v, p, -1);
	particles_.push_back(anchor_);


	v.size = { BALLSIZE_ * 2, WALLW_, WALLW_ };
	v.type = Particle::geomType::geomBox;

	p.damp = 0.5f;
	p.mass = SPRINGMASS_;
	p.pos += Vector3(0, 0, RESTINGLENGTH_);

	spring_ = new Particle(v, p, -1);
	particles_.push_back(spring_);


	// No hace falta añadir la partícula estática a las fuerzas
	throw_ = new SpringForceGenerator(K_, RESTINGLENGTH_, anchor_);
	forces_.insert(throw_);
	partForceReg_->addForce(throw_, spring_);
	throw_->setActive(true);

}

void ProySys::setupExpl() {
	expl_ = new ExplosionForceGenerator(EXPLK_, EXPLR_, EXPLT_, { 0, 0, 0 });
	forces_.insert(expl_);
	partForceReg_->addForce(expl_, ball_);


	Particle* p = new Debris(FLOORCOLOR_, PARTLIFETIME_);
	debrisGen_ = new GaussianParticleGenerator(DEBRGENTIME_, DEBRGENMEAN_, DEBRGENDEV_, DEBRGENOFF_, true, true, true, true);
	debrisGen_->changeModelPart(p);
	debrisGen_->changeGenerateN(DEBRGENN_);
	debrisGen_->setName("debrisGen_");
	generators_.insert({ "debrisGen_", debrisGen_ });
	
	delete p;

	p = new Smoke(PARTLIFETIME_);
	smokeGen_ = new GaussianParticleGenerator(SMOKEGENTIME_, SMOKEGENMEAN_, SMOKEGENDEV_, SMOKEGENOFF_);
	smokeGen_->changeModelPart(p);
	smokeGen_->changeGenerateN(SMOKEGENN_);
	smokeGen_->setName("smokeGen_");
	generators_.insert({ "smokeGen_", smokeGen_ });
	
	delete p;
}



void ProySys::createMap() {
	Vector3 nextPos = { 0,0,0 };
	nextPos.z += -40;
	createFloor({ FLOORW_, FLOORH_, 50 }, nextPos);
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, { 0, 0, 10 });
	createWall({ WALLW_, WALLH_, 50 }, nextPos + Vector3(-FLOORW_, 0, 0));
	createWall({ WALLW_, WALLH_, 50 }, nextPos + Vector3(FLOORW_, 0, 0));

	createPin(nextPos + Vector3(0, 0.01f, 25));
	createPin(nextPos + Vector3(-2, 0.01f, 10));
	createBarrel(nextPos + Vector3(0, 0, -5.0f));
	createPin(nextPos + Vector3(0, 0.01f, -20));
	createPin(nextPos + Vector3(4, 0.01f, -35));

	nextPos.z += -50;
	createFloor({ FLOORW_, FLOORH_, 20 }, nextPos, PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(-FLOORW_, 0, 0), PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(FLOORW_, 0, 0), PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));
	
	createBooster(nextPos + Vector3(0, 5, -7), {0, 0, -1}, PxQuat(PxHalfPi / 3, Vector3(1, 0, 0)));


	nextPos.y += 9.85f;
	nextPos.z += -36.9f;
	createFloor({ FLOORW_, FLOORH_, 20 }, nextPos);
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(-FLOORW_, 0, 0));
	createWall({ WALLW_, WALLH_, 20 }, nextPos + Vector3(FLOORW_, 0, 0));


	createPin(nextPos + Vector3(-2, 0.01f, 10));
	createBarrel(nextPos + Vector3(0, 0, -5.0f));
	createPin(nextPos + Vector3(4, 0.01f, -25));


	nextPos.z += -FLOORW_ * 3;
	createFloor({ FLOORW_, FLOORH_, FLOORW_ }, nextPos);
	createWall({ WALLW_, WALLH_, FLOORW_ }, nextPos + Vector3(FLOORW_, 0, 0));
	createWall({ FLOORW_ + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, -FLOORW_));

	nextPos.x += -FLOORW_ * 3;
	createFloor({ FLOORW_ * 2, FLOORH_, FLOORW_ }, nextPos);
	createWall({ FLOORW_ * 2 + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, -FLOORW_));
	createWall({ FLOORW_ * 2 + WALLW_, WALLH_, WALLW_ }, nextPos + Vector3(0, 0, +FLOORW_));

	createBarrel(nextPos + Vector3(0, 0, 0));


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


}


SRigidBody* ProySys::createFloor(Vector3 dims, Vector3 pos, PxQuat rot) {
	Particle::visual v;
	v.size = dims;
	v.type = Particle::geomType::geomBox;
	v.color = FLOORCOLOR_;
	v.material = gPhysics_->createMaterial(FLOORFR_, FLOORFR_, FLOORRESTIT_);

	Particle::physics p;
	p.pos = pos;

	SRigidBody* floor = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(floor);

	PxTransform tr = PxTransform(floor->getRigidActor()->getGlobalPose().p, rot);
	floor->getRigidActor()->setGlobalPose(tr);

	return floor;
}

void ProySys::createWall(Vector3 dims, Vector3 pos, PxQuat rot) {
	Particle::visual v;
	v.size = dims;
	v.type = Particle::geomType::geomBox;
	v.color = BORDERSCOLOR_;
	v.material = gPhysics_->createMaterial(WALLFR_, WALLFR_, WALLRESTIT_); 

	Particle::physics p;
	p.pos = pos;

	SRigidBody* wall = new SRigidBody(v, p, -1, gPhysics_, gScene_);
	particles_.push_back(wall);

	PxTransform tr = PxTransform(wall->getRigidActor()->getGlobalPose().p, rot);
	wall->getRigidActor()->setGlobalPose(tr);
}

void ProySys::createPin(Vector3 pos) {
	auto pin = new Pin(gPhysics_, gScene_, pos, [&](Pin* cb) {
		// Callback: cuando un bolo llama a esta función, se añade a la 
		// lista de partículas a eliminar y se actualiza la puntuación
		remove_.push_back(cb);
		score_++;
		changeScore(score_);
		pinsFalling_ = false;
		}, [&]() {
			pinsFalling_ = true;
			finishTimer_ = 0;
		}
	);

	particles_.push_back(pin);
}

void ProySys::createBarrel(Vector3 pos) {
	auto barrel = new Barrel(gPhysics_, gScene_, pos);
	particles_.push_back(barrel);
	barrels_.push_back(barrel);
}

void ProySys::createBooster(Vector3 pos, Vector3 dir, PxQuat rot) {
	Booster* boost = new Booster(gPhysics_, gScene_, pos, [=]() {
			if (ball_->getVel().normalize() >= MINVEL_) {
				impulse_->setDir(dir);
				impulse_->setActive(true);
			}
		}, ball_->getRigidActor()
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

void ProySys::createDebris() {
	auto parts = debrisGen_->generateParticles();
	for (auto p : parts) {
		particles_.push_back(p);
		partForceReg_->addForce(expl_, p);
		partForceReg_->addForce(gr_, p);

		// Masa aleatoria entre 1 y 5 veces la masa inicial
		float rndMass = (rand() % 5 + 1) * p->getMass();
		p->setInvMass(1.0f / rndMass);
	}
}
