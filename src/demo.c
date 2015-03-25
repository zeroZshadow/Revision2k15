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
#include "mathutil.h"

//Data
#include "mine_bmb.h"
#include "gothicube_bmb.h"
#include "cube_bmb.h"

#include "music_ogg.h"

/* Data vars */
GXTexObj whiteTexObj, fontTexObj;
font_t* font;

//Scene 1
model_t *modelMine;
GXTexObj backTexObj, foreTexObj;
object_t *objectMine[3];
sprite_t* spriteBackground, *spriteForeground;
//Scene 2
#define cubeCount 64
model_t *modelCube;
GXTexObj magicianTexObj, magicbackTexObj;
object_t *objectCube[cubeCount], *objectGrow;
sprite_t *spriteMagician, *spriteMagicback;
//Scene 3
model_t *modelGothic;
GXTexObj zombieTexObj;
object_t *objectGothic;
sprite_t *spriteZombie;
//Scene 4
GXTexObj ballTexObj;
sprite_t *spriteBall[8];

int framenr = 0;

/* Light */
static GXColor lightColor[] = {
	{ 0x33, 0xCC, 0x33, 0xFF }, /* Light color 1 */
	{ 0x66, 0x66, 0xff, 0xFF }, /* Ambient 1 */
	{ 0xFF, 0xFF, 0xFF, 0xFF }  /* Material 1 */
};

static guVector ballPos[] = {
	//Front
	{ -1, 1, 1 },
	{ 1, 1, 1 },
	{ 1, -1, 1 },
	{ -1, -1, 1 },
	//Back
	{ -1, 1, -1 },
	{ 1, 1, -1 },
	{ 1, -1, -1 },
	{ -1, -1, -1 }
};

