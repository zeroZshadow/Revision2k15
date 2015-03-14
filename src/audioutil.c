#include "audioutil.h"

#include <asndlib.h>

#include "oggplayer.h"

void AU_init() {
	ASND_Init(NULL);
	//WARNING, NEEDS A FRAME OF TIME BEFORE YOU CAN PLAY MUSIC
}

void AU_playMusic(const void* music, const s32 size) {
	PlayOgg(music, size, 0, OGG_ONE_TIME);
}

void AU_stopMusic() {
	StopOgg();
}