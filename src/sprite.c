#include "sprite.h"
#include "mathutil.h"
#include <malloc.h>

sprite_t* SPRITE_create(f32 x, f32 y, f32 width, f32 height) {
	sprite_t* sprite = malloc(sizeof(sprite_t));
	sprite->width = width;
	sprite->height = height;
	sprite->texture = NULL;

	guMtxIdentity(sprite->transform.matrix);

	sprite->transform.position = (guVector) { x, y, 0 };
	sprite->transform.scale = (guVector) { 1, 1, 1 };
	guQuaternion rotation;
	EulerToQuaternion(&rotation, 0, 0, 0);
	sprite->transform.rotation = rotation;
	sprite->transform.dirty = TRUE;

	return sprite;
}

void SPRITE_setTexture(sprite_t* sprite, GXTexObj* texture) {
	sprite->texture = texture;
}

void SPRITE_flush(sprite_t* sprite) {
	if (sprite->transform.dirty == TRUE) {
		MakeMatrix(&sprite->transform);
	}
}

void SPRITE_render(sprite_t* sprite) {
	if (sprite->texture == NULL) return;

	/* Check dirty flag */
	SPRITE_flush(sprite);

	/* Set position */
	GX_LoadPosMtxImm(sprite->transform.matrix, GX_PNMTX0);

	/* Set sprite texture */
	GX_LoadTexObj(sprite->texture, GX_TEXMAP0);

	/* Vtx descriptors reset and set */
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	/* Lighting off, Alpha blend */
	GX_SetNumChans(1);
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	/* Orthographic mode */
	GXU_2DMode();

	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
	
	/* Top left */
	GX_Position2f32(0, 0);
	GX_TexCoord2f32(0, 0);
	/* Bottom left */
	GX_Position2f32(0, sprite->height);
	GX_TexCoord2f32(0, 1);
	/* Bottom right */
	GX_Position2f32(sprite->width, sprite->height);
	GX_TexCoord2f32(1, 1);
	/* Top right */
	GX_Position2f32(sprite->width, 0);
	GX_TexCoord2f32(1, 0);

	GX_End();
}

