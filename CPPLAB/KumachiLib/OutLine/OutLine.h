/*
*	@file 	OutLine.h
*	@brief	輪郭線描画クラス
*/
#pragma once



// 輪郭線描画クラス
class OutLine
{
public:
	// アクセサ
	// ビューポートを設定する
	void SetViewport(const D3D11_VIEWPORT& viewport) { m_viewport = viewport; }
	// モデルとワールド行列のペアを設定
	void SetOutLineModel(DirectX::Model* model, const  DirectX::SimpleMath::Matrix& world) { m_pOutLineInfos.push_back(std::make_pair(model, world)); }
	// FOVのθを設定
	void SetFovTheta(float fovTheta) { m_fovTheta = fovTheta; }
public:
	// public関数
	// コンストラクタ
	OutLine();
	// デストラクタ
	~OutLine();
	// 輪郭になるモデルを描画
	void RenderOutLine(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	// private関数
	// シェーダをロードする
	void LoadShader(ID3D11Device* device);
	// バッファを作成する
	void CreateConstanBuffer(ID3D11Device* device);
private:
	// 定数
	// 輪郭の太さ
	static constexpr float OUTLINE_WIDTH = 0.01f;
private:
	// private変数
	// Direct3Dデバイス
	ID3D11Device1* m_pDevice;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
	// 輪郭にしたいモデルとワールド行列のペア配列
	std::vector<std::pair<DirectX::Model*, DirectX::SimpleMath::Matrix>> m_pOutLineInfos;
	// 通常描画用ビューポート
	D3D11_VIEWPORT m_viewport;
	// アウトライン用バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pOutlineConstantBuffer;
	// FOVのθ
	float m_fovTheta;
};