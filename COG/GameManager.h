#pragma once

/*******************************************************************************************************************
	GameManager.h, GameManager.cpp
	Created by Kim Kane
	Last updated: 15/04/2018

	Singleton class that handles the setup, run and shutdown of the main game.

	[Features]
	Supports game states using a finite state machine.
	Performance trackers - support for FPS, frame time, update time, delta time and CPU usage.
	
	[Upcoming]
	Nothing at present.

	[Side Notes]
	Nothing at present.

*******************************************************************************************************************/
#include <string>

#include "Singleton.h"

#include "ScreenManager.h"
#include "InputManager.h"
#include "AudioManager.h"

#include "GameState.h"
#include "MenuState.h"
#include "StoryState.h"
#include "GuideState.h"
#include "CreditsState.h"
#include "EndState.h"
#include "BeginState.h"
#include "PlayState.h"

#include "FiniteStateMachine.h"
#include "Timer.h"
#include "Timestep.h"
#include "MainframeTracker.h"

class GameManager {

public:
	friend class Singleton<GameManager>;
	
private:
	typedef FiniteStateMachine<GameManager, GameState> GameStates;

public:
	void Initialize(const std::string& title, bool fullScreen, bool coreMode, bool vSync);
	void Shutdown();
	void Run();

public:
	GameStates*		GetStates();

public:
	unsigned int	GetFramesPerSecond() const;
	float			GetCurrentFrameTime() const;
	int				GetMainframePercentage();
	float			GetDeltaTime() const;

private:
	void LoadAudio();

private:
	Timestep BeginTime();

private:
	bool HasWindowClosed();
	void Update();
	void Render();
	void ConcludeTime();
	void CheckGameStateStatus();
	void StopSystemTrackers();

private:
	GameManager();
	GameManager(const GameManager&)				= delete;
	GameManager& operator=(const GameManager&)	= delete;

private:
	bool				m_endGame;
	GameStates			m_gameStates;
	Timer				m_gameTimer;
	Timestep			m_timestep;
	MainframeTracker	m_mainframeTracker;
	bool				m_hasLoaded;
};

typedef Singleton<GameManager> Game;