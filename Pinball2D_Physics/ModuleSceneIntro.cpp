#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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
	red_light = App->textures->Load("assets/textures/Red.png");
	green_light = App->textures->Load("assets/textures/Green.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");


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

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}
