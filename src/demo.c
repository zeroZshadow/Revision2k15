#include "demo.h"

#include <malloc.h>
#include <gccore.h>

//Structures
#include "audioutil.h"
#include "textures.h"
#include "model.h"
#include "font.h"
#include "object.h"
#include "sprite.h"

//Data
#include "symbol_bmb.h"
#include "music_mod.h"

/* Data vars */
model_t *modelSymbol;
object_t *objectSymbol;
sprite_t* spriteBackground;
GXTexObj whiteTexObj, backgroundTexObj, fontTexObj;
font_t* font;

/* Light */
static GXColor lightColor[] = {
	{ 0xF0, 0xF0, 0xF0, 0xff }, /* Light color   */
	{ 0xB0, 0xB0, 0xB0, 0xff }, /* Ambient color */
	{ 0xFF, 0xFF, 0xFF, 0xff }  /* Mat color     */
};

void DEMO_init() {
	//Textures
	GXU_loadTexture(whiteTex, &whiteTexObj);
	GXU_loadTexture(backgroundTex, &backgroundTexObj);
	GXU_loadTexture(ubuntuFontTex, &fontTexObj);

	//Meshes
	modelSymbol = MODEL_setup(symbol_bmb);
	MODEL_setTexture(modelSymbol, &whiteTexObj);

	//Sprites
	spriteBackground = SPRITE_create();
	f32 pos[2] = { 0, 0 };
	f32 size[2] = { 256, 256 };
	SPRITE_setTexture(spriteBackground, &backgroundTexObj, pos, size, 1024);

	//Objects
	objectSymbol = OBJECT_create(modelSymbol);

	//Symbol
	OBJECT_scaleTo(objectSymbol, .4f, .4f, .4f);
	OBJECT_moveTo(objectSymbol, 0, 0, 50);

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256);

	//Start music
	AU_playMusic(music_mod);
}

void DEMO_update() {
	OBJECT_rotate(objectSymbol, 0, .01f, 0);
}

void DEMO_render(Mtx viewMtx) {

	/* Set default blend mode */
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	/* Enable Light */
	GXU_setLight(viewMtx, lightColor);

	/* Draw Symbol */
	OBJECT_render(objectSymbol, viewMtx);

	SPRITE_render(spriteBackground);

	GXRModeObj* rmode = GXU_getMode();
	FONT_draw(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth / 2, rmode->viHeight - 200, TRUE);
}
