/*! \file game.h
 *  \brief Demo logic handling
 */
#ifndef _DEMO_H
#define _DEMO_H

#include <ogc/gu.h>
#include "scene.h"

/*! \brief Initialize demo data
 */
void DEMO_init();

/*! \brief Update demo state (logic)
*/
void DEMO_update();
void DEMO_update_scene1();
void DEMO_update_scene2();

/*! \brief Render demo
*/
void DEMO_render(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene1(camera_t* mainCamera, Mtx viewMtx);
void DEMO_render_scene2(camera_t* mainCamera, Mtx viewMtx);

#endif
