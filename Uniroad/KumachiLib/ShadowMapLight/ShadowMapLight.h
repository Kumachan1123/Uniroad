/*
*	@file ShadowMapLight.h
*	@brief シャドウマップ用ライトクラス
*/
#pragma once
// 外部ライブラリ
#include "Libraries/Microsoft/ReadData.h"
#include "Libraries/Microsoft/RenderTexture/RenderTexture.h"
#include "Libraries/MyLib/DepthStencil.h"
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

// 前方宣言
class CommonResources;
namespace DX
{
	class RenderTexture;
}
// シャドウマップ用ライトクラス
class ShadowMapLight
{
private:
	// ライトの定数バッファ用構造体
	struct cbLight
	{
		DirectX::XMMATRIX lightViewProjection;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;		// ライトの位置
		DirectX::XMVECTOR lightDirection;		// ライトの方向
		DirectX::XMVECTOR lightAmbient;			// ライトの環境光
	};
public:
	// アクセサ
	// モデルとワールド行列のペアを設定
	void SetShadowModel(DirectX::Model* model, const  DirectX::SimpleMath::Matrix& world) { m_pShadowInfos.push_back(std::make_pair(model, world)); }
	// ライトの座標を設定
	void SetLightPosition(const DirectX::SimpleMath::Vector3& position) { m_lightPosition = position; }
	// ビューポートを設定する
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }


public:
	// public関数
	// コンストラクタ
	ShadowMapLight(CommonResources* commonResources);
	// デストラクタ
	~ShadowMapLight();
	// 更新
	void Update(float elapsedTime);
	// 影になるモデルを描画
	void RenderShadow();
	// モデル描画時のラムダ式内で呼ぶ処理をまとめた関数
	void ApplyShader(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states);
private:
	// private関数
	// シェーダをロードする
	void LoadShader(ID3D11Device* device);
	// バッファを作成する
	void CreateConstanBuffer(ID3D11Device* device);
	// シャドウマップ用の各種オブジェクトを作成する
	void CreateShadowMapObject(ID3D11Device* device);
private:
	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// Direct3Dデバイス
	ID3D11Device1* m_pDevice;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
	// ライトの座標
	DirectX::SimpleMath::Vector3 m_lightPosition;
	// ライトの回転
	DirectX::SimpleMath::Quaternion m_lightQuaternion;
	// ライトの画角：ライトから見た風景を映すカメラの画角
	float m_lightTheta;
	// 定数バッファ用のバッファオブジェクト
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_pShadowMapRT;
	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<mylib::DepthStencil> m_pShadowMapDS;
	// 頂点シェーダ(シャドウマップ用)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVSShadowMap;
	// ピクセルシェーダ(シャドウマップ用)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPSShadowMap;
	// サンプラ
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pShadowMapSampler;
	// 影にしたいモデルとワールド行列のペア配列
	std::vector<std::pair<DirectX::Model*, DirectX::SimpleMath::Matrix>> m_pShadowInfos;
	// シェーダーリソースビュー
	ID3D11ShaderResourceView* m_pSRV;
	// 通常描画用ビューポート
	D3D11_VIEWPORT m_viewport;
};