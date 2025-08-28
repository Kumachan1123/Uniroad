/*
*	@file		SettingBar.cpp
*	@brief		設定画面のバークラス
*/
#include "pch.h"
#include "SettingBar.h"
// 最終的な座標の定義
const std::vector<DirectX::SimpleMath::Vector2> SettingBar::BAR_POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.6f, 0.35f), // BGMバーの位置
	DirectX::SimpleMath::Vector2(0.6f, 0.65f), // SEバーの位置

};
const std::vector<DirectX::SimpleMath::Vector2> SettingBar::CONTROLLER_POSITIONS =
{
	DirectX::SimpleMath::Vector2(0.6f, 0.35f), // BGMバーの位置
	DirectX::SimpleMath::Vector2(0.6f, 0.65f), // SEバーの位置

};
// 最終的なサイズの定義
const std::vector<DirectX::SimpleMath::Vector2> SettingBar::BAR_SIZES =
{
	DirectX::SimpleMath::Vector2(0.5f, 0.04f), // BGMバーのサイズ
	DirectX::SimpleMath::Vector2(0.5f, 0.04f), // SEバーのサイズ
};
const std::vector<DirectX::SimpleMath::Vector2> SettingBar::CONTROLLER_SIZES =
{
	DirectX::SimpleMath::Vector2(0.05f, 0.05f), // BGMバーのサイズ
	DirectX::SimpleMath::Vector2(0.05f, 0.05f), // SEバーのサイズ
};
/*
*	@brief コンストラクタ
*	@details 設定画面のバークラスのコンストラクタ
*	@param なし
*	@return なし
*/
SettingBar::SettingBar()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pBGMBar(std::make_unique<ScrollBar>()) // BGMバーへのポインタ
	, m_pSEBar(std::make_unique<ScrollBar>()) // SEバーへのポインタ
	, m_position(DirectX::SimpleMath::Vector2(0.5f, 0.5f)) // バーの位置
	, m_size(DirectX::SimpleMath::Vector2(0.2f, 0.1f)) // バーのサイズ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_isHit(false)// 当たり判定フラグ
{
}
/*
*	@brief デストラクタ
*	@details 設定画面のバークラスのデストラクタ
*	@param なし
*	@return なし
*/
SettingBar::~SettingBar()
{
}
/*
*	@brief 初期化
*	@details 設定画面のバークラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void SettingBar::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// スクロールバーの棒部分の画像を設定
	m_pBGMBar->SetControllerTexture(resources->GetTextureManager()->GetTexture("Wheel"));
	// スクロールバーの操作部分の画像を設定
	m_pBGMBar->SetBarTexture(resources->GetTextureManager()->GetTexture("ScrollBar"));
	// スクロールバーを配列に追加
	m_bars.push_back(std::move(m_pBGMBar));
	// スクロールバーの棒部分の画像を設定
	m_pSEBar->SetControllerTexture(resources->GetTextureManager()->GetTexture("Wheel"));
	// スクロールバーの操作部分の画像を設定
	m_pSEBar->SetBarTexture(resources->GetTextureManager()->GetTexture("ScrollBar"));
	// スクロールバーを配列に追加
	m_bars.push_back(std::move(m_pSEBar));
	// スクロールバーの数ループ
	for (auto& bar : m_bars)
	{
		// シェーダーパスを渡す
		bar->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
		bar->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
		// シェーダーバッファサイズを設定
		bar->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
		// スクロールバーを初期化
		bar->Initialize(resources, width, height);
	}
	// スクロールバーの矩形を設定
	for (int i = 0; i < m_bars.size(); i++)
	{
		// 矩形の定義
		Rect barRect;
		Rect controllerRect;
		// 矩形の位置を設定
		barRect.position = BAR_POSITIONS[i];
		controllerRect.position = CONTROLLER_POSITIONS[i];
		// 矩形のサイズを設定
		barRect.size = BAR_SIZES[i];
		controllerRect.size = CONTROLLER_SIZES[i];
		// 矩形を追加
		m_barRects.push_back(barRect);
		m_controllerRects.push_back(controllerRect);
		// 当たり判定フラグを初期化
		m_isHit.push_back(false);
		// 当たったX座標を初期化
		m_ratioX.push_back(0.0f);
	}
	// 1つ目のスクロールバーの当たったX座標を初期化
	m_ratioX[0] = m_bars[0]->MapRatioToRect(m_barRects[0], m_pCommonResources->GetSettingManager()->GetBGMVolume());
	// 2つ目のスクロールバーの当たったX座標を初期化
	m_ratioX[1] = m_bars[1]->MapRatioToRect(m_barRects[1], m_pCommonResources->GetSettingManager()->GetSEVolume());
}
/*
*	@brief 更新
*	@details 設定画面のバークラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void SettingBar::Update(float elapsedTime)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 1つ目のスクロールバーの当たったX座標を初期化
	m_controllerRects[0].position.x = m_bars[0]->MapRatioToRect(m_barRects[0], m_pCommonResources->GetSettingManager()->GetBGMVolume());
	// 2つ目のスクロールバーの当たったX座標を初期化
	m_controllerRects[1].position.x = m_bars[1]->MapRatioToRect(m_barRects[1], m_pCommonResources->GetSettingManager()->GetSEVolume());
	// スクロールバーの数ループ
	for (int i = 0; i < m_bars.size(); i++)
	{
		// 当たり判定を行う
		m_isHit[i] = m_bars[i]->Hit(mousePos, m_barRects[i], m_ratioX[i]);
		// マウスが当たって左クリックされたら操作部分のX座標を更新
		if (mouseState.leftButton && m_isHit[i])m_controllerRects[i].position.x = m_ratioX[i];
	}
	// スクロールバーを更新
	for (const auto& bar : m_bars)bar->Update(elapsedTime);
	// 定数バッファを更新
	UpdateConstantBuffer();
	// BGM音量調整
	m_pCommonResources->GetSettingManager()->SetBGMVolume(m_bars[0]->UnmapRectToRatio(m_barRects[0], m_controllerRects[0].position.x));
	// SE音量調整
	m_pCommonResources->GetSettingManager()->SetSEVolume(m_bars[1]->UnmapRectToRatio(m_barRects[1], m_controllerRects[1].position.x));
}
/*
*	@brief 描画
*	@details 設定画面のバークラスの描画を行う
*	@param なし
*	@return なし
*/
void SettingBar::Render()
{
	// ボタンを描画
	for (size_t i = 0; i < m_bars.size(); i++)m_bars[i]->DrawQuadWithBuffer(m_barRects[i], m_controllerRects[i], m_spriteSheetBuffer);
}
/*
*	@brief 定数バッファを更新
*	@details 定数バッファの更新を行う
*	@param なし
*	@return なし
*/
void SettingBar::UpdateConstantBuffer()
{
	// 名前空間を使用
	using namespace DirectX::SimpleMath;
	// 定数バッファを更新
	// ワールド行列を単位行列に設定
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_spriteSheetBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_spriteSheetBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_spriteSheetBuffer.count = Vector4(0.0f);
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}

