/*
*	@file		SettingScene.cpp
*	@brief		設定画面シーンクラス
*/
#include "pch.h"
#include "SettingScene.h"
/*
*	@brief		コンストラクタ
*	@details	設定画面シーンの初期化を行う
*	@param		sceneID シーンID
*	@return		なし
*/
SettingScene::SettingScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_time(0.0f) // 経過時間
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_pFade(nullptr) // フェードへのポインタ
{
}
/*
*	@brief		デストラクタ
*	@details	設定画面シーンの終了処理を行う
*	@param		なし
*	@return		なし
*/
SettingScene::~SettingScene()
{
	// 何もしない
}
/*
*	@brief		初期化
*	@details	設定画面シーンの初期化を行う
*	@param		resources 共通リソースへのポインタ
*	@return		なし
*/
void SettingScene::Initialize(CommonResources* resources)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// SimpleMathの名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 出力サイズを取得
	const RECT outputSize = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// BGMの再生
	m_pCommonResources->GetAudioManager()->PlaySound("SettingBGM", m_pCommonResources->GetSettingManager()->GetBGMVolume());
	// ボタンを作成する
	m_pBackButton = std::make_unique<BackButton>();
	// ボタンを初期化する
	m_pBackButton->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// 設定バーを作成する
	m_pSettingBar = std::make_unique<SettingBar>();
	// 設定バーを初期化する
	m_pSettingBar->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// 設定画面のパネルを作成する
	m_pSettingPanel = std::make_unique<SettingPanel>();
	// 設定画面のパネルを初期化する
	m_pSettingPanel->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// 背景画像を作成
	m_pBack = std::make_unique<Back>();
	// 背景画像を初期化
	m_pBack->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// フェードを作成する
	m_pFade = std::make_unique<Fade>();
	// フェードを初期化する
	m_pFade->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 2つ作成する
	for (int i = 0; i < 2; i++)
	{
		// Minテクスチャを作成する
		std::unique_ptr<MinTexture> pMinTexture = std::make_unique<MinTexture>();
		// 座標を設定する
		pMinTexture->SetPosition(MIN_POSITION + INTERVAL_Y * float(i));
		// Minテクスチャを初期化する
		pMinTexture->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
		// 配列に追加する
		m_pMinTextures.push_back(std::move(pMinTexture));
		// Maxテクスチャを作成する
		std::unique_ptr<MaxTexture> pMaxTexture = std::make_unique<MaxTexture>();
		// 座標を設定する
		pMaxTexture->SetPosition(MAX_POSITION + INTERVAL_Y * float(i));
		// Minテクスチャを初期化する
		pMaxTexture->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
		// 配列に追加する
		m_pMaxTextures.push_back(std::move(pMaxTexture));
	}
	// BGM画像を作成
	m_pBGMTexture = std::make_unique<BGMTexture>();
	// BGM画像を初期化
	m_pBGMTexture->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
	// SE画像を作成
	m_pSETexture = std::make_unique<SETexture>();
	// SE画像を初期化
	m_pSETexture->Initialize(m_pCommonResources, outputSize.right, outputSize.bottom);
}
/*
*	@brief		更新
*	@details	設定画面シーンの更新を行う
*	@param		elapsedTime 経過時間
*	@return		なし
*/
void SettingScene::Update(float elapsedTime)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// SimpleMathの名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 時間を更新
	m_time += elapsedTime;
	// オーディオマネージャーの更新処理
	m_pCommonResources->GetAudioManager()->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);
	// ボタンを更新
	m_pBackButton->Update(elapsedTime);
	// 設定バーを更新
	m_pSettingBar->Update(elapsedTime);
	// 設定画面のパネルを更新
	m_pSettingPanel->Update(elapsedTime);
	// 背景画像を更新
	m_pBack->Update(elapsedTime);
	// Minテクスチャを更新
	for (const auto& minTexture : m_pMinTextures)minTexture->Update(elapsedTime);
	// Maxテクスチャを更新
	for (const auto& maxTexture : m_pMaxTextures)maxTexture->Update(elapsedTime);
	// BGM画像を更新
	m_pBGMTexture->Update(elapsedTime);
	// SE画像を更新
	m_pSETexture->Update(elapsedTime);
	// フェード状態が「なし」の場合のみ更新を行う
	if (m_pFade->GetState() == Fade::FadeState::None)
	{
		// ボタンが押された場合
		if (m_pBackButton->IsPressed())
		{
			// フェードアウトに移行
			m_pFade->SetState(Fade::FadeState::FadeOut);
			// ボタンの番号を取得
			m_pBackButton->GetPressedButtonIndex();
			// BGMを止める
			m_pCommonResources->GetAudioManager()->StopSound("SettingBGM", 1.0f);
			// ボタンクリック音を鳴らす
			m_pCommonResources->GetAudioManager()->PlaySound("ButtonClick", m_pCommonResources->GetSettingManager()->GetSEVolume());
		}
	}
	// フェードインが終わったらフェード状態をなくす
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)m_pFade->SetState(Fade::FadeState::None);
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
}
/*
*	@brief		描画
*	@details	設定画面シーンの描画を行う
*	@param		なし
*	@return		なし
*/
void SettingScene::Render()
{
	// 背景画像を描画する
	m_pBack->Render();
	// ボタンを描画する
	m_pBackButton->Render();
	// 設定画面のパネルを描画する
	m_pSettingPanel->Render();
	// 設定バーを描画する
	m_pSettingBar->Render();
	// Minテクスチャを描画する
	for (const auto& minTexture : m_pMinTextures)minTexture->Render();
	// Maxテクスチャを描画する
	for (const auto& maxTexture : m_pMaxTextures)maxTexture->Render();
	// BGM画像を描画する
	m_pBGMTexture->Render();
	// SE画像を描画する
	m_pSETexture->Render();
	// フェードを描画する
	m_pFade->Render();
}
/*
*	@brief		終了
*	@details	設定画面シーンの終了処理を行う
*	@param		なし
*	@return		なし
*/
void SettingScene::Finalize()
{
	// 何もしない
}
/*
*	@brief		シーンIDを取得する
*	@details	シーンIDを取得する
*	@param		なし
*	@return		シーンID
*/
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene && m_pBackButton->GetPressedButtonIndex() == 0)
	{
		// 変更を適用
		m_pCommonResources->GetSettingManager()->SaveSettings();
		// タイトルシーンへ
		return IScene::SceneID::TITLE;
	}
	// シーン変更がない場合何もしない
	return IScene::SceneID::NONE;
}
