#pragma once

#include <list>
#include "../P1/Particle.h"
#include <random>

class ParticleGenerator {
protected:
	std::string name_;			// Nombre del sistema
	int generateN_;				// Partículas a generar en cada llamada a generateParticles
	double genProb_;			// Probabilidad de que genere una partícula (1 => siempre genera una)
	Particle* modelPart_;		// Guarda los atributos de la partícula a generar (damping, tiempo de vida, etc.)
	Vector3 origin_, vel_;		// Origen y velocidad de las partículas que se crean

	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ 0, 1 };


public:
	ParticleGenerator(const std::string& name) 
		: name_(name), generateN_(3), genProb_(1.0), modelPart_(nullptr) { };

	virtual std::list<Particle*> generateParticles() { 
		std::list<Particle*> generated;
		Particle* p;
		for (int i = 0; i < generateN_; i++) {
			p = modelPart_->clone();
			generated.push_back(p);
		}

		return generated;
	};

	inline void setOrigin(const Vector3& p) {
		modelPart_->setInitPos(p);
		origin_ = p;
	}
	inline void setVel(const Vector3& v) { 
		modelPart_->setVel(v);
		vel_ = v; 
	}
	inline void setAcc(const Vector3& acc) {
		modelPart_->setAcc(acc);
	}
	inline Vector3 getVel() const { return vel_; }
	
	inline void changeGenerateN(int n) { generateN_ = n; }

	inline void changeLifetime(double t) { modelPart_->changeLifetime(t); }
	
	inline void changeModelPart(Particle* p, bool modifyPosVel = true) {
		delete modelPart_;
		modelPart_ = p->clone();
		if (modifyPosVel) {
			origin_ = p->getPos();
			vel_ = p->getVel();
		}
		// ??? modelPart_->setPos({ -1000.0f, -1000.0f, -1000.0f });
	}



	
};