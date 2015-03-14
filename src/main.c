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
s32 reset_func(s32 final);

static sys_resetinfo resetinfo = {
	{ NULL, NULL },
	reset_func,
	1
};

int main() {
	/* Setup reset function */
	SYS_RegisterResetFunc(&resetinfo);

	AU_init();
	GXU_init();
	SCENE_load();

	isRunning = TRUE;
	while (isRunning) {
		SCENE_render();
	}

	return 0;
}

s32 reset_func(s32 final) {
	isRunning = FALSE;
	return 1;
}