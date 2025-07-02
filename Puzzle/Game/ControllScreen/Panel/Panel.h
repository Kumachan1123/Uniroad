/*
*	@file Panel.h
*	@brief パネルクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
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
#include "Game/ControllScreen/Canvas/Canvas.h"
#include "Game/Interface/IUI/IUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/TileDatas/TileDatas.h"
#include "Game/Mouse/Mouse.h"

// 前方宣言
class CommonResources;

// パネルクラス
class Panel : public IUI
{
public:
	// アクセサ
	// UIにヒットしたかどうか取得
	bool GetIsHit()const { return m_hit; }
	// UIにヒットしたかどうか設定
	void SetIsHit(bool hit) { m_hit = hit; }
	// CSVマップを設定
	void SetCSVMap(CSVMap* pCSVMap) { m_pCSVMap = pCSVMap; }
	// マウスポインターを設定
	void SetMouse(MyMouse* pMouse) { m_pMouse = pMouse; }
	// ビューポートを設定
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewPortControll = viewport; }
public:
	// public関数
	// コンストラクタ
	Panel(int mapSizeX, int mapSizeY);
	// デストラクタ
	~Panel();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height) override;
	// 更新
	void Update(const float elapsedTime) override;
	// 描画
	void Render() override;
	// UI追加
	void Add(
		const std::string& key,						// テクスチャのキー
		const DirectX::SimpleMath::Vector2& position,	// 位置
		const DirectX::SimpleMath::Vector2& scale,	// スケール
		KumachiLib::ANCHOR anchor,					// アンカー
		UIType type) override;						// UIの種類

private:
	// private関数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能UI
	std::vector<std::unique_ptr<Canvas>> m_pUI;
	// 選択中であることを表示するUI
	std::unique_ptr<Canvas> m_pSelectedUI;
	// マウスのポインター
	MyMouse* m_pMouse;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 時間
	float m_time;
	// ヒットフラグ
	bool m_hit;
	// ヒットフラグが立ったUIのインデックス
	int m_menuIndex;
	// マップサイズ
	int m_mapSizeX, m_mapSizeY;
	// CSVマップ
	CSVMap* m_pCSVMap;
	// 操作用ビューポート
	D3D11_VIEWPORT m_viewPortControll;
};