void DEMO_init() {
	u32 i;

	//Textures
	GXU_loadTexture(whiteTex, &whiteTexObj);
	GXU_loadTexture(foreTex, &foreTexObj);
	//Scene 1
	GXU_loadTexture(backTex, &backTexObj);
	GXU_loadTexture(fontTex, &fontTexObj);
	//Scene 2
	GXU_loadTexture(magicianTex, &magicianTexObj);
	GXU_loadTexture(magicbackTex, &magicbackTexObj);
	//Scene 3
	GXU_loadTexture(zombieTex, &zombieTexObj);
	//Scene 4
	GXU_loadTexture(ballTex, &ballTexObj);
	GX_InvalidateTexAll();

	//Meshes
	modelMine = MODEL_setup(mine_bmb);
	modelCube = MODEL_setup(cube_bmb);
	modelGothic = MODEL_setup(gothicube_bmb);
	MODEL_setTexture(modelMine, &whiteTexObj);
	MODEL_setTexture(modelCube, &whiteTexObj);
	MODEL_setTexture(modelGothic, &whiteTexObj);

	//Sprites
	spriteBackground = SPRITE_create(0, 0, -10, 640, 528);
	spriteForeground = SPRITE_create(0, 0, -10, 640, 528);
	spriteMagician = SPRITE_create(0, 0, -10, 640, 528);
	spriteMagicback = SPRITE_create(0, 0, -10, 640, 528);
	spriteZombie = SPRITE_create(0, 0, -80, 640, 528);
	SPRITE_setTexture(spriteBackground, &backTexObj);
	SPRITE_setTexture(spriteForeground, &foreTexObj);
	SPRITE_setTexture(spriteMagician, &magicianTexObj);
	SPRITE_setTexture(spriteMagicback, &magicbackTexObj);
	SPRITE_setTexture(spriteZombie, &zombieTexObj);
	for (i = 0; i < 8; i++) {
		spriteBall[i] = SPRITE_create(640/2, 528/2, -50, 32, 32);
		SPRITE_setTexture(spriteBall[i], &ballTexObj);
	}

	//Objects
	//Mines
	for (i = 0; i < 3; ++i) {
		objectMine[i] = OBJECT_create(modelMine);
		OBJECT_scaleTo(objectMine[i], .2f, .2f, .2f);
	}
	//Cubes
	for (i = 0; i < cubeCount; ++i) {
		objectCube[i] = OBJECT_create(modelCube);
		OBJECT_scaleTo(objectCube[i], 2.0f, 2.0f, 2.0f);
		OBJECT_rotate(objectCube[i], fioraRand(), fioraRand(), 0);
	}
	objectGrow = OBJECT_create(modelCube);
	OBJECT_moveTo(objectGrow, 0, -6, -50);
	//Gothic
	objectGothic = OBJECT_create(modelGothic);
	OBJECT_moveTo(objectGothic, -10, -6, -50);

	//Fonts
	font = FONT_load(&fontTexObj, " !,.0123456789:<>?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 12, 22, 256, 3.0f);

	//Start music
	//AU_playMusic(music_ogg, music_ogg_size);
}

void DEMO_update() {
	DEMO_update_scene4();
	framenr++;
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {
	DEMO_render_scene4(mainCamera, viewMtx);
}

void DEMO_update_scene1() {
	//Move mines around
	u32 i;
	for (i = 0; i < 3; ++i) {
		f32 prog = (framenr * 0.02f) + (i * 5);
		OBJECT_rotate(objectMine[i], sin(prog) * 0.04f, cos(prog) * 0.06f, 0);
		f32 xoffset = (sin(prog * 1.3f) * 0.8f + (i * 0.3f) - 0.3f) * 20.0f;
		f32 yoffset = (cos(prog) * 0.5f + (i * 0.2f) - 0.2f) * 20.0f;
		f32 zoffset = cos(prog * 3) * 20.0f;
		OBJECT_moveTo(objectMine[i], xoffset, yoffset, -50 + zoffset);
	}
}

void DEMO_render_scene1(camera_t* mainCamera, Mtx viewMtx) {
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
		OBJECT_render(objectMine[i], viewMtx);
	}

	GX_SetZMode(GX_TRUE, GX_LESS, GX_FALSE);
	SPRITE_render(spriteBackground);
}

void DEMO_update_scene2() {
	f32 prog = framenr * 0.02f;
	u32 i;
	for (i = 0; i < cubeCount; ++i) {
		f32 p = i <= (cubeCount >> 1) ? prog : -prog;
		//curve * size
		f32 xcurve = cos(p + i) * 25;
		//offset + spread curve - bend
		f32 ycurve = -8 + sin((p + i) * 2) * ((i % 51) * (7.0f / 30.0f)) - (fabs(xcurve) * 0.2f);
		OBJECT_moveTo(objectCube[i], xcurve, ycurve + sin(p + i * 200) * 5, -50);
		OBJECT_rotate(objectCube[i], 0.04f, 0.06f, 0);

		f32 scale = 2.0f * fabs(sin(p + i));
		OBJECT_scaleTo(objectCube[i], scale, scale, scale);
	}


	//Slowly grow the cube
	OBJECT_rotate(objectGrow, 0.05f, 0, 0);

	f32 scale = (prog * fabs(sin(prog)) * 3 + 5);
	OBJECT_scaleTo(objectGrow, scale, scale, scale);
}

void DEMO_render_scene2(camera_t* mainCamera, Mtx viewMtx) {
	u32 i;

	/* Background */
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	SPRITE_render(spriteMagician);

	/* Draw objects */
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	for (i = 0; i < cubeCount; ++i) {
		OBJECT_render(objectCube[i], viewMtx);
	}

	GX_SetZMode(GX_TRUE, GX_LESS, GX_TRUE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	SPRITE_render(spriteMagicback);


	//Grow cube
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);
	GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	OBJECT_render(objectGrow, viewMtx);

	GX_SetZMode(GX_TRUE, GX_LESS, GX_TRUE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	SPRITE_render(spriteForeground);
}

void DEMO_update_scene3() {
	//rotate the cube
	OBJECT_rotate(objectGothic, 0.02f, 0, 0);
	OBJECT_rotate(objectGothic, 0, 0.01f, 0);
}

void DEMO_render_scene3(camera_t* mainCamera, Mtx viewMtx) {
	/* Render bitmaps */
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	SPRITE_render(spriteZombie);

	/* Draw objects */
	GX_LoadProjectionMtx(mainCamera->perspectiveMtx, GX_PERSPECTIVE);
	GXU_setLight(viewMtx, lightColor, (guVector) { 0, 0, 1 });
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	OBJECT_render(objectGothic, viewMtx);

	/* Disable font */
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXRModeObj* rmode = GXU_getMode();
	FONT_drawScroller(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth - (framenr * 2.0f), rmode->viHeight - 120, 0, 0.5f, 8, framenr * 0.1f);
}

u32 ballZindex[8];

void DEMO_update_scene4() {
	u32 i;
	float smallZ = 100;
	float hwidth = 640 / 2;
	float hheight = 528 / 2;
	f32 prog = framenr * 0.02f;

	Mtx mat;
	guVector axis = (guVector) { 0, -1, -1 };
	guMtxIdentity(mat);
	guMtxRotAxisRad(mat, &axis, prog);

	//Preset sorting
	ballZindex[0] = 0;

	for (i = 0; i < 8; ++i) {
		guVector pos;
		guVecMultiply(mat, &ballPos[i], &pos);
		float scale = (pos.z + 1.0f) * 0.5f + 1.0f;

		SPRITE_moveTo(spriteBall[i], pos.x * 20 + hwidth, pos.y * 20 + hheight, -50 + scale);
		SPRITE_scaleTo(spriteBall[i], scale, scale, 1);
	}
}

void DEMO_render_scene4(camera_t* mainCamera, Mtx viewMtx) {
	u32 i;

	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, 0);
	GX_SetZMode(GX_TRUE, GX_LESS, GX_TRUE);
	GX_SetZCompLoc(GX_FALSE);
	for (i = 0; i < 8; ++i) {
		SPRITE_render(spriteBall[i]);
	}
	GX_SetZCompLoc(GX_TRUE);
}