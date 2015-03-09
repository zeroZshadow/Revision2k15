#include "audioutil.h"

#include <aesndlib.h>
#include <gcmodplay.h>

static MODPlay play;

void AU_init() {
	AESND_Init(NULL);
	AESND_Pause(FALSE);
	//WARNING, NEEDS A FRAME OF TIME BEFORE YOU CAN PLAY MUSIC
}

void AU_playMusic(const void* music) {
	MODPlay_Init(&play);
	MODPlay_SetMOD(&play, music);
	MODPlay_Start(&play);
}