/*
*	@file Sky.cpp
*	@brief 空の描画を行うクラス
*/
#include "pch.h"
#include "Sky.h"
/*
*	@brief コンストラクタ
*	@details 空の描画を行うクラスのコンストラクタ
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
Sky::Sky(CommonResources* resources)
	: m_pCommonResources(resources) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_position(0.0f, 40.0f, 0.0f) // 空の位置
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 空の回転
	, m_world(DirectX::SimpleMath::Matrix::Identity) // ワールド行列
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビューマトリックス
	, m_proj(DirectX::SimpleMath::Matrix::Identity) // プロジェクションマトリックス
	, m_time(0.0f)// 時間
{
}
/*
*	@brief デストラクタ
*	@details 空の描画を行うクラスのデストラクタ
*	@param なし
*	@return なし
*/
Sky::~Sky()
{
}
/*
*	@brief 初期化
*	@details 空の描画を行うクラスの初期化を行う
*	@param なし
*	@return なし
*/
void Sky::Initialize()
{

	const auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Sky");
	// シェーダを読み込むための変数
	std::vector<uint8_t> blob;
	// 頂点シェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/Sky/VS_Sky.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vs.ReleaseAndGetAddressOf())
	);
	// ライト用のピクセルシェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/Sky/PS_Sky.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);

}
/*
*	@brief 更新
*	@details 空の描画を行うクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Sky::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 時間を加算
	m_time += elapsedTime;
	// 時間経過で回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time, 0.0f, 0.0f);
	// ワールド行列を更新
	m_world = Matrix::CreateScale(0.2f) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
}
/*
*	@brief 描画
*	@details 空の描画を行う
*	@param view ビューマトリックス
*	@param proj プロジェクションマトリックス
*	@return なし
*/
void Sky::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto states = m_pCommonResources->GetCommonStates();
	m_pModel->Draw(context, *states, m_world, view, proj, false, [&]
		{
			// シェーダを設定する
			context->VSSetShader(m_vs.Get(), nullptr, 0);
			context->PSSetShader(m_ps.Get(), nullptr, 0);
		});
}


void Sky::Finalize()
{
}
