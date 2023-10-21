#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator(/*const std::string& name,*/ double genTime, bool changeX, bool changeY, bool changeZ)
	: /*name_(name),*/ generateN_(3), genProb_(1.0), modelPart_(nullptr), time_(0), generationTime_(genTime), changeX_(changeX), changeY_(changeY), changeZ_(changeZ), active_(true) { };


std::list<Particle*> ParticleGenerator::generateParticles() {
	std::list<Particle*> generated;
	Particle* p;
	for (int i = 0; i < generateN_; i++) {
		p = modelPart_->clone();
		generated.push_back(p);
	}

	return generated;
};


std::list<Particle*> ParticleGenerator::update(double t) {
	time_ += t;
	if (time_ >= generationTime_ && active_) {
		std::list<Particle*> generated = generateParticles();
		time_ = 0;
		return generated;
	}
	else return { };
}

void ParticleGenerator::changeModelPart(Particle* p, bool modifyPosVel) {
	delete modelPart_;
	modelPart_ = p->clone();
	if (modifyPosVel) {
		origin_ = p->getInitPos();
		vel_ = p->getInitVel();
	}
	modelPart_->setPos({ -1000.0f, -1000.0f, -1000.0f });
}