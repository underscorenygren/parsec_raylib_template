#ifndef PARSECRAYLIB_C
#define PARSECRAYLIB_C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"
#include "parsec.h"

/* Initializes parsec. returns true on failure, false othwerwise. */
bool parsecraylib_init(Parsec **parsec, char *session) {
	if (PARSEC_OK != ParsecInit(PARSEC_VER, NULL, NULL, parsec)) {
		return true;
	}

	if (PARSEC_OK != ParsecHostStart(*parsec, HOST_GAME, NULL, session)) {
		return true;
	}

	return false;
}

void parsecraylib_deinit(Parsec *parsec) {
	if (parsec != NULL) {
		ParsecHostStop(parsec);
	}
	ParsecDestroy(parsec);
}

#endif
