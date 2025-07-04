#include "AppWindow.h"
#include "structs.h"
#include <Windows.h>
#include <iostream>
#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "ParticleSystem.h"

#include <cstdlib>
#include <ctime>

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

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5, -1.5, 0), Vector3D(1.5, 1.5, 0), m_delta_pos));
	//cc.m_world *= temp;
			
	/*		cc.m_world.setScale(Vector3D(m_scale_cube));

		    temp.setRotationZ(0.0f);
			cc.m_world *= temp;
			temp.setRotationY(m_rot_y);
			cc.m_world *= temp;
			temp.setRotationX(m_rot_x);
			cc.m_world *= temp;
	*/		
			cc.m_world.setIdentity();
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





			cc.m_view = world_cam;
			//cc.m_view.setIdentity();

			RECT rc = this->getClientWindowRect();
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;

			//cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);
			cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);



			this->m_cb->update(GraphicsEngine::get()->getDeviceContext(), &cc);
		


	}



void AppWindow::createGraphicsWindow()
{

	//InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::initialize();

	this->m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	std::cout << "Window rect width: " << width << std::endl;
	std::cout << "Window rect height: " << height << std::endl;

	this->m_swap_chain->init(this->m_hwnd, width, height);

	m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -2.0f), false);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

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

	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
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



	this->m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();

	this->m_ib->load(index_list, size_index_list);
	this->m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	this->m_cb->load(&cc, sizeof(constant));


	
	/*Cube cube("Test", shader_byte_code, size_shader);
		this->cubes.push_back(cube);


		Plane plane("Test", shader_byte_code, size_shader);
		plane.setScale(Vector3D(2.0f, 1.0f, 1.0f));
		plane.setRotation(Vector3D(85.f,0.0f,0.0f));
		this->planes.push_back(plane);*/

	srand(time(0));
	int preset = 0;
	ParticleSystem::initialize();
	Particle templateParticle = Particle();
	if (preset == 0)
	{
		templateParticle.setColor(Vector3D(1));
		templateParticle.setColor(Vector3D(0.6f, 1, 1), 7);
		templateParticle.setColor1(Vector3D(0.5f, 1, 1));
		templateParticle.setColor1(Vector3D(0.95f), 0);

		ParticleSystem::getInstance()->interval = 0.1f;
		ParticleSystem::getInstance()->max_size = 100;
		ParticleSystem::getInstance()->spawnAreaCenter = { 0.f,0.9f,0.0f };
		ParticleSystem::getInstance()->spawnAreaWidth = 2.30f;
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
	else 
	{
		ParticleSystem::getInstance()->interval= 0.001f;
		ParticleSystem::getInstance()->max_size = 2000;
		ParticleSystem::getInstance()->spawnAreaWidth = 1.80f;
		ParticleSystem::getInstance()->spawnAreaHeight = 1.80f;
		ParticleSystem::getInstance()->particleMovementRandom = true;
	}

	ParticleSystem::getInstance()->Create(templateParticle,&shader_byte_code, &size_shader);


	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

}

void AppWindow::onCreate()
{
	Window::onCreate();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();             
	InputSystem::get()->update(); 
	GraphicsEngine::get()->getDeviceContext()->clearRenderTargetColor(this->m_swap_chain, (float)(135.f/255.f), (float)(206.f /255.f), (float)(255.f /255.f), 1);




	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	GraphicsEngine::get()->getDeviceContext()->setViewportSize(width, height);


	this->update();

	
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	GraphicsEngine::get()->getDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(this->m_ps);


	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vb);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_ib); 
	//Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_ib->getSizeIndexList(), 0, 0);

	for (int i = 0; i < quads.size(); i++)
		this->quads[i].draw(width, height, this->m_vs, this->m_ps);

	for(int i = 0 ; i < cubes.size(); i++)
		this->cubes[i].draw(width, height, this->m_vs, this->m_ps);

	for (int i = 0; i < planes.size(); i++)
		this->planes[i].draw(width, height, this->m_vs, this->m_ps);
	
	ParticleSystem::getInstance()->Update(EngineTime::getDeltaTime());
	if(InputSystem::get()->isKeyDown('P'))
	ParticleSystem::getInstance()->Draw(width, height, this->m_vs, this->m_ps);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	if (this->m_vb != nullptr) //vertex buffers are part of the quad class instead
		this->m_vb->release();
	if (this->m_ib != nullptr);
		this->m_ib->release();
	if (this->m_cb != nullptr);
		this->m_cb->release();
	this->m_swap_chain->release();
	if (this->m_vs != nullptr)
		this->m_vs->release();
	if (this->m_ps != nullptr)
		this->m_ps->release();
	GraphicsEngine::get()->release();

	for (int i = 0; i < quads.size(); i++)
		quads[i].Release();
	
	for (int i = 0; i < cubes.size(); i++)
		cubes[i].release();
	
	for (int i = 0; i < planes.size(); i++)
		planes[i].release();
	

	ParticleSystem::destroy();
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

	m_rot_x += (mouse_pos.y - (height / 2.0f)) * EngineTime::getDeltaTime() * 0.2f;
	m_rot_y += (mouse_pos.x - (width / 2.0f)) * EngineTime::getDeltaTime() * 0.2f;


	InputSystem::get()->setCursorPositon(Point(width / 2.0f, height / 2.0f)); // Reset cursor position to center of the window
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

AppWindow::~AppWindow()
{

}