/* SDK Libraries */
#include <gccore.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Internal libraries */
#include "demo.h"
#include "audioutil.h"
#include "mathutil.h"
#include "gxutils.h"
#include "mathutil.h"
#include "font.h"
#include "sprite.h"

camera_t* mainCamera;
Mtx mainView;
BOOL isRunning;

int main() {
	AU_init();
	GXU_init();
	fioraSeed(0);

	/* Setup spectator matrix */
	mainCamera = malloc(sizeof(camera_t));
	memset(mainCamera, 0, sizeof(camera_t));

	GXU_setupCamera(mainCamera);
	guVector cameraPos = { 0, 0, 0 };
	guVector targetPos = { 0, 0, -1 };
	guVector cameraUp = { 0, 1, 0 };
	guLookAt(mainView, &cameraPos, &cameraUp, &targetPos);

	FONT_init();

	DEMO_init();

	isRunning = TRUE;
	while (isRunning && !SYS_ResetButtonDown()) {
		//Update demo
		DEMO_update();

		//Render scene
		DEMO_render(mainCamera, mainView);

		//Run script
		if (DEMO_script()) {
			break;
		}


		/* Flip framebuffer */
		GXU_done();
	}

	SYS_ResetSystem(SYS_RESTART, 0, 0);

	return 0;
}