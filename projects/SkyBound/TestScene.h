#pragma once
#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/Shader.h"
#include "Graphics/TextureCubeMap.h"
#include "Graphics/TextureCubeMapData.h"
#include "Gameplay/ShaderMaterial.h"
#include "Gameplay/IBehaviour.h"

#include "Gameplay/Camera.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Application.h"
#include "Gameplay/RendererComponent.h"
#include "Gameplay/GameObjectTag.h"

#include "Behaviours/SimpleMoveBehaviour.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Gameplay/Transform.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Texture2DData.h"
#include "Utilities/InputHelpers.h"
#include "Utilities/MeshBuilder.h"
#include "Utilities/MeshFactory.h"
#include "Utilities/NotObjLoader.h"
#include "Utilities/ObjLoader.h"
#include "Utilities/VertexTypes.h"
#include "src/Utilities/SceneManager.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

#define LOG_GL_NOTIFICATIONS

class TestScene : public SceneManager
{
public:
	TestScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
};

