/* System and SDK libraries */
#include <gccore.h>

/* Generated assets headers */
#include "demo.h"
#include "gxutils.h"
#include "mathutil.h"
#include "font.h"
#include "sprite.h"

/* Data vars */
GXTexObj terrainTexObj, fontTexObj;

/* Spectator */
camera_t mainCamera;
Mtx mainView;

BOOL firstFrame = TRUE;

void SCENE_load() {
	/* Setup spectator matrix */
	GXU_setupCamera(&mainCamera);
	GX_SetViewport(mainCamera.offsetLeft, mainCamera.offsetTop, mainCamera.width, mainCamera.height, 0, 1);
	guVector cameraPos = { 0, 0, 0 };
	guVector targetPos = { 0, 0, -1 };
	guVector cameraUp = { 0, 1, 0 };
	guLookAt(mainView, &cameraPos, &cameraUp, &targetPos);

	FONT_init();

	DEMO_init();
}

void SCENE_render() {
	/* Render time */
	GX_SetNumChans(1);

	//Update demo login
	DEMO_update();

	//Render scene
	DEMO_render(&mainCamera, mainView);

	/* Flip framebuffer */
	GXU_done();
}
