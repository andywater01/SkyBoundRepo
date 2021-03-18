#include <iostream>

#include <tiny_gltf.h>
#include <GLM/glm.hpp>

#pragma region Standard Graphics Code

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>

#include "Graphics/Shader.h"
#include "Graphics/VertexArrayObject.h"
#include "Utilities/VertexTypes.h"

#pragma endregion Standard Graphics Code

#include "SkinnedMesh.h"

#define FOV 1.0472f
#define ASPECT 1.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

#pragma region Standard Graphics Code

GLFWwindow* window;

bool InitGLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		printf("Failed to initialize GLFW");
		return false;
	}

	// Create a new GLFW window
	window = glfwCreateWindow(800, 800, "GDW II Skinned Mesh Animation", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	return true;
}

bool InitGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
	{
		printf("Failed to initialize GLAD");
		return false;
	}

	return true;
}

int main()
{
	// Initialize GLFW
	if (!InitGLFW())
	{
		return 1;
	}

	// Initialize GLAD
	if (!InitGLAD())
	{
		return 1;
	}
#pragma endregion Standard Graphics Code

	// INSERT LOADING SKINNED MESH HERE
	GLTFSkinnedMesh mesh;
	mesh.LoadFromFile("models/dude.gltf");
	mesh.SetLooping(true);

#pragma region Standard Graphics Code

		{
			// Load our shaders
			Shader::sptr shader = Shader::Create();
			shader->LoadShaderPartFromFile("shaders/skinned_vertex_shader.glsl", GL_VERTEX_SHADER);
			shader->LoadShaderPartFromFile("shaders/frag_blinn_phong.glsl", GL_FRAGMENT_SHADER);
			shader->Link();
	
			glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
			glm::vec3 lightCol = glm::vec3(0.9f, 0.85f, 0.5f);
			float	  lightAmbientPow = 0.05f;
			float	  lightSpecularPow = 1.0f;
			glm::vec3 ambientCol = glm::vec3(1.0f);
			float	  ambientPow = 0.1f;
	
			shader->SetUniform("u_LightPos", lightPos);
			shader->SetUniform("u_LightCol", lightCol);
			shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
			shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
			shader->SetUniform("u_AmbientCol", ambientCol);
			shader->SetUniform("u_AmbientStrength", ambientPow);
		

			glm::vec3 cameraPos(0, 2, 10);
			glm::vec3 cameraRot(0, 0, 0);

			glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), cameraPos) * glm::toMat4(glm::quat(cameraRot));
			glm::mat4 cameraProjection = glm::perspective(FOV, ASPECT, NEAR_PLANE, FAR_PLANE);
			glm::mat4 cameraViewProjection = cameraProjection * glm::inverse(cameraTransform);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);

			double currentFrame;
			double lastFrame = glfwGetTime();

			///// Game Loop /////
			while (!glfwWindowShouldClose(window))
			{
				currentFrame = glfwGetTime();
				float deltaTime = static_cast<float>(currentFrame - lastFrame);

				glfwPollEvents();
				


				// Clear the screen
				glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
				glEnable(GL_DEPTH_TEST);
				glClearDepth(1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
				// INSERT SKELETAL ANIMATION RENDERING HERE

				// Render our mesh!!
				mesh.UpdateAnimation(mesh.GetAnimation(1), deltaTime);
				glm::mat4 origin = glm::mat4(1.0f);
				mesh.Draw(shader, cameraViewProjection, origin);

				

				glfwSwapBuffers(window);
				lastFrame = currentFrame;
			}
		}
#pragma endregion Standard Graphics Code

	return 0;
}
