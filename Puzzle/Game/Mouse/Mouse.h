/*
*	@file Mouse.h
*	@brief マウスクラス
*/
#pragma once
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

// 前方宣言
class CommonResources;

// マウスクラス
class MyMouse
{
public:
	// アクセサ
	// マウスの位置を取得
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// マウスの位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	// 論理解像度基準のビューポート左上Xを取得
	float GetVpLeftUI() const { return m_vp_left_UI; }
	// 論理解像度基準のビューポート左上Yを取得
	float GetVpTopUI() const { return m_vp_top_UI; }
	// 論理解像度基準のビューポート幅を取得
	float GetVpWidthUI() const { return m_vp_width_UI; }
	// 論理解像度基準のビューポート高さを取得
	float GetVpHeightUI() const { return m_vp_height_UI; }
	// 
	// ビューポートを設定
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }

public:
	// public関数
	// コンストラクタ
	MyMouse();
	// デストラクタ
	~MyMouse();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(const float elapsedTime);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// マウスの位置
	DirectX::SimpleMath::Vector2 m_position;
	// 操作用ビューポート
	D3D11_VIEWPORT m_viewPortControll;
	// ウィンドウの幅
	float m_renderWidth;
	// ウィンドウの高さ
	float m_renderHeight;
	// 論理解像度基準のビューポート左上X
	float m_vp_left_UI;
	// 論理解像度基準のビューポート左上Y
	float m_vp_top_UI;
	// 論理解像度基準のビューポート幅
	float m_vp_width_UI;
	// 論理解像度基準のビューポート高さ
	float m_vp_height_UI;
};