/*
*	@file Road.cpp
*	@brief 道路の描画を行うクラス
*/
#include "pch.h"
#include "Road.h"
/*
*	@brief 道路の描画を行うクラスのコンストラクタ
*	@details 道路の描画を行うクラスのコンストラクタ
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
Road::Road(CommonResources* resources)
	: m_pCommonResources(resources) // 共通リソースへのポインタ
	, m_pShadowMapLight(nullptr) // シャドウマップライトへのポインタ
	, m_pModels() // モデルへのポインタ
	, m_positions() // 道路の位置
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 道路の回転
	, m_world(DirectX::SimpleMath::Matrix::Identity) // 道路のワールド行列
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビューマトリックス
	, m_proj(DirectX::SimpleMath::Matrix::Identity) // プロジェクションマトリックス
	, m_time(0.0f) // 時間
	, m_scrollSpeed(5.0f) // スクロール速度
{
}
/*
*	@brief 道路の描画を行うクラスのデストラクタ
*	@details 道路の描画を行うクラスのデストラクタ
*	@param なし
*	@return なし
*/
Road::~Road()
{
	// 後処理を行う
	Finalize();
}
/*
*	@brief 初期化
*	@details 道路の描画を行うクラスの初期化を行い、必要なリソースを設定する。
*	@param commonResources 共通リソースへのポインタ
*	@return なし
*/
void Road::Initialize()
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// 道を作成
	for (int i = 0; i < 2; i++)
	{	// モデルを読み込む
		m_pModels.push_back(m_pCommonResources->GetModelManager()->GetModel("Road"));
		// 初期位置をリセット
		m_positions.push_back(Vector3(i * 10.0f, -0.52f, 0.0f));
	}
	// 深度ステンシルバッファを作成
	CreateDepthStencilBuffer(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
}
/*
*	@brief 更新
*	@details 道路の描画を行うクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Road::Update(float elapsedTime)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// 時間を加算
	m_time += elapsedTime;
	// スクロール
	for (int i = 0; i < 2; ++i)	m_positions[i].x -= m_scrollSpeed * elapsedTime;
	// ループ処理：手前に来たやつを奥に回す
	// 道の長さを定義
	const float roadLength = 10.0f;
	// 道路の位置を更新
	for (int i = 0; i < 2; ++i)
	{
		// 道路の位置が手前に来たら奥に回す
		if (m_positions[i].x < -roadLength)
		{
			// もう一方の道路の位置を取得
			int other = (i == 0) ? 1 : 0;
			// 手前の道路の位置を奥の道路の位置に設定
			m_positions[i].x = m_positions[other].x + roadLength;
		}
	}
}
/*
*	@brief 描画
*	@details 道路の描画を行うクラスの描画を行う
*	@param view ビューマトリックス
*	@param proj プロジェクションマトリックス
*	@return なし
*/
void Road::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// デバイスコンテキストを取得
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	const auto states = m_pCommonResources->GetCommonStates();
	// モデルを描画
	for (int i = 0; i < 2; ++i)
	{
		// ワールド行列を作成 
		auto world = DirectX::SimpleMath::Matrix::CreateTranslation(m_positions[i]);

		// モデルを描画
		m_pModels[i]->Draw(context, *states, world, view, proj, false, [&]
			{
				//// ブレンドステートを設定する
				//context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
				//// 深度ステンシルステートを設定する
				//context->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);	// 参照値：0
				//// カリングを設定する
				//context->RSSetState(states->CullCounterClockwise());
				//// テクスチャサンプラを適用する
				//ID3D11SamplerState* sampler = states->PointWrap();
				//context->PSSetSamplers(0, 1, &sampler);
				m_pShadowMapLight->ApplyShader(context, states);
			});
	}
}
/*
*	@brief 終了
*	@details 道路の描画を行うクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void Road::Finalize()
{
	// リソースを解放する
	m_pCommonResources = nullptr;
	// モデルを解放
	for (auto& model : m_pModels) if (model)model = nullptr;

}
/*
*	@brief 深度ステンシルバッファを作成する
*	@details 深度ステンシルバッファを作成する。
*	@param pDevice Direct3Dデバイスへのポインタ
*	@return なし
*/
void Road::CreateDepthStencilBuffer(ID3D11Device* pDevice)
{
	// 深度ステンシル情報を定義する
	D3D11_DEPTH_STENCIL_DESC desc = {};

	// 床：床描画時にステンシルバッファの値をインクリメントする（0を1にする）
	desc.DepthEnable = true;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する

	desc.StencilEnable = true;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff（マスク値）
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff（マスク値）

	// ポリゴンの表面の設定
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// 裏面も同じ設定
	desc.BackFace = desc.FrontFace;
	// 深度ステンシルステートを作成する
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf()));
}
