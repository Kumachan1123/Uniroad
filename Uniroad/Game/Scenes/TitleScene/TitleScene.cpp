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
	, m_pMiniCharacterBase(nullptr) // ミニキャラベースへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
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
	// シャドウマップライトを作成する
	m_pShadowMapLight = std::make_unique<ShadowMapLight>(m_pCommonResources);
	// 輪郭線描画クラスを作成する
	m_pOutLine = std::make_unique<OutLine>(m_pCommonResources);
	// 視野角を輪郭描画クラスに設定
	m_pOutLine->SetFovTheta(XMConvertToRadians(FOV));
	// 空を作成する
	m_pSky = std::make_unique<Sky>(m_pCommonResources);
	// 空を初期化する
	m_pSky->Initialize();
	// 空のスケールを設定
	m_pSky->SetScale(SKY_SCALE);
	// 空の回転速度を設定
	m_pSky->SetRotationSpeed(SKY_ROTATION_SPEED);
	// 道路を作成する
	m_pRoad = std::make_unique<Road>(m_pCommonResources);
	// 道路にシャドウマップライトを設定する
	m_pRoad->SetShadowMapLight(m_pShadowMapLight.get());
	// 道路を初期化する
	m_pRoad->Initialize();
	// ロゴを作成する
	m_pTitleLogo = std::make_unique<TitleLogo>();
	// ロゴを初期化する
	m_pTitleLogo->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// ボタンを作成する
	m_pTitleButton = std::make_unique<TitleButton>();
	// ボタンを初期化する
	m_pTitleButton->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードを作成する
	m_pFade = std::make_unique<Fade>();
	// フェードを初期化する
	m_pFade->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3::Zero, 0.0f);
	// ミニキャラにカメラを設定する
	m_pMiniCharacterBase->SetCamera(m_pFixedCamera.get());
	// ミニキャラのベースにシャドウマップライトを設定する
	m_pMiniCharacterBase->SetShadowMapLight(m_pShadowMapLight.get());
	// ミニキャラのベースに輪郭線描画を設定する
	m_pMiniCharacterBase->SetOutLine(m_pOutLine.get());
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterTitle>(m_pMiniCharacterBase.get(), SHEEP_START_POS, 0.0f));
	// ミニキャラのアニメーションステートを設定
	m_pMiniCharacterBase->SetTitleAnimationState(NONE);
	// BGMの再生
	m_pCommonResources->GetAudioManager()->PlaySound("TitleBGM", m_pCommonResources->GetSettingManager()->GetBGMVolume());

}
/*
*	@brief 更新処理
*	@details タイトルシーンの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleScene::Update(float elapsedTime)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// SinpleMathの名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 時間を更新
	m_time += elapsedTime;
	// オーディオマネージャーの更新処理
	m_pCommonResources->GetAudioManager()->Update(elapsedTime);
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// デバッグカメラを更新する
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	// カメラの位置を調整
	m_pFixedCamera->SetCameraDistance(CAMERA_POSITION);
	// ミニキャラのカメラ位置を取得
	Vector3 targetPos = m_pMiniCharacterBase->GetCameraPosition();
	// カメラのターゲット位置をミニキャラのカメラ位置に設定
	m_pFixedCamera->SetTargetPosition(targetPos + CAMERA_LOOK_OFFSET);
	// カメラの座標を更新
	m_pFixedCamera->SetEyePosition(m_pMiniCharacterBase->GetCameraPosition() + m_pFixedCamera->GetCameraDistance());
	// ロゴを更新
	m_pTitleLogo->Update(elapsedTime);
	// ボタンを更新
	m_pTitleButton->Update(elapsedTime);
	// フェードの更新
	if (m_time >= FADE_START_TIME) m_pFade->Update(elapsedTime);
	// 空の更新
	m_pSky->Update(elapsedTime);
	// 道路の更新
	m_pRoad->Update(elapsedTime);
	// シャドウマップライトを更新
	m_pShadowMapLight->Update(elapsedTime);
	// シャドウマップライトの位置をミニキャラの位置に設定
	m_pShadowMapLight->SetLightPosition(targetPos + SHADOW_MAP_LIGHT_POSITION);
	// 座標を初期化
	Vector3 position(Vector3::Zero);
	// Y軸に90°回転
	Quaternion angle(Quaternion::CreateFromAxisAngle(Vector3::UnitY, XMConvertToRadians(90.0f)));
	// ミニキャラの更新
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	// フェードインが終わったら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// タイトルアニメーションを開始
		m_pMiniCharacterBase->SetTitleAnimationState(START);
		// フェード状態をなくす
		m_pFade->SetState(Fade::FadeState::None);
	}
	// ボタンが押された場合
	if (m_pTitleButton->IsPressed())
	{
		// ミニキャラのタイトルアニメーションを終了状態にする
		m_pMiniCharacterBase->SetTitleAnimationState(CONTINUE);
		// 押されたかをリセット
		m_pTitleButton->SetPressed(false);
		// ロゴを再稼働する
		m_pTitleLogo->AdvanceAnimation();
		// BGMの停止
		m_pCommonResources->GetAudioManager()->StopSound("TitleBGM", 1.0f);
		// ボタンクリック音再生
		m_pCommonResources->GetAudioManager()->PlaySound("ButtonClick", m_pCommonResources->GetSettingManager()->GetSEVolume());
	}
	// ミニキャラのタイトルアニメーションが終了状態なら
	if (m_pMiniCharacterBase->GetTitleAnimationState() == END)
	{
		// フェードアウトに移行
		m_pFade->SetState(Fade::FadeState::FadeOut);
		// ミニキャラのタイトルアニメーションをリセット
		m_pMiniCharacterBase->SetTitleAnimationState(NONE);
	}
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
}
/*
*	@brief 描画処理
*	@details タイトルシーンの描画処理を行う
*	@param なし
*	@return なし
*/
void TitleScene::Render()
{
	// 空を描画する
	m_pSky->Render(m_view, m_projection);
	// 道路を描画する
	m_pRoad->Render(m_view, m_projection);
	// シャドウマップライトをレンダリングする
	m_pShadowMapLight->RenderShadow();
	// 輪郭線描画を開始する
	m_pOutLine->RenderOutLine(m_view, m_projection);
	// ミニキャラの描画
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// 以下、2D描画のものを描画する
	// ボタンを描画する
	m_pTitleButton->Render();
	// フェードを描画する
	m_pFade->Render();
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
	// ミニキャラの終了
	if (m_pMiniCharacterBase) m_pMiniCharacterBase->Finalize();
}

/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// 押されたボタンで分岐
	switch (m_pTitleButton->GetPressedButtonIndex())
	{
	case 0: // ゲーム開始ボタンが押された場合
		// ステージセレクトへ
		return IScene::SceneID::STAGESELECT;
		// 処理を終える
		break;
	case 1: // 設定メニューボタンが押された場合
		// 設定メニューへ
		return IScene::SceneID::SETTING;
		// 処理を終える
		break;
	case 2: // ゲーム終了ボタンが押された場合
		// アプリケーションを終了する
		PostQuitMessage(0);
		// 処理を終える
		break;
	}
	// ステージセレクトへ
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
		0.1f, 100.0f);// ニアクリップ距離、ファークリップ距離
	// カメラに射影行列をセット
	m_pFixedCamera->SetProjectionMatrix(m_projection);

}

