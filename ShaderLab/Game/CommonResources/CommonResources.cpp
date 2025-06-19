/*
	@file	CommonResources.cpp
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#include "pch.h"
#include "CommonResources.h"
#include <cassert>

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
CommonResources::CommonResources()
	:
	m_stepTimer{ nullptr },
	m_deviceResources{ nullptr },
	m_commonStates{ nullptr },
	m_debugString{ nullptr },
	m_inputManager{ nullptr }
{
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void CommonResources::Initialize(
	DX::StepTimer* timer,
	DX::DeviceResources* dr,
	DirectX::CommonStates* commonStates,
	mylib::DebugString* debugString,
	mylib::InputManager* inputManager
)
{
	assert(timer);
	assert(dr);
	assert(commonStates);
	assert(debugString);
	assert(inputManager);

	m_stepTimer = timer;
	m_deviceResources = dr;
	m_commonStates = commonStates;
	m_debugString = debugString;
	m_inputManager = inputManager;
}
