#include <stdio.h>
#include <string.h>

#include "libparsecraylib.c"

#define DISABLE_PARSEC "noparsec"

struct Game {
	int screenWidth;
	int screenHeight;
	int fps;
	char *title;
};

void game_init(struct Game *game) {

	game->screenWidth = 800;
	game->screenHeight = 450;
	game->fps = 60;
	game->title = "My Game";

	InitWindow(game->screenWidth,
		game->screenHeight,
		game->title);

	SetTargetFPS(game->fps);
}

void game_update(struct Game *game) {
	//put game logic in here
}

void game_draw(struct Game *game) {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
}

void game_deinit(struct Game *game) {
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

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		game_update(&game);
		game_draw(&game);
			//----------------------------------------------------------------------------------
	}

	parsecraylib_deinit(parsec);
	game_deinit(&game);

	return 0;
}
