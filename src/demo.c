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
GXTexObj ballTexObj[2], demonTexObj;
sprite_t *spriteBall[8], *spriteDemon;

s32 sceneoffset = 0;
s32 scenetime = 0;
u8 scenecount = 0;

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
	AU_setVolume(255); //TODO: REMOVE
}

//#define SCENEID 4

void DEMO_update() {
	scenetime = AU_getPos();

#ifdef SCENEID
	UPDATE();
#else
	switch (scenecount) {
	case 0:
		DEMO_update_scene2();
		if (scenetime > 17851) {
			sceneoffset = 17851;
			scenecount++;
		} //Timing done!
		break;
	case 1:
		DEMO_update_scene1();
		if (scenetime > 40000) {
			sceneoffset = 40000;
			scenecount++;
		}
			break;
	case 2:
		DEMO_update_scene3();
		if (scenetime > 80000) {
			sceneoffset = 80000;
			scenecount++;
		}
			break;
	case 3:
		DEMO_update_scene4();
		break;
	}
#endif
}

void DEMO_render(camera_t* mainCamera, Mtx viewMtx) {
#ifdef SCENEID
	RENDER(mainCamera, viewMtx);
#else
	switch (scenecount) {
	case 0: DEMO_render_scene2(mainCamera, viewMtx); break;
	case 1: DEMO_render_scene1(mainCamera, viewMtx); break;
	case 2: DEMO_render_scene3(mainCamera, viewMtx); break;
	case 3: DEMO_render_scene4(mainCamera, viewMtx); break;
	}
#endif
}

void DEMO_update_scene1() {
	//Move mines around
	u32 i;
	for (i = 0; i < 3; ++i) {
		f32 prog = (scenetime * 0.001f) + (i * 5);
		OBJECT_rotateTo(objectMine[i], sin(prog) * 2 + prog, cos(prog) * 3, 0);
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
	f32 prog = scenetime * 0.001f;
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
	FONT_drawScroller(font, "Aww yeah a brand new DeSiRe demo for the GameCube", rmode->viWidth - (scenetime * 0.1f), rmode->viHeight - 120, 0, 0.5f, 8, scenetime * 0.005f);
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

void DEMO_update_scene4() {
	u32 i;
	GXRModeObj* rmode = GXU_getMode();
	const float hwidth = rmode->viWidth / 2;
	const float hheight = rmode->xfbHeight / 2;

	const f32 prog = scenetime * 0.002f;

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

void DEMO_render_scene4(camera_t* mainCamera, Mtx viewMtx) {
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