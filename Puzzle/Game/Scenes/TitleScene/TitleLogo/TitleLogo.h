/*
*	@file TitleLogo.h
*	@brief タイトルロゴクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"
#include "Game/Interface/IImage.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Easing/Easing.h"
//#include "KumachiLib/AnimationStep/AnimationStep.h"
#include "KumachiLib/Animaiton/Animation.h"

// 前方宣言
class CommonResources;

// タイトルロゴクラス
class TitleLogo : public IImage
{
public:
	// アクセサ
	// ロゴの位置を取得
	const DirectX::SimpleMath::Vector2& GetPosition() const override { return m_position; }
	// ロゴの位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) override { m_position = position; }
	// ロゴのサイズを取得
	const DirectX::SimpleMath::Vector2& GetSize() const override { return m_size; }
	// ロゴのサイズを設定
	void SetSize(const DirectX::SimpleMath::Vector2& size) override { m_size = size; }
public:
	// public関数
	// コンストラクタ
	TitleLogo();
	// デストラクタ
	~TitleLogo();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新
	void Update(float elapsedTime)override;
	// 画像を表示
	void Render()override;
private:
	// private関数
	// アニメーションシーケンスを作成
	void CreateAnimationSequence();
private:
	// private定数
	// 表示位置（左上）
	static const DirectX::SimpleMath::Vector2 POSITION;
	// 表示サイズ
	static const DirectX::SimpleMath::Vector2 SIZE;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 画像
	std::unique_ptr<Image> m_pImage;
	// アニメーション
	std::unique_ptr<Animation> m_pAnimation;
	// ロゴの位置
	DirectX::SimpleMath::Vector2 m_position;
	// ロゴのサイズ
	DirectX::SimpleMath::Vector2 m_size;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// ロゴの矩形
	Rect m_logoRect;
	// アニメーション進行度
	float m_animationTime;
	// アニメーションのフェーズ
	int m_animationPhase;
	// 現在のアニメーションステップ
	size_t m_currentStep;
	// アニメーションステップ時間
	float m_animStepTime;
	// アニメーションシーケンス
	std::vector<AnimationStep> m_animSequence;
};