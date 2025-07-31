/*
*	@file	NextTiles.h
*	@brief	次のタイルクラス
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
#include "Game/Mouse/Mouse.h"
#include "Game/MainScreen/Tiles/TileDatas/TileDatas.h"
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/ControllScreen/TileConnectionTable/TileConnectionTable.h"

// 前方宣言
class CommonResources;

// 次のタイルクラス
class NextTiles : public IUI
{
private:
	// タイル情報を保持する構造体
	struct TileInfo
	{
		std::string textureKey; // テクスチャのキー
		std::unique_ptr<Canvas> canvas; // キャンバスオブジェクト
	};

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
	// ミニキャラの進行方向(速度ベクトル)を設定
	void SetMiniCharacterVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_miniCharacterVelocity = velocity; }
	// ミニキャラが今いるタイルの名前を設定
	void SetMiniCharacterTileName(const std::string& tileName) { m_miniCharacterTileName = tileName; }
public:
	// public関数
	// コンストラクタ
	NextTiles();
	// デストラクタ
	~NextTiles();
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
	// 設置済みタイルのみ描画
	void DrawPlacedTiles() const;
private:
	// private関数
	// 更新中に定期的にUIを追加する
	void AddNextTiles();
	// 新しいパネルを配置
	void AddToPanel();
	// 元の位置にタイルを戻す
	void ResetTilePosition();
	// 今いるタイルと進行方向から次に進めるタイルを取得
	std::vector<std::string> GetAvailableNextTiles(const std::string& currentTileName, const DirectX::SimpleMath::Vector3& velocity) const;
	// タイルの接続可能なタイルを取得
	Direction GetDirectionFromVelocity(const DirectX::SimpleMath::Vector3& velocity) const;
	// 接続可能なタイルを抽選
	std::string GetRandomConnectableTile(const std::vector<std::string>& availableTiles, const std::string& previousTileName) const;
private:
	// private関数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 設置可能タイル
	std::vector<TileInfo> m_pTile;
	// 背景
	std::vector<TileInfo> m_pBack;
	// 設置済みタイル
	std::vector<TileInfo> m_pPlacedTile;
	// CSVマップ
	CSVMap* m_pCSVMap;
	// マウスのポインター
	MyMouse* m_pMouse;
	// 各選択可能UIの初期位置リスト
	std::vector<DirectX::SimpleMath::Vector2> m_initialPositions;
	// ドラッグ中のUIインデックス
	int m_draggingIndex;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 時間
	float m_time;
	// ヒットフラグ
	bool m_hit;
	// ヒットフラグが立ったUIのインデックス
	int m_menuIndex;
	// 操作用ビューポート
	D3D11_VIEWPORT m_viewPortControll;
	// 使うタイルの辞書
	std::vector<std::string> m_tilesDictionary;
	// ミニキャラの速度ベクトル
	DirectX::SimpleMath::Vector3 m_miniCharacterVelocity;
	// ミニキャラが今いるタイルの名前
	std::string m_miniCharacterTileName;
	// 最後に置いたタイルの名前
	std::string m_lastPlacedTileName;
	// 前回生成したタイル名
	std::string m_previousTileName;
};