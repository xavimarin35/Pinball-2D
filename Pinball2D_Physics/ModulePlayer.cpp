#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "ModuleWindow.h"

#include "Box2D/Box2D/Box2D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	ball_tex = App->textures->Load("assets/textures/Ball.png");

	lifes = 3;
	dead = false;
	score = 0;
	highest_score = 0;

	ball = App->physics->CreateCircle(SCREEN_WIDTH - 32, SCREEN_HEIGHT - 600, 12);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(ball_tex);



	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (ball != nullptr) {

		int ballpos_x;
		int ballpos_y;
		
		ball->GetPosition(ballpos_x, ballpos_y);
		App->renderer->Blit(ball_tex, ballpos_x, ballpos_y, NULL);
	}

	// funtion of highscore
	if (score > highest_score) {
		highest_score = score;
	}

	return UPDATE_CONTINUE;
}



