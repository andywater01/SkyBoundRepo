#pragma once

#include <GLM/glm.hpp>
#include "Graphics/VertexArrayObject.h"

struct VertexPosCol {
	glm::vec3 Position;
	glm::vec4 Color;

	VertexPosCol() : Position(glm::vec3(0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosCol(const glm::vec3& pos, const glm::vec4& col) :
		Position(pos), Color(col) {}
	VertexPosCol(float x, float y, float z, float r, float g, float b, float a = 1.0f) :
		Position({x, y, z}), Color({r, g, b, a}) {}

	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormCol {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Color;

	VertexPosNormCol() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormCol(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& col) :
		Position(pos), Normal(norm), Color(col) {}
	VertexPosNormCol(float x, float y, float z, float nX, float nY, float nZ, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Normal({nX, nY, nZ}), Color({ r, g, b, a }) {}
	
	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormTex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;

	VertexPosNormTex() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)) {}
	VertexPosNormTex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv) :
		Position(pos), Normal(norm), UV(uv) {}
	VertexPosNormTex(float x, float y, float z, float nX, float nY, float nZ, float u, float v) :
		Position({ x, y, z }), Normal({ nX, nY, nZ }), UV({ u, v }) {}

	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormTexCol {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec4 Color;

	VertexPosNormTexCol() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormTexCol(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv, const glm::vec4& col) :
		Position(pos), Normal(norm), UV(uv), Color(col) {}
	VertexPosNormTexCol(float x, float y, float z, float nX, float nY, float nZ, float u, float v, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Normal({ nX, nY, nZ }), UV({ u, v }), Color({r, g, b, a}) {}

	static const std::vector<BufferAttribute> V_DECL;
};



struct VertexPosNormTexColAnim1 {
	glm::vec3 Position;
	glm::vec3 Position2;
	glm::vec3 Normal;
	glm::vec3 Normal2;
	glm::vec2 UV;
	glm::vec4 Color;


	VertexPosNormTexColAnim1() : Position(glm::vec3(0.0f)), Position2(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), Normal2(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormTexColAnim1(const glm::vec3& pos, const glm::vec3& pos2, const glm::vec3& norm, const glm::vec3& norm2, const glm::vec2& uv, const glm::vec4& col) :
		Position(pos), Position2(pos2), Normal(norm), Normal2(norm2), UV(uv), Color(col) {}
	VertexPosNormTexColAnim1(float x, float y, float z, float x2, float y2, float z2, float nX, float nY, float nZ, float nX2, float nY2, float nZ2, float u, float v, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Position2({ x2, y2, z2 }), Normal({ nX, nY, nZ }), Normal2({ nX2, nY2, nZ2 }), UV({ u, v }), Color({ r, g, b, a }) {}

	static const std::vector<BufferAttribute> V_Anim1;
};

struct VertexPosNormTexColAnim2 {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec4 Color;

	VertexPosNormTexColAnim2() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormTexColAnim2(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv, const glm::vec4& col) :
		Position(pos), Normal(norm), UV(uv), Color(col) {}
	VertexPosNormTexColAnim2(float x, float y, float z, float nX, float nY, float nZ, float u, float v, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Normal({ nX, nY, nZ }), UV({ u, v }), Color({ r, g, b, a }) {}

	static const std::vector<BufferAttribute> V_Anim2;
};