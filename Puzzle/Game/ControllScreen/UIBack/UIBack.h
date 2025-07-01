/*
*	@file UIBack.h
*	@brief	操作画面の背景クラス　
*/
#pragma once
#ifndef UIBACK_DEFINED
#define UIBACK_DEFINED
// 標準ライブラリ
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/DrawPolygon/DrawPolygon.h"
#include "KumachiLib/CreateShader/CreateShader.h"

// 前方宣言
class CommonResources;

// 操作画面の背景クラス
class UIBack
{
public:
	// 構造体
	// シェーダーに渡すデータ構造体
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 colors;    // カラー
		DirectX::SimpleMath::Vector4 time;		// 時間                            
	};
public:
	// public関数
	// コンストラクタ
	UIBack(CommonResources* resources);
	// デストラクタ
	~UIBack();
	// 生成
	void Create(DX::DeviceResources* pDR);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:
	// private関数
	// シェーダーの作成
	void CreateShaders();
private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// 共通リソース
	CommonResources* m_pCommonResources;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// コンスタントバッファ（シェーダーに送るデータ）
	ConstBuffer m_constBuffer;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 時間
	float m_time;
};
#endif // UIBACK_DEFINED