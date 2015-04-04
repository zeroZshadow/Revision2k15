#include "demo.h"

#include <malloc.h>
#include <gccore.h>
#include <math.h>
#include <stdio.h>

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
sprite_t *spriteFade;
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
GXTexObj ballTexObj[2], demonTexObj;
sprite_t *spriteBall[8], *spriteDemon;

//Scene timing and index
s32 sceneoffset = 0;
s32 scenetime = 0;
s32 subscenetime = 0;
u8 scenecount = -1;

//Fade info
s32 fadestart = -1;

/* Light */
static GXColor lightColor[] = {
	{ 0x33, 0xCC, 0x33, 0xFF }, /* Light color 1 */
	{ 0x66, 0x66, 0xff, 0xFF }, /* Ambient 1 */
	{ 0xFF, 0xFF, 0xFF, 0xFF }  /* Material 1 */
};

static guVector ballPos[] = {
	//Front
	{ -10, 10, 10 },
	{ 10, 10, 10 },
	{ 10, -10, 10 },
	{ -10, -10, 10 },
	//Back
	{ -10, 10, -10 },
	{ 10, 10, -10 },
	{ 10, -10, -10 },
	{ -10, -10, -10 }
};

static guVector ballPos2[] = {
	//Line
	{ 70.71067812, 70.71067812, 0 },
	{ 0, 100, 0 },
	{ -70.71067812, 70.71067812, 0 },
	{ -100, 0, 0 },
	{ -70.71067812, -70.71067812, 0 },
	{ 0, -100, 0 },
	{ 70.71067812, -70.71067812, 0 },
	{ 100, 0, 0 },
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
	GXU_loadTexture(ballTex, &ballTexObj[0]);
	GX_InitTexObjFilterMode(&ballTexObj[0], GX_NEAR, GX_NEAR); //Point filtering
	GXU_loadTexture(ball2Tex, &ballTexObj[1]);
	GX_InitTexObjFilterMode(&ballTexObj[1], GX_NEAR, GX_NEAR); //Point filtering
	GXU_loadTexture(isabellaTex, &demonTexObj);

	GXU_closeTPL();

	//Invalidate texture cache
	GX_InvalidateTexAll();

	//Meshes
	modelMine = MODEL_setup(mine_bmb, &whiteTexObj);
	modelCube = MODEL_setup(cube_bmb, &whiteTexObj);
	modelGothic = MODEL_setup(gothicube_bmb, &whiteTexObj);

	//Sprites
	spriteBackground = SPRITE_create(0, 0, -10, 640, 528, &backTexObj);
	spriteForeground = SPRITE_create(0, 0, -10, 640, 528, &foreTexObj);
	spriteMagician = SPRITE_create(0, 0, -10, 640, 528, &magicianTexObj);
	spriteMagicback = SPRITE_create(0, 0, -10, 640, 528, &magicbackTexObj);
	spriteZombie = SPRITE_create(0, 0, -80, 640, 528, &zombieTexObj);
	spriteDemon = SPRITE_create(0, 0, -280, 640, 528, &demonTexObj);

	spriteFade = SPRITE_create(0, 0, -1, 640, 528, &whiteTexObj);

	for (i = 0; i < 8; i++) {
		spriteBall[i] = SPRITE_create(640 / 2, 528 / 2, -50, 32, 32, &ballTexObj[i%2]);
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
	AU_playMusic(music_ogg, music_ogg_size);
	AU_setVolume(0); //TODO: REMOVE
}

// 0 WHITE GIRL
// 1 WIZARD
// 2 DARK GIRL
// 3 SPRITES

u32 scriptIndex = 0;
const s32 scriptList[] = {
	SCENE, 2,	//DARKGIRL
	WAITTILL, 19500,
	FADESLIDEOUT, 500,
	SCENE, 1,	//WIZARD
	FADEIN, 500,
	WAITTILL, 37851,
	SCENE, 0,	//WHITEGIRL
	WAITTILL, 54500,
	FADEOUT, 500,
	SCENE, 3,	//SPRITES
	FADESLIDEIN, 500,
	WAITTILL, 75000,
	FADEOUT, 5000,
	EXIT
};

BOOL DEMO_script() {
	s32 t;

	if (scriptIndex >= sizeof(scriptList)) {
		return TRUE;
	} else {
		BOOL loop;
		do {
			loop = FALSE;
			const s32 cmd = scriptList[scriptIndex];
			switch (cmd) {
			case SCENE:
				//Grab param
				scenecount = scriptList[scriptIndex + 1];
				sceneoffset = scenetime;
				scriptIndex += 2;
				loop = TRUE;
				break;
			case WAITTILL:
				//Grab param
				t = scriptList[scriptIndex + 1];
				if (scenetime >= t) {
					scriptIndex += 2;
					loop = TRUE;
				}
				break;
			case FADESLIDEIN:
			case FADESLIDEOUT:
			case FADEIN:
			case FADEOUT:
				//Param
				t = scriptList[scriptIndex + 1];

				//First run, setup
				if (fadestart < 0) {
					fadestart = scenetime;
				}

				BOOL r = FALSE;

				switch (cmd) {
				case FADESLIDEIN:
					r = DEMO_fadeSlideIn(t); break;
				case FADESLIDEOUT:
					r = DEMO_fadeSlideOut(t); break;
				case FADEIN:
					r = DEMO_fadeIn(t); break;
				case FADEOUT:
					r = DEMO_fadeOut(t); break;
				}

				if (r) {
					fadestart = -1;
					scriptIndex += 2;
					loop = TRUE;
				}
				break;
			default:
				scriptIndex++;
				return TRUE;
			}
		} while (loop);
	}

	return FALSE;
}

void DEMO_update() {
	scenetime = AU_getPos();
	subscenetime = scenetime - sceneoffset;

	switch (scenecount) {
	case 0:	DEMO_update_scene0();break;
	case 1:	DEMO_update_scene1();break;
	case 2:	DEMO_update_scene2();break;
	case 3:	DEMO_update_scene3();break;
	}
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {
	switch (scenecount) {
	case 0: DEMO_render_scene0(mainCamera, viewMtx); break;
	case 1: DEMO_render_scene1(mainCamera, viewMtx); break;
	case 2: DEMO_render_scene2(mainCamera, viewMtx); break;
	case 3: DEMO_render_scene3(mainCamera, viewMtx); break;
	}
}

void DEMO_update_scene0() {
	//popin scaler
	f32 popin = subscenetime / 1000.0f; popin = (popin > 1.0f ? 1.0f : popin) * 0.2f;

	//Move mines around
	u32 i;
	for (i = 0; i < 3; ++i) {
		f32 prog = (subscenetime * 0.001f) + (i * 5);
		OBJECT_rotateTo(objectMine[i], sin(prog) * 2 + prog, cos(prog) * 3, 0);
		f32 xoffset = (sin(prog * 1.3f) * 0.8f + (i * 0.3f) - 0.3f) * 20.0f;
		f32 yoffset = (cos(prog) * 0.5f + (i * 0.2f) - 0.2f) * 20.0f;
		f32 zoffset = cos(prog * 3) * 20.0f;
		OBJECT_moveTo(objectMine[i], xoffset, yoffset, -50 + zoffset);
		OBJECT_scaleTo(objectMine[i], popin, popin, popin);
	}
}

void DEMO_render_scene0(camera_t* mainCamera, Mtx viewMtx) {
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

void DEMO_update_scene1() {
	f32 prog = subscenetime * 0.001f;
	u32 i;
	for (i = 0; i < cubeCount; ++i) {
		f32 p = i <= (cubeCount >> 1) ? prog : -prog;
		//curve * size
		f32 xcurve = cos(p + i) * 25;
		//offset + spread curve - bend
		f32 ycurve = -8 + sin((p + i) * 2) * ((i % 51) * (7.0f / 30.0f)) - (fabs(xcurve) * 0.2f);
		OBJECT_moveTo(objectCube[i], xcurve, ycurve + sin(p + i * 200) * 5, -50);
		OBJECT_rotateTo(objectCube[i], prog * 3, prog * 5, 0);

		f32 scale = 2.0f * fabs(sin(p + i));
		OBJECT_scaleTo(objectCube[i], scale, scale, scale);
	}


	//Slowly grow the cube
	OBJECT_rotateTo(objectGrow, prog * 3, 0, 0);

	f32 scale = (prog * fabs(sin(prog)) * 3 + 5);
	OBJECT_scaleTo(objectGrow, scale, scale, scale);
}

void DEMO_render_scene1(camera_t* mainCamera, Mtx viewMtx) {
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

void DEMO_update_scene2() {
	f32 prog = subscenetime * 0.005f;

	//rotate the cube
	OBJECT_rotateTo(objectGothic, prog * 0.1f, prog * 0.5f, 0);
}

void DEMO_render_scene2(camera_t* mainCamera, Mtx viewMtx) {
	/* Render bitmaps */
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
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
	FONT_drawScroller(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth - (subscenetime * 0.1f), rmode->viHeight - 120, 0, 0.5f, 8, subscenetime * 0.005f);
}

u32 spriteID[8];
f32 spriteDepth[8];

void SortSprites() {
	u32 c, d, p;
	for (c = 0; c < (8 - 1); c++) {
		p = c;

		for (d = c + 1; d < 8; d++) {
			if (spriteDepth[p] > spriteDepth[d])
				p = d;
		}

		if (p != c) {
			//Swap depth
			u32* spriteDepthu = (u32*)spriteDepth;
			spriteDepthu[c] ^= spriteDepthu[p];
			spriteDepthu[p] ^= spriteDepthu[c];
			spriteDepthu[c] ^= spriteDepthu[p];

			//Swap ID
			spriteID[c] ^= spriteID[p];
			spriteID[p] ^= spriteID[c];
			spriteID[c] ^= spriteID[p];
		}
	}
}

void DEMO_update_scene3() {
	u32 i;
	GXRModeObj* rmode = GXU_getMode();
	const float hwidth = rmode->viWidth / 2;
	const float hheight = rmode->xfbHeight / 2;

	const f32 prog = subscenetime * 0.002f;

	Mtx mat1, mat2;
	guQuaternion quat;
	guVector axis1 = (guVector) { 0, -1, -1 };
	guVecNormalize(&axis1);
	guVector axis2 = (guVector) { 0, 0, -1 };

	//Build matrix 1
	AxisAngleToQuaternion(&quat, axis1, prog);
	SimpleMatrix(mat1, &quat, &(guVector) { 5, 5, 5 }, &(guVector) { hwidth - 16, hheight - 16, 0 });

	//Mat 2
	AxisAngleToQuaternion(&quat, axis2, -prog * 2);
	SimpleMatrix(mat2, &quat, &(guVector) { 1, 1, 1 }, &(guVector) { hwidth - 16, hheight - 16, 0 });

	for (i = 0; i < 8; ++i) {
		guVector pos1, pos2, delta;
		guVecMultiply(mat1, &ballPos[i], &pos1);
		guVecMultiply(mat2, &ballPos2[i], &pos2);

		float t = ((sin(prog * 0.1f) * 2 + 2) * 0.5f) - 1; t = t < 0 ? 0 : (t > 1 ? 1 : t);
		guVecSub(&pos2, &pos1, &delta);
		guVecScale(&delta, &delta, t);
		guVecAdd(&pos1, &delta, &pos1);

		float scale = ((pos1.z / 50) + 1 / 0.5f) * 1;
		SPRITE_moveTo(spriteBall[i], pos1.x, pos1.y, pos1.z - 150);
		SPRITE_scaleTo(spriteBall[i], scale, scale, 1);

		//Sorting
		spriteID[i] = i;
		spriteDepth[i] = pos1.z;
	}

	SortSprites();
}

void DEMO_render_scene3(camera_t* mainCamera, Mtx viewMtx) {
	u32 i;

	/* Render bitmaps */
	GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	SPRITE_render(spriteDemon);

	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, 0);
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	for (i = 0; i < 8; ++i) {
		SPRITE_render(spriteBall[spriteID[i]]);
	}
}

BOOL DEMO_fadeSlideOut(s32 length) {
	f32 prog = (f32)(scenetime - fadestart) / (f32)length;
	if (prog >= 1.0f) {
		return TRUE;
	} else {
		GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		SPRITE_moveTo(spriteFade, -640 + prog * 640, 0, -1);
		SPRITE_color(spriteFade, (GXColor) { 0, 0, 0, 0xff });
		SPRITE_render(spriteFade); //Colorize alter alpha
	}

	return FALSE;
}

BOOL DEMO_fadeSlideIn(s32 length) {
	f32 prog = (f32)(scenetime - fadestart) / (f32)length;
	if (prog >= 1.0f) {
		return TRUE;
	} else {
		GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		SPRITE_moveTo(spriteFade, prog * 640, 0, -1);
		SPRITE_color(spriteFade, (GXColor) { 0, 0, 0, 0xff });
		SPRITE_render(spriteFade); //Colorize alter alpha
	}

	return FALSE;
}

BOOL DEMO_fadeOut(s32 length) {
	f32 prog = (f32)(scenetime - fadestart) / (f32)length;
	if (prog > 1.0f) {
		return TRUE;
	} else {
		GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		SPRITE_moveTo(spriteFade, 0, 0, -1);
		SPRITE_color(spriteFade, (GXColor) { 0, 0, 0, (u8)(prog * 255) });
		SPRITE_render(spriteFade); //Colorize alter alpha
	}

	return FALSE;
}

BOOL DEMO_fadeIn(s32 length) {
	f32 prog = (f32)(scenetime - fadestart) / (f32)length;
	if (prog > 1.0f) {
		return TRUE;
	} else {
		GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
		GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		SPRITE_moveTo(spriteFade, 0, 0, -1);
		SPRITE_color(spriteFade, (GXColor) { 0, 0, 0, (u8)((1.0f - prog) * 255) });
		SPRITE_render(spriteFade); //Colorize alter alpha
	}

	return FALSE;
}