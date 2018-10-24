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

	lifes = 4;
	finished_ball = false;
	score = 0;
	highest_score = 0;

	initialBallPosition_x = SCREEN_WIDTH - 32;
	initialBallPosition_y = SCREEN_HEIGHT - 600;

	ball = App->physics->CreateCircle(initialBallPosition_x, initialBallPosition_y, 12);

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
	//+ debug function
	if (/*App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN ||*/ finished_ball == true )
	{
		if (ball != nullptr) {
			ball->body->GetWorld()->DestroyBody(ball->body);
			ball = nullptr;
		}

		// Appears the new ball
		ball = App->physics->CreateCircle(initialBallPosition_x, initialBallPosition_y, 12);
		finished_ball = false;
		lifes--;

		// games has finished completely
		if (lifes <= 0) 
		{
			App->scene_intro->restart = true;

		}
		else {
			App->audio->PlayFx(App->audio->ball_falls, 0);
		}
		
	}

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

	// Updating the game info on the window title
	p2SString game_info("BALLS: %d  ||  SCORE: %d  ||  HIGHEST SCORE: %d", lifes, score, highest_score);
	App->window->SetTitle(game_info.GetString());

	return UPDATE_CONTINUE;
}



