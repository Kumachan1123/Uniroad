/*
*	@file ResultAnimation.h
*	@brief 結果アニメーションクラス
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
#include "KumachiLib/Easing/Easing.h"
#include "KumachiLib/ShaderBuffer/ShaderBuffer.h"

// 前方宣言
class CommonResources;

// 結果アニメーションクラス
class ResultAnimation
{
public:
	// アクセサ
	// プレイシーンから結果を受け取る
	void SetResult(bool gameover, bool gameclear);
	// アニメーション終了フラグを取得する
	bool IsAnimationEnd() const { return m_animationEnd; }
	// アニメーション終了フラグを設定する
	void SetAnimationEnd(bool end) { m_animationEnd = end; }
	// アニメーション有効フラグを取得する
	bool IsAnimationEnable() const { return m_animationEnable; }
	// アニメーション有効フラグを設定する
	void SetAnimationEnable(bool enable) { m_animationEnable = enable; }

public:
	// public関数
	// コンストラクタ
	ResultAnimation();
	// デストラクタ
	~ResultAnimation();
	// 初期化
	void Initialize(CommonResources* resources);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// テクスチャ決定
	void DecideTexture();
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
	static constexpr int VERTEX_COUNT = 4;
	// アニメーション時間
	static constexpr float ANIMATION_TIME = 3.0f;
	// ゲームクリアの時の列数
	static constexpr int CLEAR_COLS = 9;
	// ゲームオーバーの時の列数
	static constexpr int OVER_COLS = 8;
	// 画像の位置X(ゲームクリア）
	const float  POS_X_CLEAR = -0.7f;
	// 画像の位置X(ゲームオーバー）
	const float POS_X_OVER = -0.65f;
	// 画像の位置Y
	const float POS_Y = 0.75;
	// 画像のサイズ
	static constexpr DirectX::SimpleMath::Vector2 SIZE = DirectX::SimpleMath::Vector2(0.16f, 0.32f);
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// ゲームオーバーフラグ
	bool m_gameover;
	// ゲームクリアフラグ
	bool m_gameclear;
	// 時間
	float m_time;
	// アニメーション有効フラグ
	bool m_animationEnable;
	// アニメーション終了フラグ
	bool m_animationEnd;
	// 画像決定フラグ
	bool m_decideTexture;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// 画像の位置X
	float m_positionX;
	// 個別に操作するための画像の位置Y
	std::vector<float> m_positionsY;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// コンスタントバッファ
	SpriteSheetBuffer m_constBuffer;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// 頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[VERTEX_COUNT];
};
