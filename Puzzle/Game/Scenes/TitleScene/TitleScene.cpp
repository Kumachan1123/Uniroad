/*
*	@file TitleScene.cpp
*	@brief タイトルシーンクラス
*/
#include "pch.h"
#include "TitleScene.h"
/*
*	@brief コンストラクタ
*	@details タイトルシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
TitleScene::TitleScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
{
}
/*
*	@brief デストラクタ
*	@details タイトルシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
TitleScene::~TitleScene()
{
	// 何もしない
}
/*
*	@brief 初期化
*	@details タイトルシーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void TitleScene::Initialize(CommonResources* resources)
{
	// 名前空間のエイリアス
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソースを取得
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// ロゴを作成する
	m_pTitleLogo = std::make_unique<TitleLogo>();
	// ロゴを初期化する
	m_pTitleLogo->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// カメラを作成する
	CreateCamera();
}
/*
*	@brief 更新処理
*	@details タイトルシーンの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	// ロゴを更新
	m_pTitleLogo->Update(elapsedTime);
}
/*
*	@brief 描画処理
*	@details タイトルシーンの描画処理を行う
*	@param なし
*	@return なし
*/
void TitleScene::Render()
{
	// ロゴを描画する
	m_pTitleLogo->Render();
}
/*
*	@brief シーンIDを取得する
*	@details タイトルシーンの次のシーンIDを取得する
*	@param なし
*	@return 次のシーンID
*/
void TitleScene::Finalize()
{
}

/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// ゲームオーバーシーンへ
		return IScene::SceneID::STAGESELECT;
	}
	// シーン変更がない場合何もしない
	return IScene::SceneID::NONE;
}

/*
*	@brief カメラを作成する
*	@details タイトルシーン用のカメラを作成する
*	@param なし
*	@return なし
*/
void TitleScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
