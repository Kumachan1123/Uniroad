/*
*	@file MedalCounter.h
*	@brief メダルカウンターの処理を定義するクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/CreateShader/CreateShader.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"

// 前方宣言
class CommonResources;

// メダルカウンタークラス
class MedalCounter
{
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
	// カウンターの単位
	struct Counter
	{
		int unit1 = 0;// 1の位
		int unit10 = 0;// 10の位
	};
public:
	// public関数
	// コンストラクタ
	MedalCounter();
	// 初期化 
	void Initialize(CommonResources* resources, int width, int height);
	// 更新する
	void Update(float elapsedTime);
	// 描画する
	void Render();

private:
	// private関数
	// シェーダーの作成
	void CreateShaders();
	// 画像を表示
	void DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
		DirectX::VertexPositionTexture* vertices,
		float startX, float startY, float width, float height,
		int frameIndex, int frameCols, int frameRows);
private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	// public定数
	// 頂点数
	static const int VERTEX_COUNT = 4;
	// メダル画像の位置X
	const float MEDAL_POS_X = -1.0f;
	// メダル画像の位置Y
	const float MEDAL_POS_Y = 1.0f;
	// メダル画像のサイズX
	const float MEDAL_SIZE_X = 0.1f;
	// メダル画像のサイズY
	const float MEDAL_SIZE_Y = 0.1f / 0.58f;
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
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	// 集めたメダル枚数
	Counter m_collectedMedalCount;
	// 集めたメダル枚数（保存用）
	int m_collectedMedalCountSave;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// メダルのテクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pMedalTextures;
	// メダルの頂点情報
	DirectX::DX11::VertexPositionTexture m_verticesRemaining[VERTEX_COUNT];

};