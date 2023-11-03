#pragma once

#include "../ParticleSystem.h"
#include "../Particles/Firework.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"
#include "../ParticleGenerators/CircleGenerator.h"
#include "../ParticleGenerators/HeartGenerator.h"


#include <time.h>       /* time */
class FireworksSystem : public ParticleSystem {
public:
	FireworksSystem (const Vector3& g = { 0.0f, -10.0f, 0.0f }) : ParticleSystem(g) {
		srand(time(NULL));
		
		Firework* shotF = new Firework(g, 0, 0, 3.0);

		
		GaussianParticleGenerator* sphereGen = new GaussianParticleGenerator(0, 0, 0.1, 0, true, true, true, true);
		sphereGen->changeGenerateN(50);
		sphereGen->setName("sphereGen");
		generators_.insert({ "sphereGen", sphereGen });
		shotF->addGenerator(sphereGen);

		CircleGenerator* circleGen = new CircleGenerator(0, 0, 0.05, 0, true);
		circleGen->changeGenerateN(10);
		circleGen->setName("circleGen");
		generators_.insert({ "circleGen", circleGen });
		shotF->addGenerator(circleGen);

		HeartGenerator* heartGen = new HeartGenerator(0, 0, 0.05, true);
		heartGen->changeGenerateN(50);
		heartGen->setName("heartGen");
		generators_.insert({ "heartGen", heartGen });
		shotF->addGenerator(heartGen);



		GaussianParticleGenerator* fireworkShooter = new GaussianParticleGenerator(5.0, 0, 0.1);
		fireworkShooter->changeModelPart(shotF);
		fireworkShooter->changeGenerateN(1);
		generators_.insert({ "fireworkShooter", fireworkShooter });
		fireworkShooter->setOrigin({ 0.0f, -60.0f, -150.0f });


		delete shotF;
	}


};