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
	
	p2List<PhysBody*> green_lights;
	p2List<PhysBody*> red_lights;
	p2List<PhysBody*> coins;
	
	PhysBody* leftflipper;
	PhysBody* rightflipper;
	PhysBody* leftflipper_joint;
	PhysBody* rightflipper_joint;
	PhysBody* leftflippersmall;
	PhysBody* rightflippersmall;
	PhysBody* leftflippersmall_joint;
	PhysBody* rightflippersmall_joint;
	
	PhysBody* green_light;
	PhysBody* red_light;
	PhysBody* coin;

	SDL_Texture* map;
	SDL_Texture* red_light_tex;
	SDL_Texture* green_light_tex;
	SDL_Texture* leftflipper_tex;
	SDL_Texture* rightflipper_tex;
	SDL_Texture* leftflippersmall_tex;
	SDL_Texture* rightflippersmall_tex;
	SDL_Texture* coin_tex;
	SDL_Texture* graphics = nullptr;

	Animation* current_animation = nullptr;
	Animation touched_green_anim;
	Animation touched_red_anim;
	Animation touched_coin_anim;
	Animation coin_anim;
	Animation green_light_anim;
	Animation red_light_anim;
	Animation banner_anim;
		
	bool restart;

	uint bonus_fx;
	uint music;
};
