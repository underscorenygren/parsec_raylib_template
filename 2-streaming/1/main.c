#include <stdio.h>
#include <string.h>

#include "libparsecraylib.c"

#define DISABLE_PARSEC "noparsec"

struct Game {
	int screenWidth;
	int screenHeight;
	int fps;
	char *title;
	//texutre rendering
	RenderTexture2D canvas;
	Rectangle src;
	Rectangle dst;
	Vector2 origin;
	float rotation;
};

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

	return 0;
}

void game_update(struct Game *game) {
	//put game logic in here
}

void game_draw(struct Game *game) {
	BeginTextureMode(game->canvas);

	ClearBackground(RAYWHITE);

	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

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
	Parsec *parsec = NULL;

	if (argc < 2) {
		printf("Usage: ./ [session-id]\n");
		return 1;
	}

	session = argv[1];

	game_init(&game);

	if (strcmp(session, DISABLE_PARSEC) == 0) {
		printf("skipping parsec init");
	} else {
		if (parsecraylib_init(&parsec, session)) {
			printf("failed to initialize parsec");
			return 1;
		}
	}

	while (!WindowShouldClose())
	{
		game_update(&game);
		game_draw(&game);
		parsecraylib_submit_frame(parsec, game.canvas.texture.id);
	}

	parsecraylib_deinit(parsec);
	game_deinit(&game);

	return 0;
}
