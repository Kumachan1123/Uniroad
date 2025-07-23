/*
*	@file StageSelect.h
*	@brief ステージセレクトシーンのステージセレクトクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

class StageSelect
{
public:
	// public関数
	// コンストラクタ
	StageSelect(CommonResources* resources);
	// デストラクタ
	~StageSelect();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// モデルポインター
	DirectX::Model* m_pModel;

};