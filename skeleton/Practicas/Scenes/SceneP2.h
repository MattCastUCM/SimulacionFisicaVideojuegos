#pragma once

class ParticleSystem; 

class SceneP2 {
private:
	ParticleSystem* water_;
	ParticleSystem* fireworks_;
	ParticleSystem* mist_;


public:
	SceneP2();
	~SceneP2();

	void update(double t);
	void keyPress(unsigned char key) { };
};

