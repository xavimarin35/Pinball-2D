#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

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

	void restartGame(); //A function to reset everything and start again must be here

public:
	p2List<PhysBody*> mapshapes;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* leftflipper;
	PhysBody* rightflipper;
	PhysBody* leftflipper_joint;
	PhysBody* rightflipper_joint;
	PhysBody* leftflippersmall;
	PhysBody* rightflippersmall;
	PhysBody* leftflippersmall_joint;
	PhysBody* rightflippersmall_joint;

	SDL_Texture* map;
	SDL_Texture* red_light_tex;
	SDL_Texture* green_light_tex;
	SDL_Texture* leftflipper_tex;
	SDL_Texture* rightflipper_tex;
	SDL_Texture* leftflippersmall_tex;
	SDL_Texture* rightflippersmall_tex;
	SDL_Texture* coin_tex;
	
	
	bool restart;

	uint bonus_fx;
	uint music;
};
