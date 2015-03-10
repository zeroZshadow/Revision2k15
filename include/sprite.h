/*! \file sprite.h
*  \brief Sprites and other 2D elements
*/

#ifndef _SPRITE_H
#define _SPRITE_H

#include <ogcsys.h>
#include "gxutils.h"
#include "object.h"

typedef struct {
	GXTexObj*   texture;
	transform_t transform;
	f32 width, height;
} sprite_t;

/*! \brief Create empty sprite
 *  \return Pointer to newly sprite structure
 */
sprite_t* SPRITE_create(f32 x, f32 y, f32 width, f32 height);

/*! \brief Frees a sprite
 *  \param sprite Sprite to destroy
 */
void SPRITE_free(sprite_t* sprite);

/*! \brief Assign a texture to a sprite, including offset/size
 *  \param sprite     Sprite to assign texture to
 *  \param texture    Texture object to use
 *  \param position   UV offset/position
 *  \param texSize    UV total size
 *  \param spriteSize Sprite size in the texture
 */
void SPRITE_setTexture(sprite_t* sprite, GXTexObj* texture);

/*! \brief Renders a sprite on screen 
 *  \param sprite Sprite to render
 */
void SPRITE_render(sprite_t* sprite);

/*! \brief Force Matrix regeneration o sprite
*  \param sprite Sprite to process
*/
void SPRITE_flush(sprite_t* sprite);

/*! \brief Move a sprite to a specified position
*  \param object Object to move
*  \param tX     X coordinate
*  \param tY     Y coordinate
*/
void SPRITE_moveTo(object_t* object, const f32 tX, const f32 tY);

#endif