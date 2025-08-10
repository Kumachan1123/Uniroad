/*
*	@file Button.h
*	@brief ボタンの大元となるクラス
*/
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
#include "KumachiLib/MouseClick/MouseClick.h"

// 前方宣言
class CommonResources;

// ボタンの大元となるクラス
class Button
{
public:
	// public構造体
	// ボタンの矩形
	struct ButtonRect
	{
		DirectX::SimpleMath::Vector2 position; // 位置
		DirectX::SimpleMath::Vector2 size;     // サイズ
	};
private:
	// private構造体
	// シェーダーに渡す定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};

public:
	// アクセサ
	// 頂点シェーダーのファイルパスを設定
	void SetVertexShaderFilePath(std::string filePath) { m_vertexShaderFilePath = filePath; }
	// ピクセルシェーダーのファイルパスを設定
	void SetPixelShaderFilePath(std::string filePath) { m_pixelShaderFilePath = filePath; }
	// テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTextures.push_back(pTexture); }
public:
	// public関数
	// コンストラクタ
	Button();
	// デストラクタ
	~Button();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height);
	// 更新
	void Update(const float elapsedTime);
	// 画像を表示
	void DrawQuad(float centerX, float centerY, float width, float height, int frameIndex, int frameCols, int frameRows);
	// 当たり判定
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const ButtonRect& buttonRect);
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
	ConstBuffer m_constBuffer;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// 頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];

};