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
		m_positions.push_back(Vector3(i * 10.0f, -0.5f, 0.0f));
	}
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
		m_pModels[i]->Draw(context, *states, world, view, proj, false);
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
