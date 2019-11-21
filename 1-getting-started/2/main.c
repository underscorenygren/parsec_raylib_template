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

	return 0;
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

	if (game_init(&game)) {
		printf("couldn't initialize game\n");
	}

	if (strcmp(session, DISABLE_PARSEC) == 0) {
		printf("skipping parsec init\n");
	} else {
		if (parsecraylib_init(&parsec, session)) {
			printf("failed to initialize parsec\n");
			return 1;
		}
	}

	while (!WindowShouldClose())
	{
		game_update(&game);
		game_draw(&game);
	}

	parsecraylib_deinit(parsec);
	game_deinit(&game);

	return 0;
}
