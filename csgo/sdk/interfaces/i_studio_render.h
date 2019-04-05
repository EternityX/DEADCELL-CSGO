#pragma once

class i_studio_render {
public:
	virtual void unk0() = 0;
	virtual void unk1() = 0;
	virtual void unk2() = 0;
	virtual void unk3() = 0;
	virtual void unk4() = 0;
	virtual void unk5() = 0;
	virtual void unk6() = 0;
	virtual void unk7() = 0;
	virtual void unk8() = 0;
	virtual void unk9() = 0;
	virtual void unk10() = 0;
	virtual void unk11() = 0;
	virtual void unk12() = 0;
	virtual void unk13() = 0;
	virtual void unk14() = 0;
	virtual void unk15() = 0;
	virtual void unk16() = 0;
	virtual void unk17() = 0;
	virtual void unk18() = 0;
	virtual void unk19() = 0;
	virtual void unk20() = 0;
	virtual void unk21() = 0;
	virtual void unk22() = 0;
	virtual void unk23() = 0;
	virtual void unk24() = 0;
	virtual void unk25() = 0;
	virtual void unk26() = 0;
	virtual void unk27() = 0;
	virtual void unk28() = 0;
	virtual void draw_model( void *pResults, const model_render_info_t &info, matrix3x4_t *pBoneToWorld, float *pFlexWeights,
	                        float *pFlexDelayedWeights, const vec3_t &modelOrigin,
	                        int flags = STUDIORENDER_DRAW_ENTIRE_MODEL ) = 0;
	virtual void unk30() = 0;
	virtual void unk31() = 0;
	virtual void unk32() = 0;
	virtual void forced_material_override( i_material *newMaterial, override_type_t nOverrideType = OVERRIDE_NORMAL ) = 0;
};
