#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "parsec.h"

#define DISABLE_PARSEC "noparsec"

int main(int argc, char** argv)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

		char *session = NULL;
    Parsec *parsec = NULL;

		if (argc < 2) {
			printf("Usage: ./ [session-id]\n");
			return 1;
		}

		session = argv[1];

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

		if (strcmp(session, DISABLE_PARSEC) == 0) {
			printf("skipping parsec init");
		} else {
      if (PARSEC_OK != ParsecInit(PARSEC_VER, NULL, NULL, &parsec)) {
        printf("Could not init Parsec");
        return 1;
      }

      if (PARSEC_OK != ParsecHostStart(parsec, HOST_GAME, NULL, session)) {
        printf("Could not start hosting Parsec");
        return 1;
      }
    }

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();        // Close window and OpenGL context
		if (parsec != NULL) {
			ParsecHostStop(parsec);
		}
    ParsecDestroy(parsec);

    return 0;
}


