/*
*	@file Sky.h
*	@brief 空の描画を行うクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

// 前方宣言
class CommonResources;

// 空の描画を行うクラス
class Sky
{
public:
	// アクセサ
	// 座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// スケールを取得する
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// スケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// 回転速度を取得する
	float GetRotationSpeed() const { return m_rotationSpeed; }
	// 回転速度を設定する
	void SetRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }

public:
	// public関数
	// コンストラクタ
	Sky(CommonResources* resources);
	// デストラクタ
	~Sky();
	// 初期化
	void Initialize();
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 終了
	void Finalize();

private:
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 空の位置
	DirectX::SimpleMath::Vector3 m_position;
	// 空の回転角
	DirectX::SimpleMath::Quaternion m_rotation;
	// 空のスケール
	DirectX::SimpleMath::Vector3 m_scale;
	// 空のモデルへのポインタ
	DirectX::Model* m_pModel;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビューマトリックス
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクションマトリックス
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
	// 回転速度
	float m_rotationSpeed;
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
};