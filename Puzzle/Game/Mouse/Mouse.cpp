/*
*	@file Mouse.cpp
*	@brief マウスクラス
*/
#include <pch.h>
#include "Mouse.h"
/*
*	@brief コンストラクタ
*	@details マウスクラスのコンストラクタ
*	@param なし
*	@return なし
*/
MyMouse::MyMouse()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_position(0.0f, 0.0f) // マウスの位置
	, m_viewPortControll() // ビューポートの制御
	, m_renderWidth(0.0f) // ウィンドウの幅（ピクセル単位）
	, m_renderHeight(0.0f) // ウィンドウの高さ（ピクセル単位）
	, m_vp_left_UI(0.0f) // ビューポート左上X（論理解像度基準）
	, m_vp_top_UI(0.0f) // ビューポート左上Y（論理解像度基準）
	, m_vp_width_UI(0.0f) // ビューポート幅（論理解像度基準）
	, m_vp_height_UI(0.0f) // ビューポート高さ（論理解像度基準）
	, m_isMouseDrag(false) // マウスドラッグフラグ
	, m_hit(false) // ヒットフラグ
	, m_hitNewTile(false) // ヒットフラグ(新しく出てきたタイル)
	, m_hitNewTileIndex(-1) // 当たっている新しく出てきたタイルの番号(-1は当たっていないことを示す)
	, m_hitPanelIndex(-1) // 当たっているパネルの番号(-1は当たっていないことを示す)
	, m_prevLeftButton(false) // 前フレームの左ボタン状態
	, m_leftReleased(false) // 左ボタンが離されたかどうか
{
}
/*
*	@brief デストラクタ
*	@details マウスクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
MyMouse::~MyMouse()
{
	// 何もしない
}

/*
*	@brief 初期化
*	@details マウスクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void MyMouse::Initialize(CommonResources* resources)
{
	// 共通リソースへのポインタを保存
	m_pCommonResources = resources;
}
/*
*	@brief 更新
*	@details マウスクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void MyMouse::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	// ウィンドウの幅（ピクセル単位）
	m_renderWidth = static_cast<float>(rect.right);
	// ウィンドウの高さ（ピクセル単位）
	m_renderHeight = static_cast<float>(rect.bottom);
	// ビューポート設定 
	D3D11_VIEWPORT viewportRight = {};
	// ビューポートの左上X座標（画面幅の70%位置）
	viewportRight.TopLeftX = m_renderWidth * 0.7f;
	// ビューポートの左上Y座標（最上部）
	viewportRight.TopLeftY = 0;
	// ビューポートの幅（画面幅の30%）
	viewportRight.Width = m_renderWidth * 0.3f;
	// ビューポートの高さ（画面高さ全体）
	viewportRight.Height = m_renderHeight;
	// 最小深度
	viewportRight.MinDepth = 0.0f;
	// 最大深度
	viewportRight.MaxDepth = 1.0f;
	// ビューポート情報をメンバ変数に保存
	m_viewPortControll = viewportRight;
	// ビューポート左上X
	float vp_left = m_viewPortControll.TopLeftX;
	// ビューポート左上Y
	float vp_top = m_viewPortControll.TopLeftY;
	// ビューポート幅
	float vp_width = m_viewPortControll.Width;
	// ビューポート高さ
	float vp_height = m_viewPortControll.Height;

	// UIの論理解像度 
	// 論理解像度の幅
	constexpr float logicalWidth = 1920.0f;
	// 論理解像度の高さ
	constexpr float logicalHeight = 1080.0f;
	// マウス座標を論理解像度基準にスケーリング
	// 論理解像度基準のマウスX座標
	float mouseX_UI = mouseState.x * (logicalWidth / m_renderWidth);
	// 論理解像度基準のマウスY座標
	float mouseY_UI = mouseState.y * (logicalHeight / m_renderHeight);
	// ビューポートの論理解像度基準での座標・サイズを計算
	// 論理解像度基準のビューポート左上X
	m_vp_left_UI = vp_left * (logicalWidth / m_renderWidth);
	// 論理解像度基準のビューポート左上Y
	m_vp_top_UI = vp_top * (logicalHeight / m_renderHeight);
	// 論理解像度基準のビューポート幅
	m_vp_width_UI = vp_width * (logicalWidth / m_renderWidth);
	// 論理解像度基準のビューポート高さ
	m_vp_height_UI = vp_height * (logicalHeight / m_renderHeight);
	// マウス座標をビューポート内ローカル座標に変換
	m_position = Vector2(mouseX_UI - m_vp_left_UI, mouseY_UI - m_vp_top_UI);

	// 離されたか判定
	m_leftReleased = (!mouseState.leftButton) && m_prevLeftButton;
	// 最後に、前フレームの状態を更新しておく
	m_prevLeftButton = mouseState.leftButton;
	// ---デバッグ表示---
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("isInside: %s",
		(mouseX_UI >= m_vp_left_UI) && (mouseX_UI < m_vp_left_UI + m_vp_width_UI)
		&& (mouseY_UI >= m_vp_top_UI) && (mouseY_UI < m_vp_top_UI + m_vp_height_UI)
		? "true" : "false"); // マウスがビューポート内にあるか
	debugString->AddString("Inside ViewPort Mouse Position: (%f, %f)", m_position.x, m_position.y); // ビューポート内マウス座標
	debugString->AddString("PanelPosition:%f,%f", GetPanelPosition().x, GetPanelPosition().y);// 当たっているパネルの位置
	debugString->AddString("HitNextTilePosition: %f, %f", GetNewTilePosition().x, GetNewTilePosition().y);// 当たっている新しいタイルの位置
	debugString->AddString("DragFlag:%s", m_isMouseDrag ? "true" : "false");// ドラッグフラグの状態
	debugString->AddString("hitPanel:%i", GetHitPanelIndex());// 当たっているパネルのインデックス
	debugString->AddString("hitNextTile:%i", GetHitNewTileIndex());// 当たっている新しく出てきたタイルのインデックス
	debugString->AddString("LeftReleased: %s", m_leftReleased ? "true" : "false");// 左ボタンが離されたかどうか
}
