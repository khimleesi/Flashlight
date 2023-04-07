#include "GuideState.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
GuideState::GuideState(GameState* previousState)
	:	GameState(previousState),
		m_shader(nullptr),
		m_background(nullptr),
		m_return(nullptr)
{
	Initialize();
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
GuideState::~GuideState() {

	if (m_return)		{ delete m_return; m_return = nullptr; }
	if (m_background)	{ delete m_background; m_background = nullptr; }
	if (m_shader)		{ delete m_shader; m_shader = nullptr; }
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to this state
*******************************************************************************************************************/
bool GuideState::Initialize() {

	IsActive() = IsAlive() = true;

	LoadShaders();
	LoadInterface();

	return true;
}


/*******************************************************************************************************************
	A function that creates and initializes all shaders within this game state
*******************************************************************************************************************/
void GuideState::LoadShaders()
{
	m_shader = new InterfaceShader("interfaceVertexShader.vert", "interfaceFragmentShader.frag");
}


/*******************************************************************************************************************
	A function that creates and initializes all interface objects within this game state
*******************************************************************************************************************/
void GuideState::LoadInterface()
{
	m_background	= Background::Create("GuideBackground");
	m_return		= Button::Create("ReturnButton");
}


/*******************************************************************************************************************
	A function that processes all input within this game state
*******************************************************************************************************************/
void GuideState::ProcessInput()
{
	if (m_return->IsClicked()) {
		m_previousState->IsActive() = true;
		IsActive() = IsAlive() = false;
	}
	m_return->Update();
}


/*******************************************************************************************************************
	Function that updates everything within this state
*******************************************************************************************************************/
bool GuideState::Update() {
	
	m_background->Update();

	ProcessInput();

	return true;
}


/*******************************************************************************************************************
	Function that renders all this states graphics to the screen
*******************************************************************************************************************/
bool GuideState::Render() {
	
	Screen::Instance()->BeginScene(0.0f, 0.0f, 0.0f);
	Screen::Instance()->PerspectiveView(false);
	Screen::Instance()->EnableBlending(true);
	Screen::Instance()->EnableDepth(false);
	Screen::Instance()->CullBackFace(false);

	m_shader->Bind();
		m_background->Render(m_shader);
		m_return->Render(m_shader);
	m_shader->Unbind();

	Screen::Instance()->EndScene();

	return true;
}