/* SDK Libraries */
#include <gccore.h>
#include <stdlib.h>
#include <time.h>
#include "gxutils.h"

/* Internal libraries */
#include "scene.h"
#include "audioutil.h"
#include "mathutil.h"

BOOL isRunning;
void OnResetCalled();

int main() {
	/* Setup reset function */
	SYS_SetResetCallback(OnResetCalled);

	AU_init();
	GXU_init();
	SCENE_load();

	isRunning = TRUE;
	while (isRunning) {
		SCENE_render();
	}

	return 0;
}

void OnResetCalled() {
	isRunning = FALSE;
}