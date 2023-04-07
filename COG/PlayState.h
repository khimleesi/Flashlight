#pragma once

/*******************************************************************************************************************
	PlayState.h, PlayState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Initializes a play state within the game.

*******************************************************************************************************************/
#include <list>
#include <forward_list>
#include <deque>
#include "GameState.h"
#include "EntityShader.h"
#include "TerrainShader.h"
#include "SkyboxShader.h"
#include "InterfaceShader.h"
#include "TextShader.h"
#include "GameComponent.h"
#include "Terrain.h"
#include "Entity.h"
#include "Camera.h"
#include "Skybox.h"
#include "Player.h"
#include "Picker.h"
#include "Text.h"
#include "MinimapWidget.h"
#include "Light.h"
#include "Frustum.h"
#include "Button.h"

class PlayState : public GameState {

private:
	enum ShaderType	{ SHADER_SKYBOX, SHADER_TERRAIN, SHADER_ENTITY, SHADER_INTERFACE, SHADER_TEXT };

public:
	PlayState(GameState* previousState);
	virtual ~PlayState();

public:
	virtual bool Update()	override;
	virtual bool Render()	override;
	
private:
	void Initialize();
	void LoadShaders();
	void LoadObjects();
	void LoadComponents();
	void LoadInterface();
	void LoadLights();

private:
	void ProcessInput();
	void UpdateLights();
	void UpdateObjects();
	void UpdateComponents();
	void UpdateInterface();

private:
	void RenderWorld();
	void RenderInterface();

private:
	void IssueFinalEvent();

private:
	Skybox*			m_skybox;
	Terrain*		m_terrain;
	Player*			m_player;

private:
	Camera*			m_mainCamera;
	Camera*			m_minimapCamera;
	Picker*			m_picker;
	Frustum*		m_frustum;

private:
	Text*			m_text;
	MinimapWidget*	m_minimapWidget;
	Button*			m_menuButton;
	Button*			m_helpButton;

private:
	unsigned int	m_lightCount;
	bool			m_finalEventIssued;
	bool			m_debugMode;
	bool			m_wireFrameMode;
	bool			m_finishedEvents;

private:
	std::vector<Shader*>			m_shaders;
	std::vector<Entity*>			m_entities;
	std::deque<Entity*>				m_collectables;
	std::vector<Light*>				m_lights;
	std::vector<GameComponent*>		m_components;

private:
	static const unsigned int s_maxEntities;
	static const unsigned int s_maxShaders;
	static const unsigned int s_maxCollectables;
	static const unsigned int s_maxComponents;

private:
	static const float s_maxLightRadius;
	static const float s_maxCollectableRange;
	static const float s_linearPulseAmount;
	static const float s_defaultCameraZoom;
};