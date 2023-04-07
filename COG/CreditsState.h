#pragma once

/*******************************************************************************************************************


*******************************************************************************************************************/
#include "GameState.h"
#include "InterfaceShader.h"
#include "Background.h"
#include "Button.h"

class CreditsState : public GameState {

public:
	CreditsState(GameState* previousState);
	virtual ~CreditsState();

public:
	virtual bool Update()	override;
	virtual bool Render()	override;

private:
	bool Initialize();
	void LoadShaders();
	void LoadInterface();
	void ProcessInput();

private:
	Shader*			m_shader;
	Background*		m_background;
	Button*			m_return;
};