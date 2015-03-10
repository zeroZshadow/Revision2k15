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

/*! \brief Render demo
*/
void DEMO_render(camera_t* mainCamera, Mtx viewMtx);

#endif
