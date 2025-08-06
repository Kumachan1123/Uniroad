/*
*	@file ResultUI.h
*	@brief 結果UIクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "KumachiLib/Easing/Easing.h"
#include "Game/CommonResources/CommonResources.h"
#include "Game/ControllScreen/Canvas/Canvas.h"
#include "Game/Interface/IUI/IUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "KumachiLib/FileCounter/FileCounter.h"

// 前方宣言
class CommonResources;

// 結果UIクラス
class ResultUI : public IUI
{
public:
	// 列挙型
	// シーンID
	enum SceneID
	{
		NONE = -1, // 無効なシーンID
		REPLAY = 0,// リプレイ
		SELECT_STAGE,// ステージ選択
	};
public:
	// アクセサ
	// シーン番号取得
	SceneID GetSceneNum() const { return m_num; }
	// シーン番号設定
	void SetSceneNum(SceneID num) { m_num = num; }
	// このクラスが有効かどうかを取得
	bool IsEnable() const { return m_enable; }
	// このクラスが有効かどうかを設定
	void SetEnable(bool enable) { m_enable = enable; }
	// シーンから結果を受け取る
	void SetResult(bool gameOver, bool gameClear);
	// シーンからステージ番号を受け取る
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }
	// ステージ番号を取得
	int GetStageNum() const { return m_stageNum; }
public:
	// public関数
	// コンストラクタ
	ResultUI();
	// デストラクタ
	~ResultUI();
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
	// private定数
	// 無効なメニューインデックス
	static const int INVALID_MENU_INDEX;
private:
	// private関数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// ボタン
	std::vector<std::unique_ptr<Canvas>> m_pButtons;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 時間
	float m_time;
	// ヒットフラグ
	bool m_hit;
	// このクラスが有効かどうか
	bool m_enable;
	// ヒットフラグが立ったUIのインデックス
	int m_menuIndex;
	// シーンID
	SceneID m_num;
	// 今のステージ番号
	int m_stageNum;
};