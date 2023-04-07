#pragma once

/*******************************************************************************************************************
	MenuState.h, MenuState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Initializes a menu state within the game.

*******************************************************************************************************************/
#include "GameState.h"
#include "InterfaceShader.h"
#include "Background.h"
#include "Button.h"

class MenuState : public GameState {

public:
	MenuState(GameState* previousState);
	virtual ~MenuState();

public:
	virtual bool Update()	override;
	virtual bool Render()	override;
	
private:
	bool Initialize();
	void LoadShaders();
	void LoadInterface();
	void ProcessInput();

private:
	Shader*					m_shader;
	Background*				m_background;
	std::vector<Button*>	m_buttons;

private:
	static const unsigned int s_maxButtons;
};