#include <stdio.h>
#include <string.h>

#include "libparsecraylib.c"

#define DISABLE_PARSEC "noparsec"
#define NUM_COLORS 4
#define NUM_SHAPES 3
#define NUM_PLAYERS 8

const Color PlayerColors[NUM_COLORS] = {
	BLUE,
	RED,
	YELLOW,
	GREEN,
};

enum PlayerShape {
	Rect = 0,
	Circle = 1,
	Triangle = 2
};

struct Player {
	ParsecGuest guest;
	bool active;
	int color;
	enum PlayerShape shape;
	bool p_up;
	bool p_down;
	bool p_left;
	bool p_right;
	uint32_t framecounter;
};

struct Game {
	int screenWidth;
	int screenHeight;
	uint32_t fps;
	char *title;
	//texutre rendering
	RenderTexture2D canvas;
	Rectangle src;
	Rectangle dst;
	Vector2 origin;
	float rotation;
	//game state
	Parsec *parsec;
	uint32_t n_guests;
	struct Player players[NUM_PLAYERS];
	uint32_t framecounter;
};

void player_init(struct Player *player, ParsecGuest *guest) {
	struct Player empty = { 0 };
	*player = empty;
	player->active = true;
	if (guest != NULL) {
		player->guest = *guest;
	}
}

void player_deinit(struct Player *player) {
	player->active = false;
}

bool game_is_player_action_blocked(struct Game *game, struct Player *player) {
	int32_t diff = game->framecounter - player->framecounter;
	return abs(diff) < game->fps;
}

void game_process_player_input(struct Game *game, struct Player *player) {
	uint32_t color_inc = 0, shape_inc = 0;
	if (player->p_up) {
		color_inc++;
	}
	if (player->p_down) {
		color_inc--;
	}
	if (player->p_right) {
		shape_inc++;
	}
	if (player->p_left) {
		shape_inc--;
	}

	if (color_inc != 0 || shape_inc != 0) {
	 if (game_is_player_action_blocked(game, player)) {
		 return;
	 } else {
		 player->framecounter = game->framecounter;
	 }
	}

	player->color = (player->color + color_inc) % NUM_COLORS;
	player->shape = (player->shape + shape_inc) % NUM_SHAPES;
}

struct Player *game_add_player(struct Game *game, ParsecGuest *guest) {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		struct Player *player = &game->players[i];
		if (!player->active) {
			player_init(player, guest);
			return player;
		}
	}
	return NULL;
}

int game_init(struct Game *game) {

	struct Game g = { 0 };

	if (game == NULL) {
		return 1;
	}

	*game = g; //initialize to 0

	game->screenWidth = 800;
	game->screenHeight = 450;
	game->fps = 60;
	game->title = "My Game";

	InitWindow(game->screenWidth,
		game->screenHeight,
		game->title);

	SetTargetFPS(game->fps);

	game->canvas = LoadRenderTexture(game->screenWidth, game->screenHeight);

	//We levarage the initialization to 0 to avoid setting 0 values.
	game->src.width = game->canvas.texture.width;
	game->src.height = -game->canvas.texture.height; //renders above origin
	game->dst.width = game->screenWidth;
	game->dst.height = game->screenHeight;

	game_add_player(game, NULL);

	return 0;
}

void game_read_raylib_input(struct Game *game) {
	struct Player *local = &game->players[0];

	local->p_up = IsKeyDown(KEY_UP);
	local->p_down = IsKeyDown(KEY_DOWN);
	local->p_left = IsKeyDown(KEY_LEFT);
	local->p_right = IsKeyDown(KEY_RIGHT);
}

void game_update(struct Game *game) {
	game->framecounter++;
	game->n_guests = parsecraylib_n_guests(game->parsec);
	game_read_raylib_input(game);
	game_process_player_input(game, &game->players[0]);
}

void player_draw(struct Player *player, float x, float y) {
	Color col = PlayerColors[player->color];
	uint32_t width = 20, height = 20;
	Vector2 v = {x, y};
	switch (player->shape) {
		case Rect:
			DrawRectangle(x, y, width, height, col);
			break;
		case Circle:
			DrawCircle(x, y, width, col);
			break;
		case Triangle:
			DrawPoly(v, 3, width, 0, col);
			break;
		default:
			break;
	}
}

void game_draw(struct Game *game) {
	uint32_t size = 100;
	char text[size];
	uint32_t n_players = parsecraylib_n_guests(game->parsec) + 1;
	float offset = game->screenWidth / (n_players + 1);
	uint32_t drawnPlayer = 0;

	BeginTextureMode(game->canvas);

	ClearBackground(RAYWHITE);

	snprintf(text, size, "You are hosting %d guest(s).", game->n_guests);

	DrawText(text, 190, 200, 20, LIGHTGRAY);

	for (int i = 0; i < NUM_PLAYERS; i++) {
		struct Player *player = &game->players[i];
		if (player->active) {
			player_draw(player, offset + offset * drawnPlayer, 300);
			drawnPlayer++;
		}
	}

	EndTextureMode();

	BeginDrawing();

	DrawTexturePro(game->canvas.texture, game->src, game->dst, game->origin, game->rotation, WHITE);

	EndDrawing();
}

void game_deinit(struct Game *game) {
	UnloadRenderTexture(game->canvas);
	CloseWindow();
}

int main(int argc, char** argv)
{
	struct Game game = { 0 };
	char *session = NULL;

	if (argc < 2) {
		printf("Usage: ./ [session-id]\n");
		return 1;
	}

	session = argv[1];

	game_init(&game);

	if (strcmp(session, DISABLE_PARSEC) == 0) {
		printf("skipping parsec init");
	} else {
		if (parsecraylib_init(&game.parsec, session)) {
			printf("failed to initialize parsec");
			return 1;
		}
	}

	while (!WindowShouldClose())
	{
		game_update(&game);
		game_draw(&game);
		parsecraylib_submit_frame(game.parsec, game.canvas.texture.id);
	}

	parsecraylib_deinit(game.parsec);
	game_deinit(&game);

	return 0;
}
