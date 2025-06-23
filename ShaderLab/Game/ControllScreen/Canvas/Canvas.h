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
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
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
		DirectX::SimpleMath::Vector4 windowSize;// ウィンドウサイズ
		float time;// 時間
		DirectX::SimpleMath::Vector3 color;// 色
	};
public:
	// アクセサ
		// ウィンドウのサイズを設定
	void SetWindowSize(const int& width, const int& height);
	// シェーダータイプを設定
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; }
	// スケールを取得
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	// スケールを設定
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }
	// 選択状態のスケールを取得
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	// 位置を取得
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; };
	// アンカーを取得
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	// アンカーを設定
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }
	// 時間を取得
	float GetTime() const { return m_time; }
	// 時間を設定
	void SetTime(float time) { m_time = time; }
	// 指定位置にヒットしたかどうかを取得
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
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
	// 時間
	float m_time;
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
};

