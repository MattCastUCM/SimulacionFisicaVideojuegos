#pragma once

#include "../ForceGenerator.h"
class WindForceGenerator : public ForceGenerator {
protected:
	float k1_, k2_;
	Vector3 windVel_, minPos_, maxPos_;

public:
	WindForceGenerator(Vector3 windVel, const float& k1, const float& k2 = 0, Vector3 minPos = { -1000, -1000, -1000 }, Vector3 maxPos = { 1000, 1000, 1000 })
		: ForceGenerator(), windVel_(windVel), k1_(k1), k2_(k2), minPos_(minPos), maxPos_(maxPos) { };
	
	inline void setK1(const float& k) { k1_ = k; }
	inline float getK1() const { return k1_; }
	inline void setK2(const float& k) { k2_ = k; }
	inline float getK2() const { return k2_; }

	inline void setVel(const Vector3& vel) { windVel_ = vel; }
	inline Vector3 getVel() { return windVel_; }

	inline virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			auto pos = p->getPos();
			if (pos.x >= minPos_.x && pos.x <= maxPos_.x
				&& pos.y >= minPos_.y && pos.y <= maxPos_.y
				&& pos.z >= minPos_.z && pos.z <= maxPos_.z)
			{
				auto partVel = p->getVel();
				auto windPartDiff = windVel_ - partVel;
				Vector3 f = k1_ * windPartDiff + k2_ * windPartDiff.magnitude() * windPartDiff;

				p->addForce(f);
			}
		}
	};



};