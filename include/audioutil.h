/*! \file audioutil.h
*  \brief Audio / Music / SFX utilities
*/
#ifndef _AUDIOUTIL_H
#define _AUDIOUTIL_H

#include <gccore.h>

/*! \brief Initialize audio subsystem
 */
void AU_init();

/*! \brief Play a specific music
 *  \param music buffer pointer, from generated assets
 *  \param size buffer size, from generated assets
 */
void AU_playMusic(const void* music, const s32 size);

void AU_stopMusic();

#endif