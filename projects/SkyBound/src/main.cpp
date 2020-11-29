#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <Attributes.h>

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

#include <Windows.h>
#include <MMSystem.h>

#pragma comment(lib, "winmm.lib")

#include <fmod.h>


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
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"


#define LOG_GL_NOTIFICATIONS

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
		#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
		#endif
	default: break;
	}
}

GLFWwindow* window;
Camera::sptr camera = nullptr;

Attributes PhantomAttrib;
bool isRotate = true;
bool isLeft = true;
int CoinCount = 0;

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera->ResizeWindow(width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	
	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "Skybound", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}

void InitImGui() {
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output 
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();

	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void ShutdownImGui()
{
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

std::vector<std::function<void()>> imGuiCallbacks;
void RenderImGui() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();

	if (ImGui::Begin("Debug")) {
		// Render our GUI stuff
		for (auto& func : imGuiCallbacks) {
			func();
		}
		ImGui::End();
	}
	
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(window, &width, &height);
	io.DisplaySize = ImVec2((float)width, (float)height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(window);
	}
}

void RenderVAO(
	const Shader::sptr& shader,
	const VertexArrayObject::sptr& vao,
	const glm::mat4& viewProjection,
	const Transform& transform)
{
	shader->SetUniformMatrix("u_ModelViewProjection", viewProjection * transform.LocalTransform());
	shader->SetUniformMatrix("u_Model", transform.LocalTransform());
	shader->SetUniformMatrix("u_NormalMatrix", transform.NormalMatrix());
	vao->Render();
}

void ManipulateTransformWithInput(Transform& transform, float dt) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transform.MoveLocal(0.0f, -1.0f * dt, 0.0f);
		camera->SetPosition(glm::vec3(0.0f, -1.0f * dt, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { 
		transform.MoveLocal(0.0f,  1.0f * dt, 0.0f);
		camera->SetPosition(glm::vec3(0.0f, 1.0f * dt, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		transform.MoveLocal(-1.0f * dt, 0.0f, 0.0f);
		camera->SetPosition(glm::vec3(-1.0f * dt, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		transform.MoveLocal( 1.0f * dt, 0.0f,  0.0f);
		camera->SetPosition(glm::vec3(1.0f * dt, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		transform.MoveLocal(0.0f, 0.0f,  1.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		transform.MoveLocal(0.0f, 0.0f, -1.0f * dt);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { 
		transform.RotateLocal(0.0f, -45.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		transform.RotateLocal(0.0f,  45.0f * dt,0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		transform.RotateLocal( 45.0f * dt, 0.0f,0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		transform.RotateLocal(-45.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		transform.RotateLocal(0.0f, 0.0f, 45.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		transform.RotateLocal(0.0f, 0.0f, -45.0f * dt);
	}
}


inline btVector3 glm2bt(const glm::vec3& vec)
{
	return { vec.x, vec.y, vec.z };
}



void PlayerInput(GameObject& transform, float dt, float speed, btRigidBody *body, btTransform phyTransform) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//transform->MoveLocal(0.0f, 0.0f, -1.0f * dt * speed);
		//transform.MoveLocalFixed(0.0f, -1.0f * dt * speed, 0.0f);
		transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, -1.0f * dt * speed, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 270.0f);
		//transform.SetLocalRotation(90.0f, 0.0f, 282.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(0.0f, 1.0f * dt * speed, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
		//body->activate(true);
		//body->setLinearVelocity(btVector3(0, 4, 0));
		//body->applyForce(btVector3(0, 1000, 0), btVector3(0, 1000, 0));
		//transform.MoveLocalFixed(0.0f, 1.0f * dt * speed, 0.0f);
		//transform.SetLocalRotation(90.0f, 0.0f, 102.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//transform->MoveLocal(1.0f * dt * speed, 0.0f, 0.0f);
		//transform.MoveLocalFixed(-1.0f * dt * speed, 0.0f, 0.0f);
		transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(-1.0f * dt * speed, 0.0f, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
		//transform.SetLocalRotation(90.0f, 0.0f, 192.0f);
		//camera->SetPosition(camera->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f) * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//transform->MoveLocal(-1.0f * dt * speed, 0.0f, 0.0f);
		//transform.MoveLocalFixed(1.0f * dt * speed, 0.0f, 0.0f);
		
		transform.get<Transform>().SetLocalPosition(transform.get<Transform>().GetLocalPosition() + glm::vec3(1.0f * dt * speed, 0.0f, 0.0f));
		transform.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
		//transform.SetLocalRotation(90.0f, 0.0f, 12.0f);
		//camera->SetPosition(camera->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//transform.MoveLocal(0.0f, 0.0f, 1.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//transform.MoveLocal(0.0f, 0.0f, -1.0f * dt);
	}
	
	//body->setWorldTransform(phyTransform);
}







struct Material
{
	Texture2D::sptr Albedo;
	Texture2D::sptr Specular;
	Texture2D::sptr NewTexture;

	float           Shininess;
	float			TextureMix;
};

void SetupShaderForFrame(const Shader::sptr& shader, const glm::mat4& view, const glm::mat4& projection) {
	shader->Bind();
	// These are the uniforms that update only once per frame
	shader->SetUniformMatrix("u_View", view);
	shader->SetUniformMatrix("u_ViewProjection", projection * view);
	shader->SetUniformMatrix("u_SkyboxMatrix", projection * glm::mat4(glm::mat3(view)));
	glm::vec3 camPos = glm::inverse(view) * glm::vec4(0, 0, 0, 1);
	shader->SetUniform("u_CamPos", camPos);
}

/*
inline btVector3 glm2bt(const glm::vec3& vec)
{
	return { vec.x, vec.y, vec.z };
}
*/

template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

bool gotCoin = false;


void GetDistance(GameObject player, GameObject object, glm::vec3 distance)
{
	distance = player.get<Transform>().GetLocalPosition() - object.get<Transform>().GetLocalPosition();

	if (distance.x <= 2.0f && distance.y <= 2.0f && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && gotCoin == false)
	{
		CoinCount = 1;
		object.get<Transform>().SetLocalPosition(100.0f, 100.0f, 100.0f);
		gotCoin = true;
	}
}

void MoveWizard(GameObject player, GameObject wizard, glm::vec3 distance2, int coinCount)
{
	distance2 = player.get<Transform>().GetLocalPosition() - wizard.get<Transform>().GetLocalPosition();

	if (distance2.x <= 2.0f && distance2.y <= 2.0f && coinCount >= 1 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && gotCoin == true)
	{
		CoinCount = 0;
		wizard.get<Transform>().SetLocalPosition(wizard.get<Transform>().GetLocalPosition() + glm::vec3(-35.0f, 4.0f, 0.0f));
	}
}






int main() {
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it

	//Initialize GLFW
	if (!initGLFW())
		return 1;

	//Initialize GLAD
	if (!initGLAD())
		return 1;

	//BG music
	//PlaySound(TEXT("Music/Island1Music.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);



	//initPhysics();

	
	//https://github.com/bulletphysics/bullet3/blob/master/examples/HelloWorld/HelloWorld.cpp
	//https://www.raywenderlich.com/2606-bullet-physics-tutorial-getting-started#toc-anchor-001

	// Build the broadphase
	//btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0.f, 0.f, 0.f));


	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	
	float planeHeight = 0.0f;

	/*
	//Plane
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, planeHeight));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 0, 1), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	dynamicsWorld->addRigidBody(body);
	*/

	//Player Physics
	btCollisionShape* playerShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));

	btTransform playerTransform;
	
	btScalar playerMass(2.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isPlayerDynamic = (playerMass != 0.f);

	btVector3 localPlayerInertia(0, 0, 0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* playerMotionState;
	btRigidBody* playerBody;


	//Wizard Physics
	btCollisionShape* wizardShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));

	btTransform wizardTransform;

	btScalar wizardMass(2.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isWizardDynamic = (wizardMass != 0.f);

	btVector3 localWizardInertia(0, 0, 0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* wizardMotionState;
	btRigidBody* wizardBody;


	////////////////// LERPING Variables ////////////////////////////////////////////////

	//Phantom Position Lerp
	glm::vec3 endPos = glm::vec3(-35.0f, -9.5f, -1.0f);
	glm::vec3 startPos = glm::vec3(-35.0f, 9.5f, -1.0f);

	float PhantomTimer = 0.0f;
	float PhantomTimeLimit = 4.0f;
	bool PhantomMove = true;

	//Phantom Rotation Lerp
	glm::quat startPhantomRot = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::quat endPhantomRot = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	float PhantomRotTimer = 0.0f;
	float PhantomRotTimeLimit = 4.0f;
	bool PhantomRot = true;
	bool flipPhantom = false;

	//////////////////////////////////////////////////////////////////////////////////////

	{


		// Let OpenGL know that we want debug output, and route it to our handler function
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(GlDebugMessage, nullptr);

		// Enable texturing
		glEnable(GL_TEXTURE_2D);


		// We'll use the provided mesh builder to build a new mesh with a few elements
		MeshBuilder<VertexPosNormTexCol> builder = MeshBuilder<VertexPosNormTexCol>();
		MeshFactory::AddCube(builder, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
		VertexArrayObject::sptr vao3 = builder.Bake();

		VertexArrayObject::sptr playerVao = ObjLoader::LoadFromFile("models/SkyBoundGuyCol.obj");

		VertexArrayObject::sptr islandVao = ObjLoader::LoadFromFile("models/Island1ObjectTex.obj");



		// Load our shaders
		Shader::sptr shader = Shader::Create();
		shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
		shader->LoadShaderPartFromFile("shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);
		shader->Link();

		glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 6.0f);
		glm::vec3 lightCol = glm::vec3(0.9f, 0.85f, 0.5f);
		float     lightAmbientPow = 0.05f;
		float     lightSpecularPow = 1.0f;
		glm::vec3 ambientCol = glm::vec3(1.0f);
		float     ambientPow = 0.8f;
		float     textureMix = 0.2f;
		float     shininess = 4.0f;
		float     lightLinearFalloff = 0.09f;
		float     lightQuadraticFalloff = 0.032f;
		float     outlineThickness = 0.15;
		
		// These are our application / scene level uniforms that don't necessarily update
		// every frame
		shader->SetUniform("u_LightPos", lightPos);
		shader->SetUniform("u_LightCol", lightCol);
		shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
		shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
		shader->SetUniform("u_AmbientCol", ambientCol);
		shader->SetUniform("u_AmbientStrength", ambientPow);
		shader->SetUniform("u_TextureMix", textureMix);
		shader->SetUniform("u_Shininess", shininess);
		shader->SetUniform("u_LightAttenuationConstant", 1.0f);
		shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
		shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);



		Texture2D::sptr PlayerDiffuse = Texture2D::LoadFromFile("images/SkyBoundCharUV2.png");
		Texture2D::sptr diffuseMp02 = Texture2D::LoadFromFile("images/GrassIslandColours.png");
		Texture2D::sptr diffuseMp03 = Texture2D::LoadFromFile("images/WizardColours.png");
		Texture2D::sptr diffuseMp04 = Texture2D::LoadFromFile("images/PhantomColours.png");
		Texture2D::sptr diffuseMp05 = Texture2D::LoadFromFile("images/CoinTex.png");

		Texture2DData::sptr specularMp02 = Texture2DData::LoadFromFile("images/Stone_001_Specular.png");

		// Load the cube map
		//TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("images/cubemaps/skybox/sample.jpg");
		TextureCubeMap::sptr environmentMap = TextureCubeMap::LoadFromImages("images/cubemaps/skybox/ocean.jpg");



		///////////////////////////////////// Scene Generation //////////////////////////////////////////////////
		#pragma region Scene Generation

		// We need to tell our scene system what extra component types we want to support
		GameScene::RegisterComponentType<RendererComponent>();
		GameScene::RegisterComponentType<BehaviourBinding>();
		GameScene::RegisterComponentType<Camera>();



		GameScene::sptr scene = GameScene::Create("test");
		Application::Instance().ActiveScene = scene;

		auto renderGroup = scene->Registry().group<RendererComponent, Transform>();

		ShaderMaterial::sptr material0 = ShaderMaterial::Create();
		material0->Shader = shader;
		material0->Set("s_Diffuse", PlayerDiffuse);
		//material0->Set("s_Diffuse2", Boxdiffuse2);
		//material0->Set("s_Specular", Boxspecular);
		material0->Set("u_Shininess", 8.0f);
		//material0->Set("u_TextureMix", 0.5f);
		//material0->Set("u_Reflectivity", 0.6f);

		ShaderMaterial::sptr material1 = ShaderMaterial::Create();
		material1->Shader = shader;
		material1->Set("s_Diffuse", diffuseMp02);
		material1->Set("u_Shininess", 8.0f);
		
		ShaderMaterial::sptr material2 = ShaderMaterial::Create();
		material2->Shader = shader;
		material2->Set("s_Diffuse", diffuseMp03);
		material2->Set("u_Shininess", 8.0f);

		ShaderMaterial::sptr material3 = ShaderMaterial::Create();
		material3->Shader = shader;
		material3->Set("s_Diffuse", diffuseMp04);
		material3->Set("u_Shininess", 8.0f);


		ShaderMaterial::sptr material4 = ShaderMaterial::Create();
		material4->Shader = shader;
		material4->Set("s_Diffuse", diffuseMp05);
		material4->Set("u_Shininess", 8.0f);

		//X = In and Out
		//Y = Left and Right
		//Z = up and down

		

		GameObject player = scene->CreateEntity("player");
		{
			VertexArrayObject::sptr PlayerVAO = ObjLoader::LoadFromFile("models/SkyBoundGuyCol.obj");
			

			player.emplace<RendererComponent>().SetMesh(PlayerVAO).SetMaterial(material0);
			player.get<Transform>().SetLocalPosition(0.5f, 0.5f, 5.1f);
			player.get<Transform>().SetLocalRotation(90.0f, 0.0f, 180.0f);
			player.get<Transform>().SetLocalScale(0.5f, 0.5f, 0.5f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(player);

			
			//Collision Stuff
			collisionShapes.push_back(playerShape);

			
			playerTransform.setIdentity();
			playerTransform.setOrigin(btVector3(0.5f, 0.5f, 0.1f));
			playerTransform.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(90.0f)));
			playerTransform.setRotation(btQuaternion(btVector3(0, 0, 1), btScalar(180.0f)));
			//playerTransform.setOrigin(glm2bt(player.get<Transform>().GetLocalPosition()));
			//playerTransform.setIdentity();
			player.get<Transform>().SetTransform(playerTransform);

			if (isPlayerDynamic)
				playerShape->calculateLocalInertia(playerMass, localPlayerInertia);

			
			

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			playerMotionState = new btDefaultMotionState(playerTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(playerMass, playerMotionState, playerShape, localPlayerInertia);
			playerBody = new btRigidBody(rbInfo);

			


			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(playerBody);
			
			//playerBody->applyGravity();

			//playerBody->setWorldTransform(playerTransform);

			//playerBody->isActive();
			//playerBody->getMotionState()->getWorldTransform(playerTransform);
			//float matrix[16];
			//playerTransform.setFromOpenGLMatrix(matrix);
			

			//add the body to the dynamics world
			//dynamicsWorld->addRigidBody(playerBody);

			
		}

		GameObject island1 = scene->CreateEntity("Island1");
		{
			VertexArrayObject::sptr Island1VAO = ObjLoader::LoadFromFile("models/Island1ObjectTex.obj");
			island1.emplace<RendererComponent>().SetMesh(Island1VAO).SetMaterial(material1);
			island1.get<Transform>().SetLocalPosition(0.0f, 0.0f, -10.0f);
			island1.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island1.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island1);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			/*
			btCollisionShape* island1Shape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

			collisionShapes.push_back(island1Shape);

			btTransform island1Transform;
			island1Transform.setIdentity();
			island1Transform.setOrigin(glm2bt(island1.get<Transform>().GetLocalPosition()));

			btScalar mass(0.);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isStatic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isStatic)
				island1Shape->calculateLocalInertia(mass, localInertia);

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(island1Transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, island1Shape, localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(body);
		*/

			GameObject island2 = scene->CreateEntity("Island2");
		
		
			island2.emplace<RendererComponent>().SetMesh(Island1VAO).SetMaterial(material1);
			island2.get<Transform>().SetLocalPosition(-35.0f, 0.0f, -10.0f);
			island2.get<Transform>().SetLocalRotation(-90.0f, 180.0f, 0.0f);
			island2.get<Transform>().SetLocalScale(2.0f, 2.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(island2);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}


		GameObject Wizard = scene->CreateEntity("Wizard");
		{
			VertexArrayObject::sptr WizardVAO = ObjLoader::LoadFromFile("models/BridgeKeeper.obj");
			Wizard.emplace<RendererComponent>().SetMesh(WizardVAO).SetMaterial(material2);
			Wizard.get<Transform>().SetLocalPosition(-8.0f, 0.5f, -2.5f);
			Wizard.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Wizard.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Wizard);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

			//Collision Stuff
			collisionShapes.push_back(wizardShape);

			wizardTransform.setIdentity();
			wizardTransform.setOrigin(glm2bt(Wizard.get<Transform>().GetLocalPosition()));

			if (isWizardDynamic)
				wizardShape->calculateLocalInertia(wizardMass, localWizardInertia);

			

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			wizardMotionState = new btDefaultMotionState(wizardTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(wizardMass, wizardMotionState, wizardShape, localWizardInertia);
			wizardBody = new btRigidBody(rbInfo);

			


			//playerBody->applyGravity();

			//playerBody->setWorldTransform(playerTransform);

			//playerBody->isActive();
			//playerBody->getMotionState()->getWorldTransform(playerTransform);
			//float matrix[16];
			//playerTransform.setFromOpenGLMatrix(matrix);


			//add the body to the dynamics world
			dynamicsWorld->addRigidBody(wizardBody);
		}


		GameObject Phantom = scene->CreateEntity("Phantom");
		{
			VertexArrayObject::sptr PhantomVAO = ObjLoader::LoadFromFile("models/Phantom2.obj");
			Phantom.emplace<RendererComponent>().SetMesh(PhantomVAO).SetMaterial(material3);
			Phantom.get<Transform>().SetLocalPosition(-35.0f, 9.5f, -1.0f);
			Phantom.get<Transform>().SetLocalRotation(90.0f, 0.0f, 0.0f);
			Phantom.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Phantom);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}


		GameObject Coin = scene->CreateEntity("Coin");
		{
			VertexArrayObject::sptr CoinVAO = ObjLoader::LoadFromFile("models/Coin/GameCoin.obj");
			Coin.emplace<RendererComponent>().SetMesh(CoinVAO).SetMaterial(material4);
			Coin.get<Transform>().SetLocalPosition(6.0f, -7.0f, -2.0f);
			Coin.get<Transform>().SetLocalRotation(90.0f, 0.0f, 90.0f);
			Coin.get<Transform>().SetLocalScale(1.0f, 1.0f, 1.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(Coin);
			//SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		}
		
		// Load a second material for our reflective material!
		Shader::sptr reflectiveShader = Shader::Create();
		reflectiveShader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
		reflectiveShader->LoadShaderPartFromFile("shaders/frag_reflection.frag.glsl", GL_FRAGMENT_SHADER);
		reflectiveShader->Link();

		ShaderMaterial::sptr reflectiveMat = ShaderMaterial::Create();
		reflectiveMat->Shader = reflectiveShader;
		reflectiveMat->Set("s_Environment", environmentMap);
		// TODO: send the rotation to apply to the skybox
		reflectiveMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
			glm::vec3(1, 0, 0))));
		
		#pragma endregion
		//////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////// Scene Generation 2 //////////////////////////////////////////////////
		#pragma region Scene Generation 2

		// We need to tell our scene system what extra component types we want to support
		GameScene::RegisterComponentType<RendererComponent>();
		GameScene::RegisterComponentType<BehaviourBinding>();
		GameScene::RegisterComponentType<Camera>();

		GameScene::sptr scene2 = GameScene::Create("Scene 2");
		Application::Instance().ActiveScene = scene2;

		auto renderGroup2 = scene2->Registry().group<RendererComponent, Transform>();



		//X = In and Out
		//Y = Left and Right
		//Z = up and down



		GameObject obj5 = scene2->CreateEntity("cube");
		{
			MeshBuilder<VertexPosNormTexCol> builder = MeshBuilder<VertexPosNormTexCol>();
			MeshFactory::AddCube(builder, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f));
			VertexArrayObject::sptr vao = builder.Bake();

			obj5.emplace<RendererComponent>().SetMesh(vao).SetMaterial(reflectiveMat);
			obj5.get<Transform>().SetLocalPosition(-4.0f, 0.0f, 2.0f);
			BehaviourBinding::BindDisabled<SimpleMoveBehaviour>(obj5);
		}

		#pragma endregion
		//////////////////////////////////////////////////////////////////////////////////////////

		{
			// Load our shaders 
			Shader::sptr skybox = std::make_shared<Shader>();
			skybox->LoadShaderPartFromFile("shaders/skybox-shader.vert.glsl", GL_VERTEX_SHADER);
			skybox->LoadShaderPartFromFile("shaders/skybox-shader.frag.glsl", GL_FRAGMENT_SHADER);
			skybox->Link();

			ShaderMaterial::sptr skyboxMat = ShaderMaterial::Create();
			skyboxMat->Shader = skybox;
			skyboxMat->Set("s_Environment", environmentMap);
			// TODO: send the rotation to apply to the skybox
			skyboxMat->RenderLayer = 100;
			skyboxMat->Set("u_EnvironmentRotation", glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
				glm::vec3(1, 0, 0))));


			MeshBuilder<VertexPosNormTexCol> mesh;
			MeshFactory::AddIcoSphere(mesh, glm::vec3(0.0f), 1.0f);
			MeshFactory::InvertFaces(mesh);
			VertexArrayObject::sptr meshVao = mesh.Bake();

			GameObject skyboxObj = scene->CreateEntity("skybox");
			skyboxObj.get<Transform>().SetLocalPosition(0.0f, 0.0f, 0.0f);
			skyboxObj.get_or_emplace<RendererComponent>().SetMesh(meshVao).SetMaterial(skyboxMat);
		}


		// We'll add some ImGui controls to control our shader
		imGuiCallbacks.push_back([&]() {
			if (ImGui::CollapsingHeader("Scene Level Lighting Settings"))
			{
				if (ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientCol))) {
					shader->SetUniform("u_AmbientCol", ambientCol);
				}
				if (ImGui::SliderFloat("Fixed Ambient Power", &ambientPow, 0.01f, 1.0f)) {
					shader->SetUniform("u_AmbientStrength", ambientPow);
				}
			}
			if (ImGui::CollapsingHeader("Light Level Lighting Settings"))
			{
				if (ImGui::DragFloat3("Light Pos", glm::value_ptr(lightPos), 0.01f, -10.0f, 10.0f)) {
					shader->SetUniform("u_LightPos", lightPos);
				}
				if (ImGui::ColorPicker3("Light Col", glm::value_ptr(lightCol))) {
					shader->SetUniform("u_LightCol", lightCol);
				}
				if (ImGui::SliderFloat("Light Ambient Power", &lightAmbientPow, 0.0f, 1.0f)) {
					shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
				}
				if (ImGui::SliderFloat("Light Specular Power", &lightSpecularPow, 0.0f, 1.0f)) {
					shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
				}
				if (ImGui::DragFloat("Light Linear Falloff", &lightLinearFalloff, 0.01f, 0.0f, 1.0f)) {
					shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
				}
				if (ImGui::DragFloat("Light Quadratic Falloff", &lightQuadraticFalloff, 0.01f, 0.0f, 1.0f)) {
					shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
				}
			}
			if (ImGui::CollapsingHeader("Material Level Lighting Settings"))
			{
				if (ImGui::SliderFloat("Shininess", &shininess, 0.1f, 128.0f)) {
					shader->SetUniform("u_Shininess", shininess);
				}
			}
			});

		// GL states
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);

		// NEW STUFF

		// Create some transforms and initialize them
		/*Transform playerTransform;
		Transform islandTransform;
		Transform islandTransform2;


		playerTransform = Transform::Create();
		islandTransform = Transform::Create();
		islandTransform2 = Transform::Create();

		Transform::sptr transforms[4];
		transforms[0] = Transform::Create();
		transforms[1] = Transform::Create();
		transforms[2] = Transform::Create();
		transforms[3] = Transform::Create();

		// We can use operator chaining, since our Set* methods return a pointer to the instance, neat!
		playerTransform->SetLocalPosition(0.0f, 10.0f, 0.0f)->SetLocalRotation(90.0f, 0.0f, 192.0f)->SetLocalScale(0.5f, 0.5f, 0.5f);
		islandTransform->SetLocalPosition(-40.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);
		islandTransform2->SetLocalPosition(-175.0f, 0.0f, -50.0f)->SetLocalRotation(90.0f, 0.0f, 0.0f)->SetLocalScale(8.0f, 8.0f, 8.0f);

		transforms[1]->SetLocalPosition(2.0f, 0.0f, 0.5f)->SetLocalRotation(00.0f, 0.0f, 45.0f);
		transforms[2]->SetLocalPosition(-2.0f, 0.0f, 0.5f)->SetLocalRotation(00.0f, 0.0f, -45.0f);
		transforms[3]->SetLocalPosition(0.0f, 0.0f, 0.5f)->SetLocalRotation(00.0f, 0.0f, 0.0f);*/

		// We'll store all our VAOs into a nice array for easy access
		VertexArrayObject::sptr vaos[4];
		vaos[3] = vao3;



		// Load our texture data from a file
		Texture2DData::sptr diffuseMp = Texture2DData::LoadFromFile("images/SkyBoundCharUV2.png");
		Texture2DData::sptr diffuseMp2 = Texture2DData::LoadFromFile("images/GrassIslandColours.png");
		Texture2DData::sptr specularMp = Texture2DData::LoadFromFile("images/Stone_001_Specular.png");

		// Create a texture from the data
		Texture2D::sptr diffuse = Texture2D::Create();
		diffuse->LoadData(diffuseMp);

		// Create a texture from the data
		Texture2D::sptr diffuse2 = Texture2D::Create();
		diffuse2->LoadData(diffuseMp2);

		Texture2D::sptr specular = Texture2D::Create();
		specular->LoadData(specularMp);

		// Creating an empty texture
		Texture2DDescription desc = Texture2DDescription();
		desc.Width = 1;
		desc.Height = 1;
		desc.Format = InternalFormat::RGB8;
		Texture2D::sptr texture2 = Texture2D::Create(desc);
		texture2->Clear();



		// We'll use a temporary lil structure to store some info about our material (we'll expand this later)
		Material materials[4];
		Material playerMaterial;
		Material islandMaterial;

		playerMaterial.Albedo = diffuse;
		playerMaterial.Specular = specular;
		playerMaterial.Shininess = 4.0f;

		islandMaterial.Albedo = diffuse2;
		islandMaterial.Specular = specular;
		islandMaterial.Shininess = 4.0f;

		materials[3].Albedo = diffuse;
		materials[3].NewTexture = diffuse2;
		materials[3].Specular = specular;
		materials[3].Shininess = 64.0f;
		materials[3].TextureMix = 0.6f;

		camera = Camera::Create();
		camera->SetPosition(glm::vec3(4.0f, 1.2f, 2.0f)); // Set initial position
		camera->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
		camera->LookAt(glm::vec3(0.0f)); // Look at center of the screen
		camera->SetFovDegrees(90.0f); // Set an initial FOV
		camera->SetOrthoHeight(3.0f);

		// We'll use a vector to store all our key press events for now
		std::vector<KeyPressWatcher> keyToggles;
		// This is an example of a key press handling helper. Look at InputHelpers.h an .cpp to see
		// how this is implemented. Note that the ampersand here is capturing the variables within
		// the scope. If you wanted to do some method on the class, your best bet would be to give it a method and
		// use std::bind
		keyToggles.emplace_back(GLFW_KEY_T, [&]() { camera->ToggleOrtho(); });

		int selectedVao = 3; // select cube by default
		keyToggles.emplace_back(GLFW_KEY_KP_ADD, [&]() {
			selectedVao++;
			if (selectedVao >= 4)
				selectedVao = 1;
			});
		keyToggles.emplace_back(GLFW_KEY_KP_SUBTRACT, [&]() {
			selectedVao--;
			if (selectedVao <= 0)
				selectedVao = 3;
			});
		
		

		InitImGui();

		// Our high-precision timer
		double lastFrame = glfwGetTime();

		//Speed Variables
		float speed = 3.0f;
		float speedTimer = 0.0f;
		float speedTimeLimit = 2.0f;
		bool canSprint = true;
		glm::vec3 CoinDistance = glm::vec3();
		glm::vec3 WizardDistance = glm::vec3();
		
		///// Game loop /////
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			// Calculate the time since our last frame (dt)
			double thisFrame = glfwGetTime();
			float dt = static_cast<float>(thisFrame - lastFrame);

			std::cout << std::to_string(CoinCount) << std::endl;

			dynamicsWorld->stepSimulation(1.f / 60.f, 10);

			//print positions of all objects
			for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
				btRigidBody* body = btRigidBody::upcast(obj);
				btTransform trans;
				if (body && body->getMotionState())
				{
					body->getMotionState()->getWorldTransform(trans);
				}
				else
				{
					trans = obj->getWorldTransform();
				}
				printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			}
			

			
			//Gravity
			if (!(player.get<Transform>().GetLocalPosition().z <= planeHeight))
			{
				player.get<Transform>().SetLocalPosition(player.get<Transform>().GetLocalPosition() - glm::vec3(0.0f, 0.0f, 2.0f * dt));
			}

			//player.get<Transform>().SetLocalPosition(player.get<Transform>().GetLocalPosition() - glm::vec3(0.0f, 0.0f, 2.0f * dt));
			

			//Updating Physics Body
			//playerTransform = playerBody->getCenterOfMassTransform();
			//playerTransform.setOrigin(glm2bt(player.get<Transform>().GetLocalPosition()));
			//playerBody->setCenterOfMassTransform(playerTransform);

			//wizardTransform = wizardBody->getCenterOfMassTransform();
			//wizardTransform.setOrigin(glm2bt(Wizard.get<Transform>().GetLocalPosition()));
			//wizardBody->setCenterOfMassTransform(wizardTransform);
			
			
			
			//playerTransform.setFromOpenGLMatrix(glm::value_ptr(player.get<Transform>().LocalTransform()));
			//playerBody->getMotionState()->setWorldTransform(playerTransform);

			/*
			btTransform playerT = playerBody->getCenterOfMassTransform();
			playerT.setOrigin(glm2bt(player.get<Transform>().GetLocalPosition()));
			playerBody->setWorldTransform(playerT);

			btTransform wizardT = wizardBody->getCenterOfMassTransform();
			wizardT.setOrigin(glm2bt(Wizard.get<Transform>().GetLocalPosition()));
			wizardBody->setWorldTransform(wizardT);


			playerT.setFromOpenGLMatrix(glm::value_ptr(player.get<Transform>().LocalTransform()));
			playerBody->getMotionState()->setWorldTransform(playerT);

			wizardT.setFromOpenGLMatrix(glm::value_ptr(Wizard.get<Transform>().LocalTransform()));
			wizardBody->getMotionState()->setWorldTransform(wizardT);
			*/

			

			

			//Phantom LERP Position
			PhantomTimer += dt;

			if (PhantomTimer >= PhantomTimeLimit)
			{
				PhantomTimer = 0.0f;
				PhantomMove = !PhantomMove;
			}

			float phantomTPos = PhantomTimer / PhantomTimeLimit;

			if (PhantomMove == true)
			{
				Phantom.get<Transform>().SetLocalPosition(LERP(startPos, endPos, phantomTPos));
			}
			else if (PhantomMove == false)
			{
				Phantom.get<Transform>().SetLocalPosition(LERP(endPos, startPos, phantomTPos));
			}


			if (Phantom.get<Transform>().GetLocalPosition() == startPos)
			{
				if (flipPhantom)
				{
					Phantom.get<Transform>().SetLocalScale(Phantom.get<Transform>().GetLocalScale() * glm::vec3(1.0f, 1.0f, -1.0f));
				}
			}
			
			if (Phantom.get<Transform>().GetLocalPosition() == endPos)
			{
				flipPhantom = true;

				if (flipPhantom)
				{
					Phantom.get<Transform>().SetLocalScale(Phantom.get<Transform>().GetLocalScale() * glm::vec3(1.0f, 1.0f, -1.0f));
				}
			}


			//LerpMove(Phantom, PhantomTimer, tPos, PhantomAttrib, endPos, startPos);
			//std::cout << (endPos.y) << std::endl;
			//std::cout << (Phantom.get<Transform>().GetLocalPosition().y) << std::endl;

			


			

			// We'll make sure our UI isn't focused before we start handling input for our game
			if (!ImGui::IsAnyWindowFocused()) {
				// We need to poll our key watchers so they can do their logic with the GLFW state
				// Note that since we want to make sure we don't copy our key handlers, we need a const
				// reference!
				for (const KeyPressWatcher& watcher : keyToggles) {
					watcher.Poll(window);
				}

				// We'll run some basic input to move our transform around
				//ManipulateTransformWithInput(transforms[selectedVao], dt);
				PlayerInput(player, dt, speed, playerBody, playerTransform);
				

				//Sprinting Function
				#pragma region Sprint Stuff
				if (canSprint)
				{
					if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
					{
						speed = 12.0f;

						speedTimer += dt;

						if (speedTimer >= speedTimeLimit)
						{
							speed = 3.0f;
							canSprint = false;
						}
					}
					if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
					{
						speed = 3.0f;
					}
				}

				if (!canSprint)
				{
					speedTimer -= dt;

					if (speedTimer <= 0.0f)
					{
						speedTimer == 0.0f;
						canSprint = true;
					}
				}
				
				#pragma endregion
			}

			

			GetDistance(player, Coin, CoinDistance);
			MoveWizard(player, Wizard, WizardDistance, CoinCount);



			glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader->Bind();
			// These are the uniforms that update only once per frame
			//shader->SetUniformMatrix("u_View", camera->GetView());
			//shader->SetUniform("u_CamPos", camera->GetPosition());

			// Tell OpenGL that slot 0 will hold the diffuse, and slot 1 will hold the specular
			shader->SetUniform("s_Diffuse", 0);
			shader->SetUniform("s_Specular", 1);
			shader->SetUniform("s_Diffuse2", 2);

			// Render all VAOs in our scene
			//for(int ix = 0; ix < 4; ix++) {
				// TODO: Apply materials

				// Apply material properties for each instance
				//materials[ix].Albedo->Bind(0);
				///materials[ix].Specular->Bind(1);
				///materials[ix].NewTexture->Bind(2);
				//shader->SetUniform("u_Shininess", materials[ix].Shininess);
				//shader->SetUniform("u_TextureMix", materials[ix].TextureMix);
				//shader->SetUniform("u_TextureMix", materials[ix].TextureMix);

				//RenderVAO(shader, vaos[ix], camera, transforms[ix]);			
			//}


			//RenderVAO(shader, vao3, camera, transforms[3]);


			playerMaterial.Albedo->Bind(0);
			playerMaterial.Specular->Bind(1);
			//RenderVAO(shader, playerVao, camera, playerTransform);


			islandMaterial.Albedo->Bind(0);
			islandMaterial.Specular->Bind(1);
			//RenderVAO(shader, islandVao, camera, islandTransform);

			islandMaterial.Albedo->Bind(0);
			islandMaterial.Specular->Bind(1);
			//RenderVAO(shader, islandVao, camera, islandTransform2);



			camera->SetPosition(player.get<Transform>().GetLocalPosition() + glm::vec3(6.0f, 0.0f, 2.5f));
			camera->SetRotation(glm::vec3(-95.0f, 0.0f, 0.0f));
			


			
			


			

			//Transform& camTransform = cameraObject.get<Transform>();
			glm::mat4 view = (camera->GetView());
			glm::mat4 projection = camera->GetProjection();
			glm::mat4 viewProjection = projection * view;

			// Sort the renderers by shader and material, we will go for a minimizing context switches approach here,
			// but you could for instance sort front to back to optimize for fill rate if you have intensive fragment shaders
			renderGroup.sort<RendererComponent>([](const RendererComponent& l, const RendererComponent& r) {
				// Sort by render layer first, higher numbers get drawn last
				if (l.Material->RenderLayer < r.Material->RenderLayer) return true;
				if (l.Material->RenderLayer > r.Material->RenderLayer) return false;

				// Sort by shader pointer next (so materials using the same shader run sequentially where possible)
				if (l.Material->Shader < r.Material->Shader) return true;
				if (l.Material->Shader > r.Material->Shader) return false;

				// Sort by material pointer last (so we can minimize switching between materials)
				if (l.Material < r.Material) return true;
				if (l.Material > r.Material) return false;

				return false;
				});

			// Start by assuming no shader or material is applied
			Shader::sptr current = nullptr;
			ShaderMaterial::sptr currentMat = nullptr;

			// Iterate over the render group components and draw them
			renderGroup.each([&](entt::entity, RendererComponent& renderer, Transform& transform) {
				// If the shader has changed, bind it and set up it's uniforms
				if (current != renderer.Material->Shader) {
					current = renderer.Material->Shader;
					current->Bind();
					SetupShaderForFrame(current, view, projection);
				}
				// If the material has changed, apply it
				if (currentMat != renderer.Material) {
					currentMat = renderer.Material;
					currentMat->Apply();
				}
				// Render the mesh
				RenderVAO(renderer.Material->Shader, renderer.Mesh, viewProjection, transform);
			});
			
			
			/// Do some simulations
			/*
			///-----stepsimulation_start-----
			for (int i = 0; i < 150; i++)
			{
				dynamicsWorld->stepSimulation(1.f / 60.f, 10);

				//print positions of all objects
				for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
				{
					btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
					btRigidBody* body = btRigidBody::upcast(obj);
					btTransform trans;
					if (body && body->getMotionState())
					{
						body->getMotionState()->getWorldTransform(trans);
					}
					else
					{
						trans = obj->getWorldTransform();
					}
					printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
				}
			}
			*/

			///-----stepsimulation_end-----

			//cleanup in the reverse order of creation/initialization
			

			


			RenderImGui();   

			glfwSwapBuffers(window);
			lastFrame = thisFrame;    
		}
		
		
		//deletePhysics();
		///-----cleanup_start-----
		
		//remove the rigidbodies from the dynamics world and delete them
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//delete collision shapes
		for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			collisionShapes[j] = 0;
			delete shape;
		}
		
		//delete dynamics world
		delete dynamicsWorld;

		//delete solver
		delete solver;

		//delete broadphase
		delete overlappingPairCache;

		//delete dispatcher
		delete dispatcher;

		delete collisionConfiguration;
		

		ShutdownImGui();

		// Clean up the toolkit logger so we don't leak memory
		Application::Instance().ActiveScene = nullptr;
	}
	Logger::Uninitialize();
	return 0;
}