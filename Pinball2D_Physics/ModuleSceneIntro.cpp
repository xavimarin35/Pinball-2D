#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

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

	int leftdown_obstacle[12] = {
	58, 639,
	57, 771,
	182, 807,
	177, 787,
	64, 751,
	65, 640
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

	int rightdown_obstacle[56] = {
	303, 792,
	304, 808,
	438, 771,
	439, 243,
	438, 223,
	435, 203,
	427, 180,
	420, 162,
	409, 141,
	397, 125,
	384, 110,
	366, 95,
	344, 79,
	341, 87,
	367, 104,
	384, 121,
	399, 140,
	409, 156,
	416, 170,
	423, 191,
	429, 212,
	433, 232,
	432, 263,
	426, 297,
	377, 443,
	433, 465,
	432, 749,
	309, 791
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

	int lefttop_path[48] = {
	144, 536,
	150, 533,
	69, 286,
	65, 270,
	65, 245,
	65, 219,
	71, 194,
	81, 167,
	92, 147,
	107, 127,
	123, 111,
	141, 95,
	151, 89,
	148, 82,
	120, 102,
	92, 134,
	80, 154,
	69, 179,
	61, 205,
	58, 229,
	57, 252,
	60, 278,
	64, 298,
	143, 535
	};

	int top_path[94] = {
	373, 305,
	367, 303,
	371, 288,
	375, 275,
	377, 261,
	378, 243,
	375, 218,
	368, 194,
	356, 172,
	340, 153,
	320, 138,
	300, 127,
	279, 119,
	258, 115,
	235, 115,
	217, 119,
	202, 123,
	187, 130,
	177, 137,
	164, 146,
	155, 156,
	143, 169,
	136, 181,
	128, 194,
	125, 200,
	118, 197,
	127, 179,
	137, 162,
	151, 147,
	170, 131,
	191, 120,
	211, 113,
	228, 109,
	245, 107,
	271, 109,
	289, 114,
	312, 123,
	331, 136,
	349, 152,
	363, 170,
	373, 189,
	380, 210,
	384, 234,
	385, 252,
	383, 269,
	380, 284,
	375, 301
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
	mapshapes.add(App->physics->CreateChain(0, 0, leftdown_obstacle, 12, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, leftdown_bouncer, 16, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rightdown_obstacle, 56, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rightdown_bouncer, 18, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, lefttop_path, 48, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, top_path, 94, b2_staticBody));
	mapshapes.add(App->physics->CreateChain(0, 0, rocket_launcher, 10, b2_staticBody));

	// Flippers stuff
	rightflipper = App->physics->CreateRectangle(250, 622, 70, 15, b2_dynamicBody);
	leftflipper = App->physics->CreateRectangle(210, 540, 70, 15, b2_dynamicBody);
	rightflippersmall = App->physics->CreateRectangle(360, 300, 55, 8, b2_dynamicBody);
	leftflippersmall = App->physics->CreateRectangle(160 + 40, 550, 55, 8, b2_dynamicBody);

	rightflipper_joint = App->physics->CreateCircle(205, 622, 5, b2_staticBody);
	leftflipper_joint = App->physics->CreateCircle(145+19, 550, 5, b2_staticBody);
	rightflippersmall_joint = App->physics->CreateCircle(364, 300, 5, b2_staticBody); 
	leftflippersmall_joint = App->physics->CreateCircle(160, 531, 5, b2_staticBody);
	
	jointDef_1.Initialize(leftflipper->body, leftflipper_joint->body, leftflipper_joint->body->GetWorldCenter());
	jointDef_2.Initialize(rightflipper_joint->body, rightflipper->body, rightflipper_joint->body->GetWorldCenter());
	jointDefsmall_1.Initialize(leftflippersmall->body, leftflippersmall_joint->body, leftflippersmall_joint->body->GetWorldCenter());
	jointDefsmall_2.Initialize(rightflippersmall_joint->body, rightflippersmall->body, rightflippersmall_joint->body->GetWorldCenter());

	jointDef_1.lowerAngle = -0.5f * b2_pi;
	jointDef_1.upperAngle = 0.25f * b2_pi;
	jointDef_2.lowerAngle = -0.5f * b2_pi;
	jointDef_2.upperAngle = 0.25f * b2_pi;
	jointDefsmall_1.lowerAngle = -0.5f * b2_pi;
	jointDefsmall_1.upperAngle = 0.25f * b2_pi;
	jointDefsmall_2.lowerAngle = -0.5f * b2_pi;
	jointDefsmall_2.upperAngle = 0.25f * b2_pi;

	jointDef_1.enableLimit = true;
	jointDef_2.enableLimit = true;
	jointDefsmall_1.enableLimit = true;
	jointDefsmall_2.enableLimit = true;

	joint_1 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDef_1);
	joint_2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDef_2);
	jointsmall_1 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDefsmall_1);
	jointsmall_2 = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointDefsmall_2);

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
	//Blit map
	App->renderer->Blit(map, 0, 0);
	
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

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
	App->audio->PlayFx(bonus_fx);
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

		restart = false;

	}
}