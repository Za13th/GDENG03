#include "AppWindow.h"
#include "structs.h"
#include <Windows.h>
#include <iostream>
#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "SceneCameraHolder.h"
#include "FogSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "BaseComponentSystem.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "GameObjectManager.h"
#include "DebugUIManager.h"
#include "GameStateManager.h"
#include "JSONManager.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <cstdlib>
#include <ctime>
#include <string>

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	GraphicsEngine::get()->getDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	GraphicsEngine::get()->getDevice()->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

// Open and read a file, then forward to LoadTextureFromMemory()
bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	FILE* f = fopen(file_name, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1)
		return false;
	fseek(f, 0, SEEK_SET);
	void* file_data = IM_ALLOC(file_size);
	fread(file_data, 1, file_size, f);
	fclose(f);
	bool ret = LoadTextureFromMemory(file_data, file_size, out_srv, out_width, out_height);
	IM_FREE(file_data);
	return ret;
}


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow* AppWindow::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new AppWindow();
		sharedInstance->init();
	}
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
	sharedInstance->init();

}

void AppWindow::destroy()
{
	if (sharedInstance != NULL)
		sharedInstance->release();
}

AppWindow::AppWindow()
{

}

void AppWindow::update()
{
	float GameSpeed = 1.57f; // Game speed multiplier (1.57f is the default.)
	m_angle += GameSpeed * EngineTime::getDeltaTime();
	constant cc;
	cc.m_angle = m_angle;


	m_delta_pos += EngineTime::getDeltaTime() / 10.0f;
	if (m_delta_pos > 1.0f) m_delta_pos = 0.0f;


	Matrix4x4 temp;

	m_delta_scale += EngineTime::getDeltaTime() / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_delta_scale) + 1.0f)/2.0f));



	cc.m_world.setScale(Vector3D(m_scale_cube));

	temp.setTranslation(Vector3D(5, 0, 0));
	cc.m_world *= temp;
	//temp.setRotationZ(0.0f);
	//cc.m_world *= temp;
	//temp.setRotationY(m_rot_y);
	//cc.m_world *= temp;
	//temp.setRotationX(m_rot_x);
	//cc.m_world *= temp;



/*
		Matrix4x4 world_cam;
		world_cam.setIdentity();

		temp.setRotationX(m_rot_x, true);
		world_cam *= temp;
		temp.setRotationY(m_rot_y, true);
		world_cam *= temp;

		Vector3D new_pos = m_world_cam.getTranslation() + m_world_cam.getZDirection() * (m_forward * 2.0f *EngineTime::getDeltaTime());
		new_pos = new_pos + m_world_cam.getXDirection() * (m_rightward * 2.0f * EngineTime::getDeltaTime());
		world_cam.setTranslation(new_pos, false);

		m_world_cam = world_cam;

		world_cam.inverse();
*/
//cc.m_world.setIdentity();
	auto world_cam = SceneCameraHolder::getInstance()->getCamera()->getViewMatrix();
	world_cam.inverse();
	cc.m_view = world_cam;
	//cc.m_view.setIdentity();

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	cc.m_view = world_cam;



	//cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);
	cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);



	this->m_cb->update(GraphicsEngine::get()->getDeviceContext(), &cc);



}



