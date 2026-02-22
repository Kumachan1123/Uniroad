/*
*		@file Material.h
* 		@brief 3Dモデルのマテリアルクラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
#include <string>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
class CommonResources;
class Material
{
	// アクセサ
public:
	// RGB値を取得する
	DirectX::SimpleMath::Vector4 GetRGB() const { return m_rgb; }
	// RGB値を設定する
	void SetRGB(const DirectX::SimpleMath::Vector4& rgb) { m_rgb = rgb; }
	// シェーダーの名前を取得する
	std::wstring GetShaderName() const { return m_shaderName; }
	// シェーダーの名前を設定する
	void SetShaderName(const std::wstring& shaderName) { m_shaderName = shaderName; }
	// 頂点シェーダーを取得する
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader() const { return m_vs; }
	// ピクセルシェーダーを取得する
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader() const { return m_ps; }
	// 環境マップのテクスチャを取得する
	ID3D11ShaderResourceView* GetEnvironmentCubeSRV() const { return m_envCubeSRV.Get(); }
	// 環境マップのテクスチャを設定する
	void SetEnvironmentCubeSRV(ID3D11ShaderResourceView* envCubeSRV) { m_envCubeSRV = envCubeSRV; }
	// ノーマルマップを取得する
	ID3D11ShaderResourceView* GetNormalMap() const { return m_normalMap.Get(); }
	// ノーマルマップを設定する
	void SetNormalMap(ID3D11ShaderResourceView* normalMap) { m_normalMap = normalMap; }
	// 発光マップを取得する
	ID3D11ShaderResourceView* GetEmissiveMap() const { return m_emissiveMap.Get(); }
	// 発光マップを設定する
	void SetEmissiveMap(ID3D11ShaderResourceView* emissiveMap) { m_emissiveMap = emissiveMap; }
	// アンビエントオクルージョンマップを取得する
	ID3D11ShaderResourceView* GetAO() const { return m_ao.Get(); }
	// アンビエントオクルージョンマップを設定する
	void SetAO(ID3D11ShaderResourceView* ao) { m_ao = ao; }
	// 粗さマップを取得する
	ID3D11ShaderResourceView* GetRoughnessMap() const { return m_roughnessMap.Get(); }
	// 粗さマップを設定する
	void SetRoughnessMap(ID3D11ShaderResourceView* roughnessMap) { m_roughnessMap = roughnessMap; }



public:
	// public関数
	// コンストラクタ
	Material();
	// デストラクタ defaultで定義
	~Material() = default;
	// 初期化
	void Initialize(CommonResources* resources);

	// 終了
	void Finalize();
	// 各テクスチャをシェーダーにセットし、モデルにシェーダーを渡す
	void SetShaders(ID3D11DeviceContext* context, DirectX::DX11::CommonStates* commonStates);
private:
	// 頂点シェーダーを作る
	void CreateVertexShader(const std::vector<uint8_t>& blob);
	// ピクセルシェーダーを作る
	void CreatePixelShader(const std::vector<uint8_t>& blob);
private:
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 環境マップのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_envCubeSRV;
	// ノーマルマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMap;
	// 発光マップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_emissiveMap;
	// アンビエントオクルージョンマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ao;
	// 粗さマップ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roughnessMap;
	// シェーダーの名前
	std::wstring m_shaderName;
	// RGB値
	DirectX::SimpleMath::Vector4 m_rgb;
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
};
