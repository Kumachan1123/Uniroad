#include <pch.h>
#include "CreateShader.h"
#include <utility>
using namespace KumachiLib;

std::unique_ptr<CreateShader> CreateShader::m_instance = nullptr;

CreateShader* const CreateShader::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new CreateShader());
	}
	return m_instance.get();
}



CreateShader::CreateShader()
	: m_device(nullptr)
	, m_pIDE(nullptr)
	, m_NumElements(0)
	, m_pInputLayout(nullptr)
{
}

CreateShader::~CreateShader()
{
	// インプットレイアウトの解放
	m_pInputLayout.Reset();
	m_pIDE = nullptr;

}

// 初期化
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	m_device = device;
	m_pIDE = pIDE;
	m_NumElements = NumElements;
	m_pInputLayout = InputLayout;
}

// 頂点シェーダーを作成
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	BinaryFile VS = BinaryFile::LoadFile(fileName);
	//	頂点シェーダ作成
	if (FAILED(m_device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	m_device->CreateInputLayout(&m_pIDE[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());
}



// ピクセルシェーダーを作成
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	BinaryFile PS = BinaryFile::LoadFile(fileName);
	//	ピクセルシェーダ作成
	if (FAILED(m_device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

// ジオメトリシェーダーを作成
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	BinaryFile GS = BinaryFile::LoadFile(fileName);
	//	ジオメトリシェーダ作成
	if (FAILED(m_device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

}
//	シェーダーにデータを渡すためのコンスタントバッファ生成
void CreateShader::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	m_device->CreateBuffer(&bd, nullptr, &cBuffer);

}