void AppWindow::createGraphicsWindow()
{

	SceneCameraHolder::initialize();
	//InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(true);

	GraphicsEngine::initialize();
	BaseComponentSystem::getInstance()->initialize();
	auto P6 = BaseComponentSystem::getInstance()->getPhysicsSystem();



	m_texture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_mesh[0] = MeshManager::getInstance()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");
	m_mesh[1] = MeshManager::getInstance()->createMeshFromFile(L"Assets\\Meshes\\bunny.obj");
	m_mesh[2] = MeshManager::getInstance()->createMeshFromFile(L"Assets\\Meshes\\armadillo.obj");



	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	std::cout << "Window rect width: " << width << std::endl;
	std::cout << "Window rect height: " << height << std::endl;

	SceneCameraHolder::getInstance()->getCamera()->height = height;
	SceneCameraHolder::getInstance()->getCamera()->width = width;

	this->m_swap_chain->init(this->m_hwnd, width, height);

	m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -2.0f), false);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	/*
	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ Vector3D(-0.5f, -0.5f, -0.5f) , Vector3D(1,0,0),  Vector3D(1,0,0)},
		{ Vector3D(-0.5f, 0.5f, -0.5f) ,   Vector3D(1,1,0),   Vector3D(1,1,0) },
		{ Vector3D(0.5f, 0.5f, -0.5f) ,  Vector3D(1,1,0), Vector3D(1,1,0) },
		{ Vector3D(0.5f, -0.5f, -0.5f),  Vector3D(1,0,0),    Vector3D(1,0,0)},

		{ Vector3D(0.5f, -0.5f, 0.5f) ,   Vector3D(0,1,0), Vector3D(0,1,0)},
		{ Vector3D(0.5f, 0.5f, 0.5f) ,   Vector3D(0,1,0),  Vector3D(0,1,0) },
		{ Vector3D(-0.5f, 0.5f, 0.5f) , Vector3D(0,1,1),   Vector3D(0,1,1) },
		{ Vector3D(-0.5f, -0.5f, 0.5f),  Vector3D(0,1,0),   Vector3D(0,1,1)}
	};

	//this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
	};



	//this->m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);*/


	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();

	//this->m_ib->load(index_list, size_index_list);
	//this->m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	this->m_cb->load(&cc, sizeof(constant));



	srand(time(0));


	/*Plane plane("Ground", shader_byte_code, size_shader);
	plane.setScale(Vector3D(10.0f, 1.0f, 10.0f));
	plane.setPosition(Vector3D(0.0f, -1.0f, 5.0f));
	this->planes.push_back(plane);*/


	GameObjectManager::initialize(shader_byte_code, size_shader);





	/*int preset = 0; // 0 for fog
	ParticleSystem::initialize();
	Particle templateParticle = Particle();
	if (preset == 0)
	{
		templateParticle.setColor(Vector3D(0.8f));

		ParticleSystem::getInstance()->particleMovementRandomDown = true;
		ParticleSystem::getInstance()->interval = 0.005f;
		ParticleSystem::getInstance()->max_size = 100;
		ParticleSystem::getInstance()->spawnAreaCenter = { 0.f,0.88f,0.0f };
		ParticleSystem::getInstance()->spawnAreaWidth = 2.20f;
		ParticleSystem::getInstance()->spawnAreaHeight = 0.10f;
	}
	else if (preset == 1)
	{
		templateParticle.setColor(Vector3D(0.15f));
		templateParticle.setColor(Vector3D(0.5, 0.1f, 0.1f), 7);
		templateParticle.setColor1(Vector3D(0.7, 0, 0.f));
		templateParticle.setColor1(Vector3D(0.35f), 0);
		templateParticle.lifeTimeLimit = 10.0f;

		ParticleSystem::getInstance()->interval = 0.01f;
		ParticleSystem::getInstance()->max_size = 500;
		ParticleSystem::getInstance()->spawnAreaCenter = { 0.f,-0.13f,0.0f };
		ParticleSystem::getInstance()->particleMovementRandomUp = true;
	}
	else if (preset == 2)
	{
		ParticleSystem::getInstance()->interval= 0.001f;
		ParticleSystem::getInstance()->max_size = 2000;
		ParticleSystem::getInstance()->spawnAreaWidth = 1.80f;
		ParticleSystem::getInstance()->spawnAreaHeight = 1.80f;
		ParticleSystem::getInstance()->particleMovementRandom = true;
	}
	else
	{
		ParticleSystem::getInstance()->max_size = 100;
		ParticleSystem::getInstance()->particleMovementRandom = true;
	}
*/
//ParticleSystem::getInstance()->Create(templateParticle,&shader_byte_code, &size_shader);
	FogSystem::initialize();


	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(this->m_hwnd);
	ImGui_ImplDX11_Init(GraphicsEngine::getInstance()->getDevice(), GraphicsEngine::getInstance()->getDeviceContext()->getDeviceContext());

}

void AppWindow::onCreate()
{
	Window::onCreate();
}

