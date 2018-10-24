#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

// structs of all components of the pinball (flippers, launchers...etc)

struct light {
	PhysBody* sensor = nullptr;
	bool working = false;
	SDL_Texture* light_tex = nullptr;
};
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	// void Reset(); //A function to reset everything and start again must be here

public:
	p2List<PhysBody*> mapshapes;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	SDL_Texture* map;
	SDL_Texture* red_light;
	SDL_Texture* green_light;


	uint bonus_fx;
	uint music;
};
