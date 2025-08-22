/*
*	@file CreateShaders.h
*	@brief シェーダーを作成するシングルトンクラス
*	@details 板ポリゴンを描画する際に使用するシェーダーを作成する
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <utility>
// 自作ヘッダーファイル
#include "KumachiLib/BinaryFile/BinaryFile.h"

// シェーダーを作成するシングルトンクラス
class CreateShader
{
public:
	// シングルトンインスタンス
	// シングルトンインスタンスを取得
	static CreateShader* const GetInstance();
public:
	// public関数
	// デストラクタ
	~CreateShader();
	// 初期化
	void Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	// 初期化（デバイスのみ）
	void Initialize(ID3D11Device1* device);
	// 頂点シェーダを作成
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);
	// ピクセルシェーダーを作成
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
	// ジオメトリシェーダーを作成
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);
	// コンピュートシェーダーを作成
	void CreateComputeShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11ComputeShader>& cs);
	// 作ったインプットレイアウトを返す
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }
	// コンスタントバッファを作成
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);
private:
	// private関数
	// コンストラクタ
	CreateShader();
	// コピーコンストラクタ
	CreateShader(const CreateShader&) = delete;
	// 代入演算子の禁止
	CreateShader& operator=(const CreateShader&) = delete;
private:
	// private変数
	// シングルトンインスタンス
	static std::unique_ptr<CreateShader> m_pInstance;
	// デバイス
	ID3D11Device1* m_pDevice;
	// 入力レイアウト
	const D3D11_INPUT_ELEMENT_DESC* m_pIED;
	// 要素数
	UINT m_NumElements;
	// レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};
