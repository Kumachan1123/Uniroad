#pragma once
// / <summary>
// / シェーダーを作成するクラス
// / </summary>
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include <memory>

class CreateShader
{
public:
	// シングルトンインスタンス
	static CreateShader* const GetInstance();


public:// 関数

	// デストラクタ
	~CreateShader();
	// 初期化
	void Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout);
	// 頂点シェーダを作成
	void CreateVertexShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs);
	// ピクセルシェーダーを作成
	void CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);
	// ジオメトリシェーダーを作成
	void CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs);
	// 作ったインプットレイアウトを返す
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() { return m_pInputLayout; }
	// コンスタントバッファを作成
	void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize);
private:
	// コンストラクタ
	CreateShader();
	// コピーコンストラクタと代入演算子の禁止
	CreateShader(const CreateShader&) = delete;
	CreateShader& operator=(const CreateShader&) = delete;
	// シングルトンインスタンス
	static std::unique_ptr<CreateShader> m_instance;
	// デバイス
	ID3D11Device1* m_device;
	// 入力レイアウト
	const D3D11_INPUT_ELEMENT_DESC* m_pIDE;
	// 要素数
	UINT m_NumElements;
	// レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};



