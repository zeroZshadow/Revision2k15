#include "demo.h"

#include <malloc.h>
#include <gccore.h>

//Structures
#include "audioutil.h"
#include "textures.h"
#include "model.h"
#include "font.h"
#include "object.h"

//Data
#include "symbol_bmb.h"
#include "music_mod.h"

/* Data vars */
model_t *modelSymbol;
object_t *objectSymbol;
GXTexObj terrainTexObj, fontTexObj;
font_t* font;

void DEMO_init() {
	//Textures
	GXU_loadTexture(terrainTex, &terrainTexObj);
	GXU_loadTexture(ubuntuFontTex, &fontTexObj);

	//Meshes
	modelSymbol = MODEL_setup(symbol_bmb);

	//Objects
	objectSymbol = OBJECT_create(modelSymbol);

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256);

	//Start music
	AU_playMusic(music_mod);
}

void DEMO_update() {
}

void DEMO_render() {
	GXRModeObj* rmode = GXU_getMode();
	FONT_draw(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth / 2, rmode->viHeight - 200, TRUE);
}
