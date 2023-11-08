#pragma once

#include "../ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
protected:
	float k1_, k2_;

public:
	WindForceGenerator(const float& k1, const float& k2) : ForceGenerator(), k1_(k1), k2_(k2) { };
	
	inline void setK1(const float& k) { k1_ = k; }
	inline float getK1() const { return k1_; }
	inline void setK2(const float& k) { k2_ = k; }
	inline float getK2() const { return k2_; }

	virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			
		}
	};
};