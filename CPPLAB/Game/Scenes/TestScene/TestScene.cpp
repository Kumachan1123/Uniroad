/*
*		@file TestScene.cpp
*		@brief テストシーンクラス
*/

#include "pch.h"
#include "TestScene.h"
/*
*	@brief コンストラクタ
*	@details テストシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
TestScene::TestScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
{
}

/*
*	@brief デストラクタ
*	@details テストシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
TestScene::~TestScene()
{
}
/*
*	@brief 初期化
*	@details テストシーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void TestScene::Initialize(CommonResources* resources)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// DirectXのSimpleMath名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソースを取得
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// カメラを作成する
	CreateCamera();
	// フェードを作成する
	m_pFade = std::make_unique<Fade>();
	// フェードを初期化する
	m_pFade->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 天球モデルを作成する
	m_pSky = std::make_unique<Sky>(m_pCommonResources);
	// 天球モデルを初期化する
	m_pSky->Initialize();
	// 天球のスケールをセット
	m_pSky->SetScale(Vector3(10.f));
	// 天球の位置をセット
	m_pSky->SetPosition(Vector3(0.f, 0.f, 0.f));
	// UIテキストを作成する
	m_pUIText = std::make_unique<UIText>();
	// UIテキストを初期化する
	m_pUIText->Initialize(deviceResources);
	// 適当な文字をセット
	m_pUIText->AddString(L"使い方によっては");
	m_pUIText->AddString(L"こんなことも");
	m_pUIText->AddString(L"できちゃいます！");
	m_pUIText->AddString(L"いやぁ～便利ですねぇ～");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L"ユッケビビンバ");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L"科学の力ってすげー！");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L".........");
	m_pUIText->AddString(L"返事がない");
	m_pUIText->AddString(L"ただの屍のようだ");


	// 文字の位置をセット
	m_pUIText->SetPosition({ 100.0f, 50.0f });
	// 文字の色をセット
	m_pUIText->SetColor(Color{ 1,1,1,1 });
	// 文字のスケールをセット
	m_pUIText->SetScale(1.0f);
	// 文字のアライメントをセット
	m_pUIText->SetAlignment(TextAlignment::LEFT);

	// モデルを取得
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("World");
	// モデルにデフォルトのフォグを設定
	m_pModel->UpdateEffects([&](IEffect* effect)
							{
								auto fog = dynamic_cast<BasicEffect*>(effect);
								if (fog)
								{
									fog->SetFogEnabled(true);
									fog->SetFogStart(75.0f);
									fog->SetFogEnd(500.0f);
									fog->SetFogColor(DirectX::Colors::GhostWhite);

								}
							});


}
/*
*	@brief 更新
*	@details テストシーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TestScene::Update(float elapsedTime)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// SinpleMathの名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 時間を更新
	m_time += elapsedTime;
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// デバッグカメラを更新する
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// ビュー行列を取得
	m_view = m_debugCamera->GetViewMatrix();
	// カメラの位置を調整
	m_pFixedCamera->SetCameraDistance(CAMERA_POSITION);
	// フェードの更新
	if (m_time >= FADE_START_TIME) m_pFade->Update(elapsedTime);
	// フェードインが終わったらフェード状態をなくす
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)m_pFade->SetState(Fade::FadeState::None);
	// UIテキストを更新する
	if (m_pFade->GetState() == Fade::FadeState::None)m_pUIText->Update(elapsedTime);
	// 天球を更新する
	m_pSky->Update(elapsedTime);
	// キーボード入力を取得
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// スペースキーが押されたら（何らかのフラグがたったら）
	if (keyState.Space)
	{
		// フェードアウトに移行
		if (m_pUIText->IsFinishedAll())m_pFade->SetState(Fade::FadeState::FadeOut);
		// 次へ要求を出す
		else m_pUIText->RequestNext();
	}
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;

}
/*
*	@brief 描画
*	@details テストシーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void TestScene::Render()
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// DirectXのSimpleMath名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto commonStates = m_pCommonResources->GetCommonStates();
	// ワールド行列を設定
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(1.0f);
	// 天球の描画
	m_pSky->Render(m_view, m_projection);
	// モデルの描画
	m_pModel->Draw(context, *commonStates, world, m_view, m_projection, false, [&]()
				   {
					   // 両面描画にする
					   ID3D11RasterizerState* rasterizerState[1];
					   rasterizerState[0] = commonStates->CullNone();
					   context->RSSetState(rasterizerState[0]);

				   });


	// UIテキストを描画する
	m_pUIText->Render();
	// フェードを描画する
	m_pFade->Render();

}
/*
*	@brief 終了
*	@details テストシーンクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void TestScene::Finalize()
{
}
/*
* 	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID TestScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// ステージセレクトへ
	return IScene::SceneID::TITLE;
}
/*
*	@brief カメラに関する設定をする
*	@details カメラの作成と設定を行う
*	@param なし
*	@return なし
*/
void TestScene::CreateCamera()
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	// 固定カメラを初期化する
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	// デバッグカメラを作成する
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	// デバッグカメラを初期化する
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(FOV),// 視野角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),// アスペクト比
		0.1f, 10000.0f);// ニアクリップ距離、ファークリップ距離
	// カメラに射影行列をセット
	m_pFixedCamera->SetProjectionMatrix(m_projection);

}
