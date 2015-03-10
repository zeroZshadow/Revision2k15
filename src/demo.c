#include "demo.h"

#include <malloc.h>
#include <gccore.h>
#include <math.h>

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

int framenr = 0;

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
	spriteBackground = SPRITE_create(0, 0, 640, 528);
	SPRITE_setTexture(spriteBackground, &backgroundTexObj);

	//Objects
	objectSymbol = OBJECT_create(modelSymbol);

	//Symbol
	OBJECT_scaleTo(objectSymbol, .4f, .4f, .4f);
	OBJECT_moveTo(objectSymbol, 30, 10, 80);

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256);

	//Start music
	//AU_playMusic(music_mod);
}

void DEMO_update() {
	//OBJECT_rotate(objectSymbol, 0, .01f, 0);
	framenr++;
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {
	/* Render background */
	/* Disable Zbuf */
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	SPRITE_render(spriteBackground);

	/* Set default blend mode */
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	/*Load perspective */
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);

	/* Enable Zbuf */
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);

	/* Enable Light */
	GXU_setLight(viewMtx, lightColor, (guVector) {0, -1, sin(framenr / 10.f)});

	/* Draw Symbol */
	OBJECT_render(objectSymbol, viewMtx);

	/* Disable Zbuf */
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GXRModeObj* rmode = GXU_getMode();
	FONT_draw(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth / 2, rmode->viHeight - 200, TRUE);
}
