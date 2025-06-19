/*
	@file	DrawPolygon.h
	@brief	当たり判定描画用クラス
*/
#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "DeviceResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
//前方宣言
class CommonResources;

class DrawPolygon
{
public:// 構造体
	// 三つのシェーダーをひとまとめにして送るための構造体
	struct Shaders
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> gs;
	};
public:// 列挙型
	// サンプラーステートの種類
	enum class SamplerStates
	{
		ANISOTROPIC_CLAMP,
		ANISOTROPIC_WRAP,
		LINEAR_CLAMP,
		LINEAR_WRAP,
		POINT_CLAMP,
		POINT_WRAP
	};
	// ブレンドステートの種類
	enum class BlendStates
	{
		ALPHA,
		ADDITIVE,
		OPAQUE,
		NONPREMULTIPLIED
	};
	// ラスタライザーステートの種類
	enum class RasterizerStates
	{
		CULL_CLOCKWISE,
		CULL_COUNTERCLOCKWISE,
		CULL_NONE,
		WIREFRAME
	};
	// 深度ステンシルステートの種類
	enum class DepthStencilStates
	{
		DEPTH_DEFAULT,
		DEPTH_NONE,
		DEPTH_READ,
		DEPTH_READ_REVERSE_Z,
		DEPTH_REVERSE_Z
	};

public:
	// シングルトンインスタンスを取得
	static DrawPolygon* const GetInstance();

public:

	// デストラクタ
	~DrawPolygon();

	// 初期化（頂点、テクスチャ）
	void InitializePositionTexture(DX::DeviceResources* pDR);
	// 初期化（頂点、色、テクスチャ）
	void InitializePositionColorTexture(DX::DeviceResources* pDR);
	// 描画開始
	void DrawStart(ID3D11InputLayout* pInputLayout, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures);
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
	// コンストラクタ
	DrawPolygon();
	// コピーコンストラクタと代入演算子の禁止
	DrawPolygon(const DrawPolygon&) = delete;
	DrawPolygon& operator=(const DrawPolygon&) = delete;

private:
	// 共通リソース
	ID3D11DeviceContext1* m_context;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	ID3D11Device1* m_device;
	// プリミティブバッチ（頂点、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;
	// プリミティブバッチ（頂点、色、テクスチャ）
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatchColorTexture;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// シングルトンインスタンス
	static std::unique_ptr<DrawPolygon> m_instance;
};