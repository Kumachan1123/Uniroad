/*
*	@file	DrawPolygon.h
*	@brief	板ポリゴン描画用クラス
*	@details 板ポリゴンを描画するためのクラス
*/
#pragma once
#ifndef DRAW_POLYGON_DEFINED
#define DRAW_POLYGON_DEFINED
// 標準ライブラリ
#include <vector>
// DirectXのヘッダファイル
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include "Game/CommonResources/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/Microsoft/DebugDraw.h>

// 前方宣言
class CommonResources;

// 板ポリゴン描画用クラス
class DrawPolygon
{
public:
	// 構造体
	// 三つのシェーダーをひとまとめにして送るための構造体
	struct Shaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;// 頂点シェーダー
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;// ピクセルシェーダー
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> gs;// ジオメトリシェーダー
	};
public:
	// 列挙型
	// サンプラーステートの種類
	enum class SamplerStates
	{
		ANISOTROPIC_CLAMP,// アニソトロピッククランプ
		ANISOTROPIC_WRAP,// アニソトロピックラップ
		LINEAR_CLAMP,// リニアクランプ
		LINEAR_WRAP,// リニアラップ
		POINT_CLAMP,// ポイントクランプ
		POINT_WRAP,// ポイントラップ
	};
	// ブレンドステートの種類
	enum class BlendStates
	{
		ALPHA,	// アルファブレンド
		ADDITIVE,	// 加算ブレンド
		OPAQUE,	// 不透明
		NONPREMULTIPLIED,	// 非プリマルチプライド
	};
	// ラスタライザーステートの種類
	enum class RasterizerStates
	{
		CULL_CLOCKWISE,// 時計回り
		CULL_COUNTERCLOCKWISE,// 反時計回り
		CULL_NONE,// カリングなし
		WIREFRAME,// ワイヤーフレーム
	};
	// 深度ステンシルステートの種類
	enum class DepthStencilStates
	{
		DEPTH_DEFAULT,// デフォルト
		DEPTH_NONE,// 深度なし
		DEPTH_READ,	// 深度読み取り
		DEPTH_READ_REVERSE_Z,// 深度読み取り（逆Z）
		DEPTH_REVERSE_Z,// 逆Z
	};

public:
	// シングルトンインスタンス
	// シングルトンインスタンスを取得
	static DrawPolygon* const GetInstance();
public:
	// public関数
	// デストラクタ
	~DrawPolygon();
	// 初期化（頂点、テクスチャ）
	void InitializePositionTexture(DX::DeviceResources* pDR);
	// 初期化（頂点、色、テクスチャ）
	void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// 描画開始
	void DrawStart(ID3D11InputLayout* pInputLayout, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures);
	// 描画前設定
	void DrawSetting(SamplerStates ss, BlendStates bs, RasterizerStates rs, DepthStencilStates dss);
	// サブリソースの更新
	void UpdateSubResources(ID3D11Resource* resource, const void* pSrcData);
	// 板ポリゴン描画（頂点、テクスチャ）
	void DrawTexture(const DirectX::DX11::VertexPositionTexture* vertices);
	// 板ポリゴン描画（頂点、色、テクスチャ）
	void DrawColorTexture(D3D_PRIMITIVE_TOPOLOGY topology, const DirectX::DX11::VertexPositionColorTexture* vertices, size_t count);
	// 解放（頂点、テクスチャ）
	void ReleasePositionTexture();
	// 解放（頂点、色、テクスチャ）
	void ReleasePositionColorTexture();
	// シェーダーにバッファを送る
	void SetShaderBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppBuffer);
	// シェーダーをセットする
	void SetShader(const Shaders& shaders, ID3D11ClassInstance* const* ppClassInstances, UINT nubClassInstances);
	// シェーダーを解放する
	void ReleaseShader();
private:
	// private関数
	// コンストラクタ
	DrawPolygon();
	// コピーコンストラクタ
	DrawPolygon(const DrawPolygon&) = delete;
	// コピーコンストラクタと代入演算子の禁止
	DrawPolygon& operator=(const DrawPolygon&) = delete;
private:
	// private変数
	// 共通リソース
	ID3D11DeviceContext1* m_pDeviceContext;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// デバイス
	ID3D11Device1* m_pDevice;
	// プリミティブバッチ（頂点、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatchTexture;
	// プリミティブバッチ（頂点、色、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_pPrimitiveBatchColorTexture;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// シングルトンインスタンス
	static std::unique_ptr<DrawPolygon> m_pInstance;
};
#endif // DRAW_POLYGON_DEFINED