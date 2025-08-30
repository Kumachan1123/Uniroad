/*
*	@file CountDown.cpp
*	@brief カウントダウンクラス
*/
#include "pch.h"
#include "CountDown.h"

/*
*	@brief コンストラクタ
*	@details カウントダウンクラスのコンストラクタ
*	@param なし
*	@return なし
*/
CountDown::CountDown()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
	, m_position(POSITION) // ロゴの位置
	, m_size(SIZE) // ロゴのサイズ
	, m_frameRows(4) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_time(0.0f) // 時間
	, m_prevFrame(-1) // 前のフレーム
	, m_onFrameChanged(nullptr) // フレーム変更時のコールバック
{
}
/*
*	@brief デストラクタ
*	@details カウントダウンクラスのデストラクタ
*	@param なし
*	@return なし
*/
CountDown::~CountDown()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details カウントダウンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void CountDown::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースをセット
	m_pCommonResources = resources;
	// 画像を作成
	m_pImage = std::make_unique<Image>();
	// 頂点シェーダーのパスを渡す
	m_pImage->SetVertexShaderFilePath("Resources/Shaders/Counter/VS_Counter.cso");
	// ピクセルシェーダーのパスを渡す
	m_pImage->SetPixelShaderFilePath("Resources/Shaders/Counter/PS_Counter.cso");
	// 画像を設定
	m_pImage->SetTexture(resources->GetTextureManager()->GetTexture("CountDown"));
	// シェーダーバッファサイズを設定
	m_pImage->SetShaderBufferSize(sizeof(SpriteSheetBuffer));
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
	// 矩形を設定
	m_rect.position = POSITION;
	m_rect.size = SIZE;
	// コマが変わった時にやりたい演出を追加
	SetOnFrameChanged([this](int frame)
		{
			// 効果音を再生
			// 3,2,1の時は"Count321"を再生
			if (frame < 3) m_pCommonResources->GetAudioManager()->PlaySound("Count321", m_pCommonResources->GetSettingManager()->GetSEVolume());
			// 0の時は"CountEnd"を再生
			else if (frame == 3)m_pCommonResources->GetAudioManager()->PlaySound("CountEnd", m_pCommonResources->GetSettingManager()->GetSEVolume());
			// それ以外は何もしない
			else return;
		});
}
/*
*	@brief 更新
*	@details カウントダウンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void CountDown::Update(float elapsedTime)
{
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// 一定時間経過したら更新しない
	if (m_time > END_TIME)return;
	// 時間を更新
	m_time += elapsedTime;
	// 現在のフレーム番号計算
	int currentFrame = static_cast<int>(m_time);
	// フレームが変わったら
	if (currentFrame != m_prevFrame)
	{
		// 今のフレームを保存
		m_prevFrame = currentFrame;
		// コールバック呼び出し
		if (m_onFrameChanged)m_onFrameChanged(currentFrame);
	}
	// 定数バッファを更新
	UpdateConstantBuffer();
}
/*
*	@brief 画像を表示
*	@details カウントダウンクラスの画像を表示する
*	@param なし
*	@return なし
*/
void CountDown::Render()
{
	// 一定時間以上経過したら描画しない
	if (m_time > END_TIME)return;
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(m_rect, m_spriteSheetBuffer);
}
/*
*	@brief 定数バッファを更新
*	@details カウントダウンクラスの定数バッファを更新する
*	@param なし
*	@return なし
*/
void CountDown::UpdateConstantBuffer()
{
	// SimpleMathの名前空間を使用
	using namespace DirectX::SimpleMath;
	// 定数バッファを更新
	// ワールド行列を単位行列に設定
	m_spriteSheetBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_spriteSheetBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_spriteSheetBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_spriteSheetBuffer.count = Vector4(m_time);
	// 高さを設定
	m_spriteSheetBuffer.height = Vector4((float)(m_frameRows));
	// 幅を設定
	m_spriteSheetBuffer.width = Vector4((float)(m_frameCols));
}

