#pragma once

#include "WindForceGenerator.h"

class TornadoForceGenerator : public WindForceGenerator {
protected:
	Vector3 origin_;
	float k_;

public:
	TornadoForceGenerator(Vector3 origin, Vector3 windVel, const float& k,
		const float& k1, const float& k2 = 0, Vector3 minPos = { -1000, -1000, -1000 }, Vector3 maxPos = { 1000, 1000, 1000 })
		: WindForceGenerator(windVel, k1, k2, minPos, maxPos), origin_(origin), k_(k) { };
	
	inline void setK(const float& k) { k_ = k; }
	inline float getK() const { return k_; }

	inline virtual void update(Particle* p, double t) { 
		if (fabs(p->getInvMass()) < 1e-10) return;
		else {
			auto pos = p->getPos();
			if (pos.x >= minPos_.x && pos.x <= maxPos_.x
				&& pos.y >= minPos_.y && pos.y <= maxPos_.y
				&& pos.z >= minPos_.z && pos.z <= maxPos_.z)
			{
				Vector3 windVel;
				windVel.x = -(pos.z - origin_.z);
				windVel.y = 50 - pos.y - origin_.y;
				windVel.z = pos.x - origin_.x;

				windVel *= k_;

				auto partVel = p->getVel();
				auto windPartDiff = windVel - partVel;
				Vector3 f = k1_ * windPartDiff + k2_ * windPartDiff.magnitude() * windPartDiff;

				p->addForce(f);
			}
		}
	};
};