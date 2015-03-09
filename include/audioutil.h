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
 *  \param music Music Id, from generated assets
 */
void AU_playMusic(const void* music);

/*! \brief Get position on channel
*  \param index Channel index
*/
u32 AU_getChannelPos(u32 index);

u32 AU_getSongPos();

#endif