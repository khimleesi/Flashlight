#include "MenuState.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
MenuState::MenuState(GameState* previousState)
	:	GameState(previousState),
		m_shader(nullptr),
		m_background(nullptr)
{
	Initialize();
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
MenuState::~MenuState() {
	
	RemoveFromScene(m_buttons);

	if (m_background)	{ delete m_background; m_background = nullptr; }
	if (m_shader)		{ delete m_shader; m_shader = nullptr; }
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the menu state
*******************************************************************************************************************/
bool MenuState::Initialize() {

	IsActive() = IsAlive() = true;
	
	Audio::Instance()->PlayAudio("Menu", 1.0f);

	LoadShaders();
	LoadInterface();

	return true;
}


/*******************************************************************************************************************
	A function that creates and initializes all shaders within this game state
*******************************************************************************************************************/
void MenuState::LoadShaders()
{
	m_shader = new InterfaceShader("interfaceVertexShader.vert", "interfaceFragmentShader.frag");
}


/*******************************************************************************************************************
	A function that creates and initializes all interface objects within this game state
*******************************************************************************************************************/
void MenuState::LoadInterface()
{
	m_background = Background::Create("MenuBackground");

	ReserveMemory(m_buttons, s_maxButtons);
	for (unsigned int i = 0; i < s_maxButtons; i++) {
		AddToScene(m_buttons, Button::Create("Button" + std::to_string(i)));
	}
}


/*******************************************************************************************************************
	A function that processes all input within this game state
*******************************************************************************************************************/
void MenuState::ProcessInput()
{
	for (auto button : m_buttons) {

		if (button->IsClicked("Play")) {
			Game::Instance()->GetStates()->MakePermanentState<PlayState>(this);
			Audio::Instance()->StopChannel("Menu");
			Audio::Instance()->PlayAudio("Play", 1.0f);
			IsActive() = IsAlive() = false;	
		}
		
		if (button->IsClicked("Story")) {
			Game::Instance()->GetStates()->MakeTemporaryState<StoryState>(this);
			IsActive() = false;
		}

		if (button->IsClicked("Guide")) {
			Game::Instance()->GetStates()->MakeTemporaryState<GuideState>(this);
			IsActive() = false;
		}

		if (button->IsClicked("Credits")) {
			Game::Instance()->GetStates()->MakeTemporaryState<CreditsState>(this);
			IsActive() = false;
		}

		if (button->IsClicked("Quit")) {
			Input::Instance()->SetWindowClosed(true);
			IsActive() = IsAlive() = false;
		}
		button->Update();
	}
}


/*******************************************************************************************************************
	Function that updates everything within the menu state
*******************************************************************************************************************/
bool MenuState::Update() {
	
	m_background->Update();
	
	ProcessInput();

	return true;
}


/*******************************************************************************************************************
	Function that renders all menu state graphics to the screen
*******************************************************************************************************************/
bool MenuState::Render() {
	
	Screen::Instance()->BeginScene(0.0f, 0.0f, 0.0f);
	Screen::Instance()->PerspectiveView(false);
	Screen::Instance()->EnableBlending(true);
	Screen::Instance()->EnableDepth(false);
	Screen::Instance()->CullBackFace(false);

	m_shader->Bind();
		m_background->Render(m_shader);
		for (auto button : m_buttons) { button->Render(m_shader); }
	m_shader->Unbind();

	Screen::Instance()->EndScene();

	return true;
}


/*******************************************************************************************************************
	Static variables and functions
*******************************************************************************************************************/
const unsigned int MenuState::s_maxButtons = 5;