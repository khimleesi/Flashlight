#include "InputManager.h"
#include "ScreenManager.h"
#include "FileManager.h"
#include "Log.h"
#include "Tools.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
InputManager::InputManager()	
	:	m_events({0}),
		m_windowClosed(false),
		m_keyStates(nullptr),
		m_cursor(nullptr),
		m_mousePosition(0.0f),
		m_mouseMotion(0.0f),
		m_mouseWheel(0.0f),
		m_controller(nullptr),
		m_controllerID(0),
		m_controllerLeftAxisMotion(0.0f),
		m_controllerRightAxisMotion(0.0f),
		m_controllerDirectionalPad(0.0f)
{
	FL_LOG("[INPUT MANAGER CONSTRUCT]", FL_LOG_EMPTY, LOG_BREAK);
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
void InputManager::ShutDown()
{
	if (m_controller) {
		FL_LOG("[INPUT] Shutting down controller: ", SDL_JoystickName(m_controller), LOG_MESSAGE);
		if (SDL_JoystickGetAttached(m_controller)) { SDL_JoystickClose(m_controller); m_controller = nullptr; }
		FL_LOG("[INPUT] Total number of controllers after shutdown: ", SDL_JoystickGetAttached(m_controller), LOG_MESSAGE);
	}

	FL_LOG("[INPUT MANAGER SHUT DOWN]", FL_LOG_EMPTY, LOG_BREAK);
}


/*******************************************************************************************************************
	Function that initializes all necessary startup procedures and resets default values
*******************************************************************************************************************/
void InputManager::Initialize()
{
	//--- Load mapped buttons from file. Implemented a 'tag' to allow us to distinguish what buttons we want to load
	//--- (might not always want to load PS4 buttons, for example, if it's an XBox game)
	LoadMappedButtons("Assets/Files/mappedButtons.config", "mouse");
	LoadMappedButtons("Assets/Files/mappedButtons.config", "controller");

	//--- Outputs all the mapped buttons to debug window - for testing only
	PrintObjectData();

	//--- Initializes all the button states to their default state upon start-up (false)
	InitializeMouseButtons();
	InitializeControllerButtons();

	//--- Setup and load a PS4 controller
	LoadController();
}


/*******************************************************************************************************************
	Update function that processes all keyboard, mouse and controller events
*******************************************************************************************************************/
void InputManager::Update()
{
	if (!SDL_PollEvent(&m_events))	{ SDL_Delay(1); }
	else							{ SDL_PushEvent(&m_events); }

	//--- Reset mouse position so input is processed from scratch
	ResetMouse();

	//--- Reset controller button states to false
	InitializeControllerButtons();

	m_keyStates = (Uint8*)SDL_GetKeyboardState(nullptr);
	
	while (SDL_PollEvent(&m_events)) {

		switch (m_events.type) {

			case SDL_QUIT:										{ m_windowClosed = true; break; }
			case SDL_MOUSEMOTION:								{ GenerateMouseMotionEvent(); break; }
			case SDL_MOUSEWHEEL:								{ GenerateMouseWheelEvent(); break; }
			case SDL_MOUSEBUTTONUP: case SDL_MOUSEBUTTONDOWN:	{ GenerateMouseButtonEvent(); break; }
			case SDL_JOYBUTTONUP:	case SDL_JOYBUTTONDOWN:		{ GenerateControllerButtonEvent(); break; }
			case SDL_JOYHATMOTION:								{ GenerateControllerDirectionalPadEvent(); break; }
			case SDL_JOYAXISMOTION:								{ GenerateControllerAxisEvent(); break; }
		}
	}
}


/*******************************************************************************************************************
	Read in button values from a text file and store them in to a map
*******************************************************************************************************************/
bool InputManager::LoadMappedButtons(const std::string& fileLocation, const std::string& objectName) {

	using namespace file_constants;

	if (!File::Instance()->OpenForReading(fileLocation)) { return false; }

	bool objectFound = false;

	while (File::Instance()->ExtractFileData()) {

		if (File::Instance()->FileDataContains(TYPE_BEGIN)) {

			std::string fileObjectName = File::Instance()->CutFileData(TYPE_BEGIN_OFFSET);
	
			if (objectName == fileObjectName) { objectFound = true;  }
		}

		else if (File::Instance()->FileDataContains(TYPE_END) && objectFound) { 
			
			File::Instance()->Close(fileLocation); 
		}

		else if (objectFound) {

			if (File::Instance()->FileDataContains(LINE_BREAK)) { continue; }
			
			int equalPosition		= File::Instance()->Find(DIVIDER);
			std::string dataKey		= File::Instance()->CutFileData(0, equalPosition);
			std::string dataValue	= File::Instance()->CutFileData(equalPosition + DIVIDER_OFFSET);

			m_mappedButtons[dataKey] = (Uint8)StringToInteger(dataValue);
		}

		else if (File::Instance()->FileDataContains(END_OF_FILE) && !objectFound) {
			
			FL_LOG("[FILE] Object not found in file: ", objectName.c_str(), LOG_ERROR);
			
			File::Instance()->Close(fileLocation);
			
			return false;
		}
	}

	return true;
}


/*******************************************************************************************************************
	Function that prints all data stored in a map (for debugging purposes)
*******************************************************************************************************************/
void InputManager::PrintObjectData() {

	if (!m_mappedButtons.empty()) {
		
		FL_LOG("The following object data was stored in to mapped buttons container: ", FL_LOG_EMPTY, LOG_MESSAGE);
			
		for (auto const& object : m_mappedButtons) {
				FL_LOG("[INPUT] Mapped Button: " + object.first + ":", (int)object.second, LOG_MESSAGE);
			}
	}
	else { FL_LOG("[INPUT] Mapped buttons container is empty", FL_LOG_EMPTY, LOG_WARN); }
}


/*******************************************************************************************************************
	Function that resets the mouse position and motion
*******************************************************************************************************************/
void InputManager::ResetMouse()
{
	m_mouseMotion.x = 0.0f;
	m_mouseMotion.y = 0.0f;
	m_mouseWheel.x	= 0.0f;
	m_mouseWheel.y	= 0.0f;
}


/*******************************************************************************************************************
	Function that initializes all the mouse buttons to up
*******************************************************************************************************************/
void InputManager::InitializeMouseButtons()
{
	for (auto& button : m_mouseButtonState) {
		button = m_mappedButtons["button.up"];
	}
}


/*******************************************************************************************************************
	Function that updates our mouse position and motion to that of the SDL_Event
*******************************************************************************************************************/
void InputManager::GenerateMouseMotionEvent()
{
	m_mousePosition.x	= (float)m_events.motion.x;
	m_mousePosition.y	= (float)m_events.motion.y;
	m_mouseMotion.x		= (float)m_events.motion.xrel;
	m_mouseMotion.y		= (float)m_events.motion.yrel;
}


/*******************************************************************************************************************
	Function that checks if a mouse button has been pressed
*******************************************************************************************************************/
void InputManager::GenerateMouseButtonEvent()
{
	m_mousePosition.x = (float)m_events.motion.x;
	m_mousePosition.y = (float)m_events.motion.y;

	m_mouseButtonState[m_events.button.button] =
		((m_events.button.state == SDL_PRESSED) ? m_mappedButtons["button.down"] : m_mappedButtons["button.up"]);
}


/*******************************************************************************************************************
	Function that updates our mouse wheel position to that of the SDL_Event
*******************************************************************************************************************/
void InputManager::GenerateMouseWheelEvent()
{
	m_mouseWheel.x = (float)m_events.wheel.x;
	m_mouseWheel.y = (float)m_events.wheel.y;
}


/*******************************************************************************************************************
	Function that loops through all controllers connected to the users system and initializes supported controller
*******************************************************************************************************************/
void InputManager::LoadController()
{
	//--- Loop through all the controllers the user has connected to their system
	for (int i = 0; i < SDL_NumJoysticks(); i++) {

		//--- Open all of the controllers
		m_controller = SDL_JoystickOpen(i);

		//--- If we successfully open a controller, we check if it's supported in this game
		if (m_controller) {

			char GUIDbuffer[input_constants::BUFFER_SIZE];

			//--- We get the GUID (Globally Unique Identifier) of the controller connected
			SDL_JoystickGUID controllerGUID = SDL_JoystickGetGUID(m_controller);

			//--- Then we return a string representation of this GUID and store it in our GUIDbuffer
			//--- with a size of 33 bytes (a char is 1 byte, and we have 32 chars in a GUID, plus a null terminator)
			SDL_JoystickGetGUIDString(controllerGUID, GUIDbuffer, sizeof(GUIDbuffer));

			//--- We use strstr to see if our user-defined supported GUID matches the controller GUID
			const char* findGUID = nullptr;

			if ((findGUID = std::strstr(GUIDbuffer, input_constants::SUPPORTED_GUID.c_str())) == NULL) {
				FL_LOG("[INPUT] Unsupported GUID, or GUID reading error: ", SDL_JoystickName(m_controller), LOG_WARN);
			}
			
			//--- If controller GUID matches the user-defined GUID, we set the controller ID to the supported controller only
			if (findGUID) {

				SDL_JoystickEventState(SDL_ENABLE);

				m_controllerID = SDL_JoystickInstanceID(m_controller);
				FL_LOG("[INPUT] Controller detected: (PS4) ", SDL_JoystickName(m_controller), LOG_MESSAGE);
				FL_LOG("[INPUT] Successfully assigned to controller ID: ", m_controllerID, LOG_SUCCESS);
				break;
			}

			//--- If GUID of the controller connected is not supported, we warn the user they don't have a supported controller connected
			else {
				FL_LOG("[INPUT] Controller detected: ", SDL_JoystickName(m_controller), LOG_WARN);
				FL_LOG("[INPUT] This controller is not supported. Please use a PS4 controller, or use PC controls", FL_LOG_EMPTY, LOG_WARN);
			}
		}

		//--- If for any reason we can't open controller(s), we warn the user
		else { FL_LOG("[INPUT] Couldn't open controller. Please use PC for player controls", FL_LOG_EMPTY, LOG_WARN); }
	}
	
	FL_LOG("[INPUT] Total number of controllers found on this machine: ", SDL_NumJoysticks(), LOG_MESSAGE);

	//--- If user doesn't have any controllers connected we output a warning message
	if (m_controller == nullptr) { FL_LOG("[INPUT] No controller detected. Please use PC for player controls", FL_LOG_EMPTY, LOG_WARN); }

	//--- NOTE
	// If we wanted to allow multiple controllers (for a multi-player game), we would have a vector of
	// controller ID's, and we would read them all in at once, without breaking out of the if statement above.
	// Upon checking for key input, we would loop through each controller ID in the vector.
	// ---

}


/*******************************************************************************************************************
	Function that initializes all the controller buttons to up
*******************************************************************************************************************/
void InputManager::InitializeControllerButtons()
{
	for (auto& button : m_controllerButtonState) {
		button = m_mappedButtons["button.up"];
	}
}


/*******************************************************************************************************************
	Function that checks for controller axis motion and moves the axis by the passed in speed value amount
*******************************************************************************************************************/
void InputManager::CheckAxisMotion(const std::string& button, float& axis, float speed)
{
	if (m_events.jaxis.axis == m_mappedButtons[button]) {

		if (m_events.jaxis.value < -input_constants::DEAD_ZONE)		{ axis = -speed; }
		else if (m_events.jaxis.value > input_constants::DEAD_ZONE)	{ axis =  speed; }
		else														{ axis =  0.0f;  }
	}
}


/*******************************************************************************************************************
	Function that checks if a controller button has been pressed
*******************************************************************************************************************/
void InputManager::GenerateControllerButtonEvent()
{
	if (m_events.jbutton.which == m_controllerID) {

		m_controllerButtonState[m_events.jbutton.button] =
			((m_events.jbutton.state == SDL_PRESSED) ? m_mappedButtons["button.down"] : m_mappedButtons["button.up"]);
	}
}


/*******************************************************************************************************************
	Function that checks if a controller directional pad button has been pressed
*******************************************************************************************************************/
void InputManager::GenerateControllerDirectionalPadEvent()
{
	//--- To enable diagonal movement, place this in SDL_HAT_CENTERED switch case instead.
	//--- Diagonal movement currently disabled
	m_controllerDirectionalPad.x = 0.0f;
	m_controllerDirectionalPad.y = 0.0f; 

	if (m_events.jaxis.which == m_controllerID) {

		switch (m_events.jhat.value) {
			case SDL_HAT_DOWN:	m_controllerDirectionalPad.y =  input_constants::SPEED; break;
			case SDL_HAT_UP:	m_controllerDirectionalPad.y = -input_constants::SPEED; break;
			case SDL_HAT_RIGHT: m_controllerDirectionalPad.x =  input_constants::SPEED; break;
			case SDL_HAT_LEFT:	m_controllerDirectionalPad.x = -input_constants::SPEED; break;
		}
	}
}


/*******************************************************************************************************************
	Function that checks if a controller axis has been moved
*******************************************************************************************************************/
void InputManager::GenerateControllerAxisEvent()
{
	if (m_events.jaxis.which == m_controllerID) {

		CheckAxisMotion("axis.lmotion.x", m_controllerLeftAxisMotion.x, input_constants::SPEED);
		CheckAxisMotion("axis.lmotion.y", m_controllerLeftAxisMotion.y, input_constants::SPEED);
		CheckAxisMotion("axis.rmotion.x", m_controllerRightAxisMotion.x, input_constants::ROTATE_SPEED);
		CheckAxisMotion("axis.rmotion.y", m_controllerRightAxisMotion.y, input_constants::ROTATE_SPEED);
	}
}


/*******************************************************************************************************************
	Function that checks if a key on the keyboard has been pressed
*******************************************************************************************************************/
bool InputManager::IsKeyPressed(Uint8 key, bool repeat) {

	//--- Check our key states for this key press
	//--- and only register a single key press if the repeat bool is false
	if (m_keyStates) {
			if (m_keyStates[key]) {
		
				//--- If we want key reads to be repeated we pass in true to the repeat bool
				if (!repeat) { m_keyStates[key] = SDL_RELEASED; }

				return true;
			}
	}
	return false;
}


/*******************************************************************************************************************
	Function that checks if a mouse button has been pressed
*******************************************************************************************************************/
bool InputManager::IsMouseButtonPressed(Uint8 button, bool repeat) {

	//--- Check our mouse button states for this button press
	//--- and only register a single button press if the repeat bool is false
	if (m_mouseButtonState[button]) {
	
		//--- If we want button reads to be repeated we pass in true to the repeat bool
		if (!repeat) { m_mouseButtonState[button] = m_mappedButtons["button.up"]; }

		return true;
	}
	return false;
}


/*******************************************************************************************************************
	Function that checks if a controller button has been pressed
*******************************************************************************************************************/
bool InputManager::IsControllerButtonPressed(const std::string& button, bool repeat)
{
	auto mappedButton = m_mappedButtons.find(button);

	if (mappedButton == m_mappedButtons.end()) {
		FL_LOG("[INPUT] Mapped button not found: ", button.c_str(), LOG_ERROR);
		return false;
	}

	//--- Check our controller button states for this button press
	//--- and only register a single button press if the repeat bool is false
	if (m_controllerButtonState[m_mappedButtons[button]]) {

		//--- If we want button reads to be repeated we pass in true to the repeat bool
		if (!repeat) { m_controllerButtonState[m_mappedButtons[button]] = m_mappedButtons["button.up"]; }

		return true;
	}
	return false;
}


/*******************************************************************************************************************
	Function that checks if the mouse (in screen coordinates) is colliding with a 2D AABB
*******************************************************************************************************************/
bool InputManager::IsMouseColliding(const AABounds2D& other)
{
	//--- Inverted Y axis as the Y coordinate starts from the top left of the screen
	glm::vec2 position	= glm::vec2(m_mousePosition.x, Screen::Instance()->GetHeight() - m_mousePosition.y);
	glm::vec2 dimension = glm::vec2(1.0f, 1.0f);

	return AABounds2D(position, dimension).IsColliding(other);
}


/*******************************************************************************************************************
	Accessor Methods
*******************************************************************************************************************/
bool InputManager::IsWindowClosed() const						{ return m_windowClosed; }
const glm::vec2& InputManager::GetMousePosition() const			{ return m_mousePosition; }
const glm::vec2& InputManager::GetMouseMotion() const			{ return m_mouseMotion; }
const glm::vec2& InputManager::GetMouseWheel() const			{ return m_mouseWheel; }
const glm::vec2& InputManager::GetControllerLeftAxis() const	{ return m_controllerLeftAxisMotion; }
const glm::vec2& InputManager::GetControllerRightAxis() const	{ return m_controllerRightAxisMotion; }
const glm::vec2& InputManager::GetDirectionalPad() const		{ return m_controllerDirectionalPad; }


/*******************************************************************************************************************
	Modifier Methods
*******************************************************************************************************************/
void InputManager::SetWindowClosed(bool isClosed)				{ m_windowClosed = isClosed;}
void InputManager::SetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(Screen::Instance()->GetWindow(), x, y);
}

void InputManager::SetMouseCursorType(const std::string& cursorType)
{
	SDL_FreeCursor(m_cursor);

	m_cursor = SDL_CreateSystemCursor(SDL_SystemCursor(m_mappedButtons[cursorType]));

	SDL_SetCursor(m_cursor);
}

void InputManager::SetMouseCursorState(const std::string& cursorEnabled, const std::string& cursorVisible)
{
	if (m_mappedButtons[cursorEnabled] == 1) {

		if (m_mappedButtons[cursorVisible] == 1)	{ SDL_ShowCursor(1); SDL_SetRelativeMouseMode(SDL_FALSE); }
		else										{ SDL_ShowCursor(NULL); SDL_SetRelativeMouseMode(SDL_FALSE); }
	}

	else { SDL_ShowCursor(NULL); SDL_SetRelativeMouseMode(SDL_TRUE); }
}