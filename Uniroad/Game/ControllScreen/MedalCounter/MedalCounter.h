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
#include "KumachiLib/ShaderBuffer/ShaderBuffer.h"

// 前方宣言
class CommonResources;

// メダルカウンタークラス
class MedalCounter
{
private:
	// private構造体
	// カウンターの単位
	struct Counter
	{
		int unit1 = 0;// 1の位
		int unit10 = 0;// 10の位
	};
public:
	// アクセサ
	// 集めたメダル枚数を取得する
	int GetCollectedMedalCount() const { return m_collectedMedalCountSave; }
	// 集めたメダル枚数を設定する
	void SetCollectedMedalCount(int count) { m_collectedMedalCountSave = count; }
public:
	// public関数
	// コンストラクタ
	MedalCounter();
	// デストラクタ
	~MedalCounter();
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
		const DirectX::SimpleMath::Vector2& startPos, const DirectX::SimpleMath::Vector2& size,
		int frameIndex, int frameCols, int frameRows);
private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 頂点数
	static const int VERTEX_COUNT = 4;
	// メダル画像の位置
	const DirectX::SimpleMath::Vector2 MEDAL_POS = DirectX::SimpleMath::Vector2(-1.0f, 1.0f);
	// メダル画像のサイズ
	const DirectX::SimpleMath::Vector2 MEDAL_SIZE = DirectX::SimpleMath::Vector2(0.11f, 0.11f / 0.58f);
	// 「×」画像の位置
	const DirectX::SimpleMath::Vector2 X_POS = DirectX::SimpleMath::Vector2(-0.9f, 0.99f);
	// 「×」画像のサイズ
	const DirectX::SimpleMath::Vector2 X_SIZE = DirectX::SimpleMath::Vector2(0.078f, 0.1f / 0.58f);
	// 10の位の位置
	const DirectX::SimpleMath::Vector2 NUMBER10_POS = DirectX::SimpleMath::Vector2(-0.82f, 0.99f);
	// 10の位のサイズ
	const DirectX::SimpleMath::Vector2 NUMBER10_SIZE = DirectX::SimpleMath::Vector2(0.078f, 0.1f / 0.58f);
	// 1の位の位置
	const DirectX::SimpleMath::Vector2 NUMBER1_POS = DirectX::SimpleMath::Vector2(-0.745f, 0.99f);
	// 1の位のサイズ
	const DirectX::SimpleMath::Vector2 NUMBER1_SIZE = DirectX::SimpleMath::Vector2(0.078f, 0.1f / 0.58f);
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
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// コンスタントバッファ
	SpriteSheetBuffer m_constBuffer;
	// 集めたメダル枚数
	Counter m_collectedMedalCount;
	// 集めたメダル枚数（保存用）
	int m_collectedMedalCountSave;
	// 入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// メダルのテクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pMedalTextures;
	// メダルの頂点情報
	DirectX::DX11::VertexPositionTexture m_verticesMedal[VERTEX_COUNT];
	// 「×」の画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pXTextures;
	// 「×」の頂点情報
	DirectX::DX11::VertexPositionTexture m_verticesX[VERTEX_COUNT];
	// 数字画像
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pNumberTextures;
	// 数字の頂点情報(10の位)
	DirectX::DX11::VertexPositionTexture m_verticesNumber10[VERTEX_COUNT];
	// 数字の頂点情報(1の位)
	DirectX::DX11::VertexPositionTexture m_verticesNumber1[VERTEX_COUNT];
};