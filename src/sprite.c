#include "sprite.h"
#include "mathutil.h"
#include <malloc.h>

void SPRITE_init() {

}

sprite_t* SPRITE_create() {
	sprite_t* sprite = malloc(sizeof(sprite_t));
	return sprite;
}

void SPRITE_setTexture(sprite_t* sprite, GXTexObj* texture, f32 position[2], f32 spriteSize[2], TexSize texSize) {
	sprite->texture = texture;
	f32 texRepr = 1.0f / texSize;
	f32 res[2] = { 0, 0 };
	f32 size[2] = { texRepr, texRepr };
	ps_float2Mul(position, size, res);
	sprite->UVoffsetX = res[0];
	sprite->UVoffsetY = res[1];
	ps_float2Mul(spriteSize, size, res);
	sprite->UVwidth = res[0];
	sprite->UVheight = res[1];
	sprite->transform.position = (guVector) { 0, 0, 0 };
	sprite->transform.scale = (guVector) { 1, 1, 1 }; 
	guQuaternion rotation;
	EulerToQuaternion(&rotation, 0, 0, 0);
	sprite->transform.rotation = rotation;

	MakeMatrix(&sprite->transform);
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

	/* Set position to identity */
	Mtx viewMtx, modelviewMtx;
	guMtxIdentity(viewMtx);
	guMtxConcat(viewMtx, sprite->transform.matrix, modelviewMtx);
	GX_LoadPosMtxImm(modelviewMtx, GX_PNMTX0);

	/* Set sprite texture */
	GX_LoadTexObj(sprite->texture, GX_TEXMAP0);

	//Msh settings
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
	GX_TexCoord2f32(sprite->UVoffsetX, sprite->UVoffsetY);
	/* Bottom left */
	GX_Position2f32(0, sprite->UVheight);
	GX_TexCoord2f32(sprite->UVoffsetX, sprite->UVoffsetY + sprite->UVheight);
	/* Bottom right */
	GX_Position2f32(sprite->UVwidth, sprite->UVheight);
	GX_TexCoord2f32(sprite->UVoffsetX + sprite->UVwidth, sprite->UVoffsetY + sprite->UVheight);
	/* Top right */
	GX_Position2f32(sprite->UVwidth, 0);
	GX_TexCoord2f32(sprite->UVoffsetX + sprite->UVwidth, sprite->UVoffsetY);

	GX_End();
}