void AppWindow::onUpdate()
{
	//FogSystem::getInstance()->update();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool totalFog = false;
	static bool visibleParticles = false;
	static bool transparentBackground = false;

	static int my_image_width = 0;
	static int my_image_height = 0;
	static ID3D11ShaderResourceView* my_texture = nullptr;
	static bool first = true;

	if (first)
	{
		bool ret = LoadTextureFromFile("dlsu.jpg", &my_texture, &my_image_width, &my_image_height);
		IM_ASSERT(ret);
		first = false;
	}






	// === ENHANCED IMGUI INTERFACE ===
	ImGuiWindowFlags flags = 0;

	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Game State", nullptr, flags);
	

	if (GameStateManager::getInstance()->getGameState() == GameStateManager::Play)
	{
		ImGui::Text("Playing...");

		if (ImGui::Button("Pause"))
		{
			GameStateManager::getInstance()->setGameState(GameStateManager::Pause);
		}
	}
	else
	{
		ImGui::Text("Paused");
		if (ImGui::Button("Play"))
		{
			GameStateManager::getInstance()->setGameState(GameStateManager::Play);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Frame Step"))
	{
		GameStateManager::getInstance()->setGameState(GameStateManager::FrameStep);
	}
	ImGui::End();

	//Debug Log
	DebugUIManager::getInstance()->draw();


	// Main Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Save"))
			{
				JSONManager::getInstance()->save();
			}

			if (ImGui::MenuItem("Load"))
			{
				JSONManager::getInstance()->load();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	if(GameStateManager::getInstance()->getGameState() != GameStateManager::Play)
	GameObjectManager::getInstance()->getObjectSpawnUI();
	

	if(GameObjectManager::getInstance()->getAllGameObjects().size() > 0)
	GameObjectManager::getInstance()->getInspectorUI();





	Window::onUpdate();
	InputSystem::get()->update();
	if (FogSystem::getInstance()->getFogState() == 0)
		GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(135.f / 255.f), (float)(206.f / 255.f), (float)(255.f / 255.f), 1);
	else if (totalFog)
		GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(0.6f), (float)(0.6f), (float)(0.6f), 1);
	else
		GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(0.65f), (float)(0.65f), (float)(0.65f), 1);

	//GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(0.65f), (float)(0.65f), (float)(0.65f), 1);


	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	GraphicsEngine::get()->getDeviceContext()->setViewportSize(width, height);

	//SceneCameraHolder::getInstance()->updateCamera();
	SceneCameraHolder::getInstance()->getCamera()->update(EngineTime::getDeltaTime());

	/*

	this->update();
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getDeviceContext()->setTexture(this->m_ps, this->m_texture);

	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_mesh[0]->getVertexBuffer());
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_mesh[0]->getIndexBuffer());
	//Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_mesh[0]->getIndexBuffer()->getSizeIndexList(), 0, 0);

	*/


	/*	this->meshes[0].draw(width, height, this->m_vs, this->m_ps);
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_mesh[1]->getVertexBuffer());
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_mesh[1]->getIndexBuffer());
	//Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_mesh[1]->getIndexBuffer()->getSizeIndexList(), 0, 0);

	this->meshes[1].draw(width, height, this->m_vs, this->m_ps);
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_mesh[2]->getVertexBuffer());
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_mesh[2]->getIndexBuffer());
	//Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_mesh[2]->getIndexBuffer()->getSizeIndexList(), 0, 0);*/


	if (GameStateManager::getInstance()->getGameState() != GameStateManager::Pause)
		BaseComponentSystem::getInstance()->getPhysicsSystem()->updateAllComponents();

	GameObjectManager::getInstance()->drawObjects(EngineTime::getDeltaTime(), width, height, this->m_vs, this->m_ps);



	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_swap_chain->present(true);

	if (GameStateManager::getInstance()->getGameState() == GameStateManager::FrameStep)
		GameStateManager::getInstance()->setGameState(GameStateManager::Pause);

	if (InputSystem::get()->isKeyDown(VK_ESCAPE))
	{
		this->onDestroy();
		exit(0);
	}
}

void AppWindow::onDestroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Window::onDestroy();


	/*	if (this->m_vb != nullptr) //vertex buffers are part of the quad class instead
		this->m_vb->release();
	if (this->m_ib != nullptr);
		this->m_ib->release();
	if (this->m_cb != nullptr);
		this->m_cb->release();*/

	this->m_swap_chain->release();
	if (this->m_vs != nullptr)
		this->m_vs->release();
	if (this->m_ps != nullptr)
		this->m_ps->release();
	GraphicsEngine::get()->release();

	for (int i = 0; i < quads.size(); i++)
		quads[i].Release();

	GameObjectManager::destroy();


	TextureManager::getInstance()->destroy();
	MeshManager::getInstance()->destroy();
	BaseComponentSystem::getInstance()->destroy();
	//ParticleSystem::destroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
	InputSystem::get()->showCursor(true);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
		m_forward = 1.0f;
	//m_rot_x += 3.14f * EngineTime::getDeltaTime();
	else if (key == 'S')
		m_forward = -1.0f;
	//m_rot_x -= 3.14f * EngineTime::getDeltaTime();

	if (key == 'A')
		m_rightward = -1.0f;
	//m_rot_y -= 3.14f * EngineTime::getDeltaTime();
	else if (key == 'D')
		m_rightward = 1.0f;
	//m_rot_y += 3.14f * EngineTime::getDeltaTime();



}
void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	if (this->m_camera_rotation)
	{
		m_rot_x += (mouse_pos.y - (height / 2.0f)) * EngineTime::getDeltaTime() * 0.2f;
		m_rot_y += (mouse_pos.x - (width / 2.0f)) * EngineTime::getDeltaTime() * 0.2f;
		InputSystem::get()->setCursorPositon(Point(width / 2.0f, height / 2.0f)); // Reset cursor position to center of the window
	}


}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{

}

AppWindow::~AppWindow()
{

}