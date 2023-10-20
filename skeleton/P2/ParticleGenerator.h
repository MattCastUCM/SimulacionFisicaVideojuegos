#pragma once

#include <list>
#include "../P1/Particle.h"
#include <random>

class ParticleGenerator {
protected:
	//std::string name_;		// Nombre del generador
	int generateN_;				// Partículas a generar en cada llamada a generateParticles
	double genProb_;			// Probabilidad de que genere una partícula (1 => siempre genera una)
	Particle* modelPart_;		// Guarda los atributos de la partícula a generar (damping, tiempo de vida, etc.)
	Vector3 origin_, vel_;		// Origen y velocidad de las partículas que se crean

	std::mt19937 mt_;
	double time_, generationTime_;			// Tiempo que ha pasado desde la última generación, tiempo que tarda en generar nuevas partículas
	bool changeX_, changeY_, changeZ_;		// Si el generador cambia la velocidad x, y o z de la partícula al generar partículas nuevas

public:
	ParticleGenerator(/*const std::string& name,*/ double genTime, bool changeX = true, bool changeY = false, bool changeZ = true)
		: /*name_(name),*/ generateN_(3), genProb_(1.0), modelPart_(nullptr), time_(0), generationTime_(genTime),
		changeX_(changeX), changeY_(changeY), changeZ_(changeZ) { };


	inline virtual std::list<Particle*> generateParticles() {
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			generated.push_back(p);
		}

		return generated;
	};

	virtual void setVelocities(Particle* p) { }
	
	inline std::list<Particle*> update(double t) {
		time_ += t; 
		if (time_ >= generationTime_) {
			std::list<Particle*> generated = generateParticles();
			time_ = 0;
			return generated;
		}
		else return { };
	}

	inline void changeModelPart(Particle* p, bool modifyPosVel = true) {
		delete modelPart_;
		modelPart_ = p->clone();
		if (modifyPosVel) {
			origin_ = p->getPos();
			vel_ = p->getVel();
		}
		modelPart_->setPos({ -1000.0f, -1000.0f, -1000.0f });
	}



	inline void setOrigin(const Vector3& p) {
		modelPart_->setInitPos(p);
		origin_ = p;
	}
	inline void setVel(const Vector3& v) { 
		modelPart_->setVel(v);
		vel_ = v; 
	}
	inline void setAcc(const Vector3& acc) {
		modelPart_->setInitAcc(acc);
	}
	inline Vector3 getVel() const { return vel_; }
	inline void changeGenerateN(int n) { generateN_ = n; }
	inline void changeLifetime(double t) { modelPart_->changeLifetime(t); }
	
	
	
	
};