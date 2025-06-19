/*
*	@file Canvas.h
*	@brief キャンバスクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/Anchor/Anchor.h"

// 前方宣言
class CommonResources;

class Canvas
{
public:
	// 列挙型
	// シェーダーの種類
	enum class ShaderType
	{
		HP = 0,// HPゲージのような動きをする
		CIRCLE,// 円形ゲージのような動きをする
		ANIM,// UVアニメーションさせる
		OTHER,// その他
	};
	// 構造体
	// シェーダーに送る定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;	// ウィンドウサイズ
		DirectX::SimpleMath::Vector2	frame;		// フレーム数
		float							animCount = 0.0f;	// アニメーションカウント
		float							renderRatio = 0.0f;// レンダリング比率
	};
public:
	// アクセサ
	// ウィンドウサイズの設定
	void SetWindowSize(const int& width, const int& height);
	// スケールの設定
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	// スケールの取得
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	// ベーススケールの取得
	DirectX::SimpleMath::Vector2 GetBaseScale() const { return m_baseScale; }
	// 描画位置の取得
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// 描画位置の設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	// アンカーの取得
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	// アンカーの設定
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }
	// レンダリング比率の取得
	float GetRenderRatio() const { return m_renderRatio; }
	// レンダリング比率の設定
	void SetRenderRatio(float ratio) { m_renderRatio = ratio; }
	// レンダリング比率のオフセットの取得
	float GetRenderRatioOffset() const { return m_renderRatioOffset; }
	// レンダリング比率のオフセットの設定
	void SetRenderRatioOffset(float offset) { m_renderRatioOffset = offset; }
	// シェーダーの種類の設定
	void SetShaderType(ShaderType type) { m_shaderType = type; }
	// アニメーションのフレーム数の設定
	void SetAnim(int anim) { m_anim = anim; }
	// フレームの行数の設定
	void SetFrameRows(int rows) { m_frameRows = rows; }
	// フレームの列数の設定
	void SetFrameCols(int cols) { m_frameCols = cols; }
public:
	// コンストラクタ
	Canvas(CommonResources* pCommonResources);
	// デストラクタ
	~Canvas();
	// テクスチャの読み込み
	void LoadTexture(std::string key);
	// UIの作成
	void Create(DX::DeviceResources* pDR
		, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
	// 描画
	void Render();
private:
	// private関数
	// シェーダーの作成
	void CreateShaders();
private:
	// 定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private変数
// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	// テクスチャハンドルの配列
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// リソースハンドル
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// コンスタントバッファ（シェーダーに送るデータ）
	ConstBuffer m_constBuffer;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// テクスチャの幅と高さ
	int m_textureWidth, m_textureHeight;
	// フレームの行数 
	int m_frameRows;
	// フレームの列数
	int m_frameCols;
	// フレーム数
	int m_anim;
	// スケール
	DirectX::SimpleMath::Vector2 m_scale;
	// ベーススケール
	DirectX::SimpleMath::Vector2 m_baseScale;
	// 描画位置
	DirectX::SimpleMath::Vector2 m_position;
	// アンカー
	KumachiLib::ANCHOR m_anchor;
	// シェーダーの種類
	ShaderType m_shaderType;
	// レンダリング比率
	float m_renderRatio;
	// レンダリング比率のオフセット
	float m_renderRatioOffset;
};

