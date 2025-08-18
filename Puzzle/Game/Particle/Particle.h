/*
*	@file	Particle.h
*	@brief	パーティクルクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <algorithm>
#include <random>
#include <list>
// DirectXのライブラリ
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include <DeviceResources.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/Particle/Utility.h"
#include "KumachiLib/ShaderBuffer/ShaderBuffer.h"
// 前方宣言
class CommonResources;

// パーティクルクラス
class Particle
{
public:
	// アクセサ
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// カメラの位置を設定
	void SetCameraPosition(const DirectX::SimpleMath::Vector3& cameraPos) { m_cameraPosition = cameraPos; }
	// カメラの注視点を設定
	void SetCameraTarget(const DirectX::SimpleMath::Vector3& cameraTarget) { m_cameraTarget = cameraTarget; }
	// カメラの上方向を設定
	void SetCameraUp(const DirectX::SimpleMath::Vector3& cameraUp) { m_cameraUp = cameraUp; }
	// パーティクルのパラメーターを設定
	void SetParams(const Utility::ParticleParams& params) { m_params = params; }
public:
	// public関数
	// コンストラクタ
	Particle(Utility::Type type, float size);
	// デストラクタ
	~Particle();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// ビルボード行列の作成
	void CreateBillboard(
		const DirectX::SimpleMath::Vector3& target,
		const DirectX::SimpleMath::Vector3& eye,
		const DirectX::SimpleMath::Vector3& up);
private:
	// private関数
	// シェーダー作成
	void CreateShaders();
	// パーティクルごとの処理
	// 水蒸気の処理
	void Steam();
private:
	// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	// public変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 経過時間
	float m_timer;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャハンドルの配列
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// コンスタントバッファ（構造体）
	ParticleBuffer m_constantBuffer;
	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;
	// フレーム数
	int m_anim;
	// アニメーションの速度
	float m_animSpeed;
	// アニメーション時間
	float m_animTime;
	// フレーム時間
	float m_elapsedTime;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 発生させる座標
	DirectX::SimpleMath::Vector3 m_position;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// カメラの注視点
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上方向
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// ビルボード行列
	DirectX::SimpleMath::Matrix m_billboard;
	// 頂点
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	// パーティクルのユーティリティ
	std::list<Utility> m_particleUtility;
	// パーティクルタイプ
	Utility::Type m_type;
	// パーティクルのサイズ
	float m_size;
	// パーティクルのパラメーター
	Utility::ParticleParams m_params;


};