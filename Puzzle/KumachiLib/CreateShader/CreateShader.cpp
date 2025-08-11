/*
*	@file CreateShaders.cpp
*	@brief シェーダーを作成するシングルトンクラス
*	@details 板ポリゴンを描画する際に使用するシェーダーを作成する
*/
#include <pch.h>
#include "CreateShader.h"
using namespace KumachiLib;
// シングルトンインスタンスの初期化
std::unique_ptr<CreateShader> CreateShader::m_pInstance = nullptr;
/*
*	@brief シングルトンインスタンスを取得
*	@details CreateShaderクラスのシングルトンインスタンスを取得する
*	@param なし
*	@return シングルトンインスタンス
*/
CreateShader* const CreateShader::GetInstance()
{
	// インスタンスがない場合
	if (m_pInstance == nullptr)
	{
		// インスタンスを生成
		m_pInstance.reset(new CreateShader());
	}
	// インスタンスを返す
	return m_pInstance.get();
}

/*
*	@brief コンストラクタ
*	@details シェーダーを作成するための初期化を行う
*	@param なし
*	@return なし
*/
CreateShader::CreateShader()
	: m_pDevice(nullptr)// デバイス
	, m_pIED(nullptr)// インプットエレメントディスクリプタ
	, m_NumElements(0)// 要素数
	, m_pInputLayout(nullptr)// レイアウト
{
}
/*
*	@brief デストラクタ
*	@details シェーダーを作成するための初期化を行う
*	@param なし
*	@return なし
*/
CreateShader::~CreateShader()
{
	// インプットレイアウトの解放
	m_pInputLayout.Reset();
	// インプットエレメントディスクリプタの解放
	m_pIED = nullptr;
	// デバイスの解放
	m_pDevice = nullptr;
}
/*
*	@brief 初期化
*	@details シェーダーを作成するための初期化を行う
*	@param device デバイス
*	@param pIED  インプットエレメントディスクリプタ
*	@param NumElements 要素数
*	@param InputLayout レイアウト
*	@return なし
*/
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIED, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	// デバイスを設定
	m_pDevice = device;
	// インプットエレメントディスクリプタを設定
	m_pIED = pIED;
	// 要素数を設定
	m_NumElements = NumElements;
	// インプットレイアウト
	m_pInputLayout = InputLayout;
}
/*
*	@brief 初期化（デバイスのみ）
*	@details シェーダーを作成するための初期化を行う
*	@param device デバイス
*	@return なし
*/
void CreateShader::Initialize(ID3D11Device1* device)
{
	// デバイスを設定
	m_pDevice = device;
}

/*
*	@brief 頂点シェーダーを作成
*	@details 頂点シェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param vs 頂点シェーダーの格納先
* 	@return なし
*/
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	// バイナリファイルを読み込む
	BinaryFile VS = BinaryFile::LoadFile(fileName);
	//シェーダーを作成
	if (FAILED(m_pDevice->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))
	{
		// エラー処理
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		// 終了
		return;
	}
	// 入力レイアウトを作成
	m_pDevice->CreateInputLayout(&m_pIED[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());
}
/*
*	@brief ピクセルシェーダーを作成
*	@details ピクセルシェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param ps ピクセルシェーダーの格納先
*	@return なし
*/
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	// バイナリファイルを読み込む
	BinaryFile PS = BinaryFile::LoadFile(fileName);
	// ピクセルシェーダ作成
	if (FAILED(m_pDevice->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))
	{
		// エラー処理
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		// 終了
		return;
	}
}
/*
*	@brief ジオメトリシェーダーを作成
*	@details ジオメトリシェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param gs ジオメトリシェーダーの格納先
*	@return なし
*/
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	// バイナリファイルを読み込む
	BinaryFile GS = BinaryFile::LoadFile(fileName);
	// ジオメトリシェーダ作成
	if (FAILED(m_pDevice->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))
	{
		// エラー処理
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		// 終了
		return;
	}
}
/*
*	@brief コンスタントバッファを作成
*	@details コンスタントバッファを作成する
*	@param cBuffer コンスタントバッファの格納先
*	@param bufferSize バッファのサイズ
*	@return なし
*/
void CreateShader::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize)
{
	// バッファの説明
	D3D11_BUFFER_DESC bd;
	// バッファの説明を初期化
	ZeroMemory(&bd, sizeof(bd));
	// 使用法
	bd.Usage = D3D11_USAGE_DEFAULT;
	// バッファのサイズ
	bd.ByteWidth = bufferSize;
	// バインドフラグ
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPUアクセスフラグ
	bd.CPUAccessFlags = 0;
	// バッファを作成
	m_pDevice->CreateBuffer(&bd, nullptr, &cBuffer);
}

