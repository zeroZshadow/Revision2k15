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
#include "mine_bmb.h"
#include "music_ogg.h"

/* Data vars */
model_t *modelSymbol;
object_t *objectSymbol[3];
sprite_t* spriteBackground, *spriteForeground;
GXTexObj whiteTexObj, backTexObj, foreTexObj, fontTexObj;
font_t* font;

int framenr = 0;

/* Light */
static GXColor lightColor[] = {
	{ 0xD0, 0xD0, 0xD0, 0xFF }, /* Light color 1 */
	{ 0x40, 0x40, 0x40, 0xFF }, /* Ambient 1 */
	{ 0x80, 0x80, 0x80, 0xFF }  /* Material 1 */
};

void DEMO_init() {
	//Textures
	GXU_loadTexture(whiteTex, &whiteTexObj);
	GXU_loadTexture(foreTex, &foreTexObj);
	GXU_loadTexture(backtestTex, &backTexObj);
	GXU_loadTexture(fontTex, &fontTexObj);
	GX_InvalidateTexAll();

	//Meshes
	modelSymbol = MODEL_setup(mine_bmb);
	MODEL_setTexture(modelSymbol, &whiteTexObj);

	//Sprites
	spriteBackground = SPRITE_create(0, 0, -20, 640, 528);
	spriteForeground = SPRITE_create(0, 0, -10, 640, 528);
	SPRITE_setTexture(spriteBackground, &backTexObj);
	SPRITE_setTexture(spriteForeground, &foreTexObj);

	//Objects
	u32 i;
	for (i = 0; i < 3; ++i) {
		objectSymbol[i] = OBJECT_create(modelSymbol);

		//Symbol
		OBJECT_scaleTo(objectSymbol[i], .2f, .2f, .2f);
	}

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256, 3.0f);

	//Start music
	AU_playMusic(music_ogg, music_ogg_size);
}

void DEMO_update() {
	u32 i;
	for (i = 0; i < 3; ++i) {
		f32 prog = (framenr * 0.02f) + (i * 5);
		OBJECT_rotate(objectSymbol[i], sin(prog) * 0.04f, cos(prog) * 0.06f, 0);
		f32 xoffset = (sin(prog * 1.3f) * 0.8f + (i * 0.3f) - 0.3f) * 20.0f;
		f32 yoffset = (cos(prog) * 0.5f + (i * 0.2f) - 0.2f) * 20.0f;
		f32 zoffset = cos(prog * 3) * 20.0f;
		OBJECT_moveTo(objectSymbol[i], xoffset, yoffset, -50 + zoffset);
	}
	framenr++;
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {
	u32 i;

	/* Render bitmaps */
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	SPRITE_render(spriteForeground);

	/* Draw objects */
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	for (i = 0; i < 3; ++i) {
		OBJECT_render(objectSymbol[i], viewMtx);
	}

	GX_SetZMode(GX_TRUE, GX_LESS, GX_FALSE);
	SPRITE_render(spriteBackground);

	/* Disable font */
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXRModeObj* rmode = GXU_getMode();
	FONT_drawScroller(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth - (framenr * 2.0f), rmode->viHeight - 120, 0, 0.5f, 8, framenr * 0.1f);
}
