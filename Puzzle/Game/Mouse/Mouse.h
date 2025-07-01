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
	// マウスがあるパネルの座標を取得
	DirectX::SimpleMath::Vector2 GetPanelPosition() const { return m_panelPosition; }
	// マウスがあるパネルの座標を設定
	void SetPanelPosition(const DirectX::SimpleMath::Vector2& position) { m_panelPosition = position; }
	// 新しいタイルの元の座標を取得
	DirectX::SimpleMath::Vector2 GetNewTilePosition() const { return m_newTilePosition; }
	// 新しいタイルの元の座標を設定
	void SetNewTilePosition(const DirectX::SimpleMath::Vector2& position) { m_newTilePosition = position; }
	// 論理解像度基準のビューポート左上Xを取得
	float GetVpLeftUI() const { return m_vp_left_UI; }
	// 論理解像度基準のビューポート左上Yを取得
	float GetVpTopUI() const { return m_vp_top_UI; }
	// 論理解像度基準のビューポート幅を取得
	float GetVpWidthUI() const { return m_vp_width_UI; }
	// 論理解像度基準のビューポート高さを取得
	float GetVpHeightUI() const { return m_vp_height_UI; }
	// ビューポートを設定
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
	// マウスドラッグフラグを取得
	bool IsMouseDrag() const { return m_isMouseDrag; }
	// マウスドラッグフラグを設定
	void SetMouseDrag(bool isMouseDrag) { m_isMouseDrag = isMouseDrag; }
	// 当たっているパネルの番号を取得
	int GetHitPanelIndex() const { return m_hitPanelIndex; }
	// 当たっているパネルの番号を設定
	void SetHitPanelIndex(int index) { m_hitPanelIndex = index; }
	// 当たっている新しく出てきたタイルの番号を取得
	int GetHitNewTileIndex() const { return m_hitNewTileIndex; }
	// 当たっている新しく出てきたタイルの番号を設定
	void SetHitNewTileIndex(int index) { m_hitNewTileIndex = index; }
	// ヒットフラグを取得
	bool IsHit() const { return m_hit; }
	// ヒットフラグを設定
	void SetHit(bool hit) { m_hit = hit; }
	// ヒットフラグ(新しく出てきたタイル)を取得
	bool IsHitNewTile() const { return m_hitNewTile; }
	// ヒットフラグ(新しく出てきたタイル)を設定
	void SetHitNewTile(bool hitNewTile) { m_hitNewTile = hitNewTile; }


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
	// マウスがあるパネルの座標
	DirectX::SimpleMath::Vector2 m_panelPosition;
	// 新しいタイルの元の座標（操作直前の位置）
	DirectX::SimpleMath::Vector2 m_newTilePosition;
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
	// マウスドラッグフラグ
	bool m_isMouseDrag;
	// 当たっているパネルの番号
	int m_hitPanelIndex;
	// 当たっている新しく出てきたタイルの番号
	int m_hitNewTileIndex;
	// ヒットフラグ(パネル)
	bool m_hit;
	// ヒットフラグ(新しく出てきたタイル)
	bool m_hitNewTile;
};