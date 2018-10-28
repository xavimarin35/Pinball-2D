#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "Animation.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	map = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	graphics = App->textures->Load("assets/textures/animations_spritesheet.png");

	map = App->textures->Load("assets/textures/background.png"); 
	red_light_tex = App->textures->Load("assets/textures/Red.png");
	green_light_tex = App->textures->Load("assets/textures/Green.png");
	leftflipper_tex = App->textures->Load("assets/textures/Flipper_LeftBig.png");
	rightflipper_tex = App->textures->Load("assets/textures/Flipper_RightBig.png");
	leftflippersmall_tex = App->textures->Load("assets/textures/Flipper_LeftSmall.png");
	rightflippersmall_tex = App->textures->Load("assets/textures/Flipper_RightSmall.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	b2RevoluteJointDef jointDef_1;
	b2RevoluteJointDef jointDef_2;
	b2RevoluteJoint* joint_1;
	b2RevoluteJoint* joint_2;
	b2RevoluteJointDef jointDefsmall_1;
	b2RevoluteJointDef jointDefsmall_2;
	b2RevoluteJoint* jointsmall_1;
	b2RevoluteJoint* jointsmall_2;

	// Green light touched:

	touched_green_anim.PushBack({ 0,0,300,300 });
	touched_green_anim.PushBack({ 300,0,300,300 });
	touched_green_anim.PushBack({ 600,0,300,300 });
	touched_green_anim.PushBack({ 900,0,300,300 });
	touched_green_anim.PushBack({ 1200,0,300,300 });
	touched_green_anim.PushBack({ 1500,0,300,300 });
	touched_green_anim.speed = 0.3f;
	touched_green_anim.loop = false;

	// Red light touched:

	touched_red_anim.PushBack({ 0,300,300,300 });
	touched_red_anim.PushBack({ 300,300,300,300 });
	touched_red_anim.PushBack({ 600,300,300,300 });
	touched_red_anim.PushBack({ 900,300,300,300 });
	touched_red_anim.PushBack({ 1200,300,300,300 });
	touched_red_anim.PushBack({ 1500,300,300,300 });
	touched_red_anim.loop = false;

	// Coin touched:

	touched_coin_anim.PushBack({ 1800,0,250,250 });
	touched_coin_anim.PushBack({ 1800,250,250,250 });
	touched_coin_anim.PushBack({ 1800,500,250,250 });
	touched_coin_anim.PushBack({ 2050,0,250,250 });
	touched_coin_anim.PushBack({ 2050,250,250,250 });
	touched_coin_anim.PushBack({ 2050,500,250,250 });
	touched_coin_anim.loop = false;

	// Coin:

	coin_anim.PushBack({ 0,722,27,28 });
	coin_anim.PushBack({ 28,721,21,29 });
	coin_anim.PushBack({ 50,721,5,29 });
	coin_anim.PushBack({ 56,715,21,35 });
	coin_anim.PushBack({ 77,719,27,31 });
	coin_anim.PushBack({ 106,715,27,21 });
	coin_anim.PushBack({ 127,721,5,29 });
	coin_anim.PushBack({ 133,721,21,29 });
	coin_anim.loop = true;

	// Green light:

	green_light_anim.PushBack({ 282,715,35,33 });
	green_light_anim.PushBack({ 0,0,0,0 });
	green_light_anim.loop = true;

	// Red light:

	red_light_anim.PushBack({ 444,707,42,43 });
	red_light_anim.PushBack({ 0,0,0,0 });
	red_light_anim.loop = true;

	// Banner:

	banner_anim.PushBack({ 2,754,428,240 });
	banner_anim.PushBack({ 430,754,428,240 });
	banner_anim.loop = true;

	// Shapes extracted from Ric's Shapes Creator!!
	int pinball_externalshape[92] = {
	434, 773,
	433, 825,
	294, 861,
	294, 872,
	493, 874,
	491, 0,
	1, 0,
	3, 875,
	203, 877,
	201, 862,
	11, 807,
	10, 602,
	15, 590,
	22, 579,
	32, 574,
	41, 572,
	52, 570,
	58, 572,
	58, 560,
	61, 549,
	67, 544,
	74, 539,
	81, 536,
	87, 534,
	93, 533,
	10, 275,
	11, 227,
	17, 189,
	38, 131,
	64, 94,
	111, 49,
	149, 27,
	206, 8,
	252, 5,
	309, 15,
	364, 35,
	393, 55,
	429, 92,
	458, 133,
	475, 175,
	484, 214,
	484, 266,
	485, 339,
	485, 864,
	438, 864,
	439, 773
	};

	int leftdown_obstacle[14] = {
	57, 639,
	63, 638,
	65, 751,
	146, 777,
	146, 797,
	55, 772,
	55, 645
	};

	int leftdown_bouncer[16] = {
	113, 634,
	111, 709,
	114, 717,
	124, 722,
	136, 727,
	146, 730,
	152, 728,
	116, 639
	};

	int rightdown_obstacle[44] = {
	337, 778,
	339, 799,
	438, 772,
	439, 236,
	435, 208,
	425, 175,
	414, 149,
	395, 122,
	376, 103,
	343, 78,
	341, 85,
	377, 114,
	397, 139,
	413, 163,
	424, 193,
	431, 221,
	432, 254,
	430, 285,
	378, 442,
	431, 465,
	432, 752,
	341, 777
	};

	int rightdown_bouncer[18] = {
	385, 631,
	345, 729,
	353, 728,
	363, 725,
	371, 722,
	380, 718,
	385, 708,
	385, 691,
	386, 637
	};

	int lefttop_path[52] = {
	140, 525,
	145, 521,
	66, 275,
	65, 253,
	65, 230,
	68, 210,
	73, 189,
	80, 170,
	90, 152,
	103, 133,
	118, 115,
	134, 101,
	153, 90,
	148, 83,
	124, 100,
	109, 114,
	94, 133,
	81, 154,
	73, 174,
	65, 194,
	61, 216,
	57, 239,
	58, 259,
	61, 283,
	136, 517,
	137, 522
	};

	int top_path[102] = {
	370, 294,
	377, 297,
	381, 280,
	384, 265,
	385, 243,
	384, 221,
	380, 203,
	370, 182,
	361, 166,
	349, 153,
	335, 139,
	320, 127,
	303, 118,
	283, 112,
	263, 108,
	241, 107,
	223, 109,
	206, 113,
	189, 121,
	173, 130,
	161, 139,
	150, 148,
	141, 157,
	133, 169,
	126, 182,
	118, 197,
	126, 199,
	135, 181,
	146, 165,
	156, 151,
	170, 141,
	183, 133,
	196, 126,
	213, 120,
	230, 115,
	248, 115,
	265, 116,
	282, 120,
	297, 124,
	312, 132,
	323, 140,
	336, 151,
	350, 164,
	358, 177,
	367, 195,
	373, 212,
	377, 230,
	378, 250,
	376, 267,
	373, 281,
	371, 290
	};


	int rocket_launcher[10] = {
	440, 771,
	484, 771,
	484, 861,
	440, 862,
	440, 777
	};

	restart = false;

	//creating map shapes
	mapshapes.add(App->physics->CreateChain(0, 0, pinball_externalshape, 92, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, leftdown_obstacle, 14, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, leftdown_bouncer, 16, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rightdown_obstacle, 44, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rightdown_bouncer, 18, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, lefttop_path, 48, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, top_path, 94, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rocket_launcher, 10, b2_staticBody));

	// green ball (RADIUS = 16)

	// Flippers stuff
	rightflipper = App->physics->CreateRectangle(295, 785, 70, 8, b2_dynamicBody);
	leftflipper = App->physics->CreateRectangle(190, 785, 70, 8, b2_dynamicBody);
	rightflippersmall = App->physics->CreateRectangle(342, 301, 55, 8, b2_dynamicBody);
	leftflippersmall = App->physics->CreateRectangle(174, 532, 55, 8, b2_dynamicBody);

	rightflipper_joint = App->physics->CreateCircle(330, 785, 5, b2_staticBody, false);
	leftflipper_joint = App->physics->CreateCircle(155, 785, 5, b2_staticBody, false);
	rightflippersmall_joint = App->physics->CreateCircle(370, 301, 5, b2_staticBody, false);
	leftflippersmall_joint = App->physics->CreateCircle(147, 532, 5, b2_staticBody, false);
	
	jointDef_1.Initialize(leftflipper->body, leftflipper_joint->body, leftflipper_joint->body->GetWorldCenter());
	jointDef_2.Initialize(rightflipper_joint->body, rightflipper->body, rightflipper_joint->body->GetWorldCenter());
	jointDefsmall_1.Initialize(leftflippersmall->body, leftflippersmall_joint->body, leftflippersmall_joint->body->GetWorldCenter());
	jointDefsmall_2.Initialize(rightflippersmall_joint->body, rightflippersmall->body, rightflippersmall_joint->body->GetWorldCenter());

	jointDef_1.lowerAngle = -0.12f * b2_pi;
	jointDef_1.upperAngle = 0.25f * b2_pi;
	jointDef_2.lowerAngle = -0.12f * b2_pi;
	jointDef_2.upperAngle = 0.25f * b2_pi;
	jointDefsmall_1.lowerAngle = -0.12f * b2_pi;
	jointDefsmall_1.upperAngle = 0.25f * b2_pi;
	jointDefsmall_2.lowerAngle = -0.12f * b2_pi;
	jointDefsmall_2.upperAngle = 0.25f * b2_pi;

	jointDef_1.enableLimit = true;
	jointDef_2.enableLimit = true;
	jointDefsmall_1.enableLimit = true;
	jointDefsmall_2.enableLimit = true;

	joint_1 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDef_1);
	joint_2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDef_2);
	jointsmall_1 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDefsmall_1);
	jointsmall_2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDefsmall_2);

	//lights stuff
	/*green_lights_list.add(new greenlight);
	green_lights_list.getLast()->data->sensor_green = App->physics->CreateCircle(242, 39, 16, b2_staticBody, true);
	green_lights_list.getLast()->data->texture_green = green_light_tex;*/

	//lights stuff
	sensorgreenup2 = new PhysBody();
	sensorgreenup2 = App->physics->CreateRectangleSensor(240, 39, 22, 22);
	sensorgreenup2->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//(b2RevoluteJoint*)App->physics->world->DestroyJoint(&jointDef_1);
	//(b2RevoluteJoint*)App->physics->world->DestroyJoint(&jointDef_2);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	int x, y;

	//Blit map
	App->renderer->Blit(map, 0, 0);
	
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	//Lights system
	if (sensorgreenup2_active == true) 
	{
		SDL_Rect greenRect = current_green_anim->GetCurrentFrame();
		App->renderer->Blit(graphics, 91, -109, &greenRect, NULL);
	}
	touched_green_anim.Finished();

	// All draw functions ------------------------------------------------------
	if (leftflipper != NULL)
	{
		int x, y;
		leftflipper->GetPosition(x, y);
		App->renderer->Blit(leftflipper_tex, x, y, NULL, 1.0f, leftflipper->GetRotation());
	}
	
	if (rightflipper != NULL)
	{
		int x, y;
		rightflipper->GetPosition(x, y);
		App->renderer->Blit(rightflipper_tex, x, y, NULL, 1.0f, rightflipper->GetRotation());
	}

	if (leftflippersmall != NULL)
	{
		int x, y;
		leftflippersmall->GetPosition(x, y);
		App->renderer->Blit(leftflippersmall_tex, x, y, NULL, 1.0f, leftflippersmall->GetRotation());
	}
	if (rightflippersmall != NULL)
	{
		int x, y;
		rightflippersmall->GetPosition(x, y);
		App->renderer->Blit(rightflippersmall_tex, x, y, NULL, 1.0f, rightflippersmall->GetRotation());
	}



	// restart game
	if (restart == true)
	{
		restartGame();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyA == sensorgreenup2)
	{
		sensorgreenup2_active = true;
		App->player->score += 30;
		current_green_anim = &touched_green_anim;
	}

	if (bodyA != sensorgreenup2) 
	{
		sensorgreenup2_active == false;
		current_green_anim = &touched_green_anim;
	}
}

void ModuleSceneIntro::restartGame() {

	//banner
	//scene of the map Unload
	//scene of the banner Load


	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)*/
	{
		App->audio->PlayFx(App->audio->restart);

		App->player->lifes = 4;
		App->player->score = 0;
		App->player->ball_launched = false;
		restart = false;

	}
}