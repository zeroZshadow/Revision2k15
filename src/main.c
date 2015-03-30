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

int main() {
	AU_init();
	GXU_init();
	fioraSeed(0);

	SCENE_load();

	isRunning = TRUE;
	while (isRunning && !SYS_ResetButtonDown()) {
		SCENE_render();
	}

	SYS_ResetSystem(SYS_RESTART, 0, 0);

	return 0;
}