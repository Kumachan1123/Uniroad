#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries\MyLib\InputManager.h>
// 前方宣言
class CommonResources;

// Androidクラス
class Android
{
public:
	// コンストラクタ
	Android();
	// デストラクタ defaultで定義
	~Android() = default;
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();
private:
	void Move(float elapsedTime);
	void Jump(float elapsedTime);
private:
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// モデルポインタ
	std::unique_ptr<DirectX::Model> m_pModel;
	// 時間
	float m_time;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 接地状態
	bool m_isGrounded;
	// 前フレームのジャンプキー状態
	bool m_wasJumpKeyDown;
	// ボーン変換行列
	DirectX::ModelBone::TransformArray m_boneTransforms;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};