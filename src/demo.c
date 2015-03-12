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
sprite_t* spriteBackground, *spriteForeground;
GXTexObj whiteTexObj, blackTexObj, darkTexObj, lightTexObj, fontTexObj;
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
	GXU_loadTexture(blackTex, &blackTexObj);
	GXU_loadTexture(lightTex, &lightTexObj);
	GXU_loadTexture(darkTex, &darkTexObj);
	GXU_loadTexture(ubuntuFontTex, &fontTexObj);

	//Meshes
	modelSymbol = MODEL_setup(symbol_bmb);
	MODEL_setTexture(modelSymbol, &blackTexObj);

	//Sprites
	spriteBackground = SPRITE_create(0, 0, -200, 640, 528);
	spriteForeground = SPRITE_create(0, 0, 0.9f, 640, 528);
	SPRITE_setTexture(spriteBackground, &lightTexObj);
	SPRITE_setTexture(spriteForeground, &darkTexObj);

	//Objects
	objectSymbol = OBJECT_create(modelSymbol);

	//Symbol
	OBJECT_scaleTo(objectSymbol, .4f, .4f, .4f);
	OBJECT_moveTo(objectSymbol, 0, 0, -150);

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256);

	//Start music
	//AU_playMusic(music_mod);
}

void DEMO_update() {
	OBJECT_rotate(objectSymbol, 0, .03f, 0);
	framenr++;
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {

	/* Draw Symbol */
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);
	GXU_setLight(viewMtx, lightColor, (guVector) {0,0,-1});
	GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	OBJECT_render(objectSymbol, viewMtx);

	/* Render background */
	//GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	//GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	//GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	SPRITE_render(spriteBackground);

	//SPRITE_render(spriteForeground);

	/* Disable Zbuf */
	GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	GXRModeObj* rmode = GXU_getMode();
	FONT_draw(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth / 2, rmode->viHeight - 100, TRUE);
}
