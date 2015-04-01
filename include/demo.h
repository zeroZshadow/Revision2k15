/*! \file game.h
 *  \brief Demo logic handling
 */
#ifndef _DEMO_H
#define _DEMO_H

#include <ogc/gu.h>
#include "scene.h"

/* DEBUG MACROS */
#define FUNC(func, id) RFUNC(func, id)
#define RFUNC(func, id) func ## id
#define RENDER(cam, mtx) FUNC(DEMO_render_scene, SCENEID)(cam,mtx)
#define UPDATE() FUNC(DEMO_update_scene, SCENEID)()

/*! \brief Initialize demo data
 */
void DEMO_init();

/*! \brief Update demo state (logic)
*/
void DEMO_update();
void DEMO_update_scene1();
void DEMO_update_scene2();
void DEMO_update_scene3();
void DEMO_update_scene4();


/*! \brief Render demo
*/
void DEMO_render(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene1(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene2(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene3(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene4(camera_t* mainCamera, Mtx viewMtx);

#endif
