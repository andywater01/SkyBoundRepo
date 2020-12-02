#include "VertexTypes.h"
#pragma warning( push )

VertexPosCol* VPC = nullptr;
VertexPosNormCol* VPNC = nullptr;
VertexPosNormTex* VPNT = nullptr;
VertexPosNormTexCol* VPNTC = nullptr;
VertexPosNormTexColAnim1* VPNTCA1 = nullptr;

//Make one for one frame and one for another

const std::vector<BufferAttribute> VertexPosCol::V_DECL = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosCol), (size_t)&VPC->Position, AttribUsage::Position),
	BufferAttribute(1, 4, GL_FLOAT, false, sizeof(VertexPosCol), (size_t)&VPC->Color, AttribUsage::Color),
};
const std::vector<BufferAttribute> VertexPosNormCol::V_DECL = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormCol), (size_t)&VPNC->Position, AttribUsage::Position),
	BufferAttribute(1, 4, GL_FLOAT, false, sizeof(VertexPosNormCol), (size_t)&VPNC->Color, AttribUsage::Color),
	BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormCol), (size_t)&VPNC->Normal, AttribUsage::Normal),
};
const std::vector<BufferAttribute> VertexPosNormTex::V_DECL = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormTex), (size_t)&VPNT->Position, AttribUsage::Position),
	BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormTex), (size_t)&VPNT->Normal, AttribUsage::Normal),
	BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTex), (size_t)&VPNT->UV, AttribUsage::Texture),
};
const std::vector<BufferAttribute> VertexPosNormTexCol::V_DECL = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Position, AttribUsage::Position),
	BufferAttribute(1, 4, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Color, AttribUsage::Color), 
	BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->Normal, AttribUsage::Normal),
	BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTexCol), (size_t)&VPNTC->UV, AttribUsage::Texture),
};
const std::vector<BufferAttribute> VertexPosNormTexColAnim1::V_Anim1 = {
	BufferAttribute(0, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->Position, AttribUsage::Position),
	BufferAttribute(4, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->Position2, AttribUsage::Position2),
	BufferAttribute(1, 4, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->Color, AttribUsage::Color),
	BufferAttribute(2, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->Normal, AttribUsage::Normal),
	BufferAttribute(5, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->Normal2, AttribUsage::Normal2),
	BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim1), (size_t)&VPNTCA1->UV, AttribUsage::Texture),
};
const std::vector<BufferAttribute> VertexPosNormTexColAnim2::V_Anim2 = {
	BufferAttribute(4, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim2), (size_t)&VPNTC->Position, AttribUsage::Position2),
	BufferAttribute(1, 4, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim2), (size_t)&VPNTC->Color, AttribUsage::Color),
	BufferAttribute(5, 3, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim2), (size_t)&VPNTC->Normal, AttribUsage::Normal2),
	BufferAttribute(3, 2, GL_FLOAT, false, sizeof(VertexPosNormTexColAnim2), (size_t)&VPNTC->UV, AttribUsage::Texture),
};
#pragma warning(pop)