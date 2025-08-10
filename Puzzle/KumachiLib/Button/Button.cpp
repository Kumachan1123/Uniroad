/*
*	@file Button.cpp
*	@brief ボタンの大元となるクラス
*/
#include "pch.h"
#include "Button.h"
/*
*	@brief コンストラクタ
*	@details ボタンの初期化を行う
*	@param なし
*	@return なし
*/
Button::Button()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pImage(std::make_unique<Image>()) // 画像へのポインタ
{
}
/*
*	@brief デストラクタ
*	@details ボタンの終了処理を行う
*	@param なし
*	@return なし
*/
Button::~Button()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details ボタンの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Button::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 画像の初期化
	m_pImage->Initialize(m_pCommonResources, width, height);
}
/*
*	@brief 更新
*	@details ボタンの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void Button::Update(const float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief 画像を表示
*	@details ボタンの画像を表示する
*	@param buttonRect ボタンの矩形
*	@param frameIndex アニメーションのコマ番号
*	@param frameCols 画像の列数
*	@param frameRows 画像の行数
*	@return なし
*/
void Button::Render(const Rect& buttonRect, int frameIndex, int frameCols, int frameRows)
{
	// 画像を描画
	m_pImage->DrawQuad(buttonRect, frameIndex, frameCols, frameRows);

}
/*
*	@brief 当たり判定
*	@details マウスの座標とボタンの矩形を比較して当たり判定を行う
*	@param mousePosition マウスの座標
*	@param buttonRect ボタンの矩形
*	@return 当たり判定があればtrue、なければfalse
*/
bool Button::Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& buttonRect)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// 左上・右下をスクリーン座標へ変換
	// アスペクト比を考慮してY方向サイズを補正
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	// アスペクト比を考慮してY方向サイズを補正
	float aspect = static_cast<float>(rect.right) / static_cast<float>(rect.bottom);
	float correctedHeight = buttonRect.size.y * aspect;
	// 中心座標を左上・右下座標へ変換
	float left = buttonRect.position.x - buttonRect.size.x / 2.0f;
	float top = buttonRect.position.y - correctedHeight / 2.0f;
	float right = left + buttonRect.size.x;
	float bottom = top + correctedHeight;
	// スクリーン座標へ変換
	Vector2 leftTop = Vector2(left * rect.right, top * rect.bottom);
	Vector2 rightBottom = Vector2(right * rect.right, bottom * rect.bottom);
	// マウスの座標が画像の範囲内にあるならtrueを返す
	if (leftTop.x <= mousePosition.x && mousePosition.x <= rightBottom.x && leftTop.y <= mousePosition.y && mousePosition.y <= rightBottom.y)
		return true;
	// 当たり判定なしならfalseを返す
	return false;
}
