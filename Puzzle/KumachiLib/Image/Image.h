/*
*	@file Image.h
*	@brief 画像クラス
*/
#pragma once
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/ShaderBuffer/ShaderBuffer.h"
// 前方宣言
class CommonResources;

// 画像クラス
class Image
{
public:
	// アクセサ
	// 頂点シェーダーのファイルパスを設定
	void SetVertexShaderFilePath(std::string filePath) { m_vertexShaderFilePath = filePath; }
	// ピクセルシェーダーのファイルパスを設定
	void SetPixelShaderFilePath(std::string filePath) { m_pixelShaderFilePath = filePath; }
	// テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTextures.push_back(pTexture); }
	// バッファサイズを設定
	void SetShaderBufferSize(UINT size) { m_bufferSize = size; }
public:
	// public関数
	// コンストラクタ
	Image();
	// デストラクタ
	~Image();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height);
	// 更新
	void Update(const float elapsedTime);

	// 画像を表示
	template<typename T>
	void DrawQuadWithBuffer(const Rect& buttonRect, const T& buffer);
private:
	// private関数
	// シェーダーの作成
	void CreateShaders();
private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 頂点数
	static const int VERTEX_COUNT = 4;
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// 描画可能範囲の幅
	int m_viewportWidth;
	// 描画可能範囲の高さ
	int m_viewportHeight;
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// 頂点シェーダーのファイルパス
	std::string m_vertexShaderFilePath;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// ピクセルシェーダーのファイルパス
	std::string m_pixelShaderFilePath;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// コンスタントバッファ
	SpriteSheetBuffer m_constBuffer;
	// バッファサイズ
	UINT m_bufferSize;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// 頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];
};

/*
*	@brief 画像を表示
*	@details 画像を表示する
*	@param rect 矩形
*	@param buffer シェーダーバッファ
*	@return なし
*/
template<typename T>
inline void Image::DrawQuadWithBuffer(const Rect& buttonRect, const T& buffer)
{
	// 名前空間の使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	float correctedHeight = buttonRect.size.y * aspect;
	// ボタンの矩形を設定
	float startX = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float startY = buttonRect.position.y - correctedHeight / 2.0f;
	// NDC座標系に変換
	Vector2 ndcLT = ToNDC(Vector2(startX, startY));
	Vector2 ndcRB = ToNDC(Vector2(startX + buttonRect.size.x, startY + correctedHeight));
	// 頂点座標の設定
	VertexPositionTexture vertices[VERTEX_COUNT]{};
	vertices[0] = { VertexPositionTexture(Vector3(ndcLT.x, ndcLT.y, 0), Vector2(0, 0)) };// 左上
	vertices[1] = { VertexPositionTexture(Vector3(ndcRB.x, ndcLT.y, 0), Vector2(1, 0)) };// 右上
	vertices[2] = { VertexPositionTexture(Vector3(ndcRB.x, ndcRB.y, 0), Vector2(1, 1)) };// 右下
	vertices[3] = { VertexPositionTexture(Vector3(ndcLT.x, ndcRB.y, 0), Vector2(0, 1)) };// 左下
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &buffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
