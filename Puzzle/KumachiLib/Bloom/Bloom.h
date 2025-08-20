/*
*	@file Bloom.h
*	@brief ブルームエフェクトのヘッダファイル
*	@details 関数を呼ぶだけでブルームエフェクトを実装できる
*/
#pragma once
#ifndef BLOOM_DEFINED
#define BLOOM_DEFINED
// DirectXのヘッダファイル
#include <PostProcess.h>
#include <CommonStates.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/Microsoft/RenderTexture/RenderTexture.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"

//前方宣言
class CommonResources;

// ブルームエフェクトクラス
class Bloom
{
public:
	// シングルトンインスタンス
	// シングルトンインスタンスを取得
	static Bloom* const GetInstance();
public:
	// 構造体
	// ブラー
	struct Blur
	{
		ID3D11RenderTargetView* RTV;// レンダーターゲットビュー
		ID3D11ShaderResourceView* SRV;// シェーダーリソースビュー
	};

public:
	// public関数
	// デストラクタ
	~Bloom();
	// ポストプロセスを生成
	void CreatePostProcess(CommonResources* resources);
	// オフスクリーン描画用にRTVを切り替える
	void ChangeOffScreenRT();
	// ポストプロセスに必要な設定を準備する
	void PostProcess();
private:
	// private関数
	// コンストラクタ
	Bloom();
	// レンダーテクスチャを作成する
	void CreateRenderTexture();
	// コピーコンストラクタ
	Bloom(const Bloom&) = delete;
	// 代入演算子の禁止
	Bloom& operator=(const Bloom&) = delete;
private:
	//private変数
	// シングルトンインスタンス
	static std::unique_ptr<Bloom> m_pInstance;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 共通リソース
	ID3D11DeviceContext1* m_pDeviceContext;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// デバイス
	ID3D11Device1* m_pDevice;
	// ブルームエフェクトのコモンステート
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// ブラー1
	Blur m_blur1;
	// ブラー2
	Blur m_blur2;
	// ポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_pBasicPostProcess;
	// デュアルポストプロセス
	std::unique_ptr<DirectX::DualPostProcess> m_pDualPostProcess;
	// スクリーンサイズ
	RECT m_screenSize;
	// オフスクリーン用のレンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_pOffScreenRT;
	// ブラー1用のレンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_pBlur1RT;
	// ブラー2用のレンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_pBlur2RT;
	// 保存されたオフスクリーン用のレンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_pSavedOffScreenRT;
	// オフスクリーン用のレンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pOffScreenRTV;
	// オフスクリーン用のシェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pOffScreenSRV;
	// デフォルトの深度ステンシルビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDefaultDSV;
	// デフォルトのレンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pDefaultRTV;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch;
};
#endif //BLOOM_DEFINED
