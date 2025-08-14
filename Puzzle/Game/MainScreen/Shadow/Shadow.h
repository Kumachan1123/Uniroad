/*
*	@file Shadow.h
*	@brief 影ポリゴンを描画するクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

// 前方宣言
class CommonResources;

// 影ポリゴンを描画するクラス
class Shadow
{
public:
	// public関数
	// コンストラクタ
	Shadow();
	// デストラクタ
	~Shadow() = default;
	// 初期化する
	void Initialize(CommonResources* resources);
	// 描画する
	void Render(const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::SimpleMath::Vector3& position,
		float radius = 0.7f);
private:
	// private関数
	// 深度ステンシルバッファを設定する
	void CreateDepthStenciBuffer(ID3D11Device* pDevice);
private:
	// private変数
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 深度ステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;

};