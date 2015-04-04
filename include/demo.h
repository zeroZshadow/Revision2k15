/*! \file game.h
 *  \brief Demo logic handling
 */
#ifndef _DEMO_H
#define _DEMO_H

#include <ogc/gu.h>
#include "gxutils.h"

/*! \brief Initialize demo data
 */
void DEMO_init();

/*! \brief Update demo state (logic)
*/
void DEMO_update();
void DEMO_update_scene0();
void DEMO_update_scene1();
void DEMO_update_scene2();
void DEMO_update_scene3();
void DEMO_update_scene4(); //INTRO
void DEMO_update_scene5(); //CREDITS


/*! \brief Render demo
*/
void DEMO_render(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene0(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene1(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene2(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene3(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene4(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene5(camera_t* mainCamera, Mtx viewMtx);

BOOL DEMO_script();

BOOL DEMO_fadeSlideOut(s32 length);
BOOL DEMO_fadeSlideIn(s32 length);
BOOL DEMO_fadeOut(s32 length);
BOOL DEMO_fadeIn(s32 length);

enum SCRIPT {
	SCENE = 0,
	WAITTILL,
	WAITTILLSUB,
	EXIT,
	FADESLIDEIN,
	FADESLIDEOUT,
	FADEIN,
	FADEOUT
};

#endif
