/*
*	@file		ScrollBar.cpp
*	@brief		スクロールバーの大元となるクラス
*/
#include "pch.h"
#include "ScrollBar.h"
/*
*	@brief コンストラクタ
*	@details スクロールバーの初期化を行う
*	@param なし
*	@return なし
*/
ScrollBar::ScrollBar()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pController(std::make_unique<Image>()) // 操作部分画像へのポインタ
	, m_pBar(std::make_unique<Image>()) // 棒部分画像へのポインタ
{
}
/*
*	@brief デストラクタ
*	@details スクロールバーの終了処理を行う
*	@param なし
*	@return なし
*/
ScrollBar::~ScrollBar()
{
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief 初期化
*	@details スクロールバーの初期化を行う
*	@param resources 共通リソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void ScrollBar::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 棒部分画像の作成
	m_pBar->Initialize(m_pCommonResources, width, height);
	// 操作部分画像の初期化
	m_pController->Initialize(m_pCommonResources, width, height);
}
/*
*	@brief 更新
*	@details スクロールバーの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void ScrollBar::Update(const float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief 当たり判定
*	@details マウスの座標とスクロールバーの矩形を比較して当たり判定を行う
*	@param mousePosition マウスの座標
*	@param scrollBarRect 矩形
*	@return 当たり判定の結果(true:当たった false:当たってない)
*/
bool ScrollBar::Hit(const DirectX::SimpleMath::Vector2& mousePosition,
	const Rect& scrollBarRect,
	float& outRatioX)
{
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// ウィンドウサイズ取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズを取得
	RECT rect;
	GetClientRect(hwnd, &rect);
	// ウィンドウの幅をfloat型に変換
	float winWidth = static_cast<float>(rect.right);
	// ウィンドウの高さをfloat型に変換
	float winHeight = static_cast<float>(rect.bottom);
	// マウスのX座標を0～1の正規化座標へ
	float mouseNormX = mousePosition.x / winWidth;
	// マウスのY座標を0～1の正規化座標へ
	float mouseNormY = mousePosition.y / winHeight;
	// 矩形座標系で棒部分の範囲
	// 左
	float left = scrollBarRect.position.x - scrollBarRect.size.x / 2.0f;
	// 右
	float right = scrollBarRect.position.x + scrollBarRect.size.x / 2.0f;
	// 上
	float top = scrollBarRect.position.y - scrollBarRect.size.y / 2.0f;
	// 下
	float bottom = scrollBarRect.position.y + scrollBarRect.size.y / 2.0f;
	// 矩形座標系でマウスが棒部分の矩形内にあるか
	if (left <= mouseNormX && mouseNormX <= right && top <= mouseNormY && mouseNormY <= bottom)
	{
		// 棒部分の矩形座標内でのXの割合
		outRatioX = mouseNormX;
		// 当たった
		return true;
	}
	// 当たってない
	return false;
}
/*
*	@brief 矩形に対する割合を座標に変換
*	@details 矩形に対する割合を座標に変換する
*	@param barRect 矩形
*	@param ratio01 矩形に対する割合(0.0～1.0)
*	@return 矩形に対する割合を座標に変換した値
*/
float ScrollBar::MapRatioToRect(const Rect& barRect, float ratio01)
{
	// 左
	float left = barRect.position.x - barRect.size.x / 2.0f;
	// 右
	float right = barRect.position.x + barRect.size.x / 2.0f;
	// clamp（念のため0～1範囲外のときも補正）
	float ratio = Clamp(ratio01, 0.0f, 1.0f);
	// 矩形に対する割合を座標に変換して返す
	return left + (right - left) * ratio;
}
/*
*	@brief 矩形に対する割合を座標に変換
*	@details 矩形に対する割合を座標に変換する(MapRatioToRectの逆関数)
*	@param barRect 矩形
*	@param positionX 矩形に対する割合を座標に変換した値
*	@return 矩形に対する割合(0.0～1.0)
*/

float ScrollBar::UnmapRectToRatio(const Rect& barRect, float positionX)
{
	// 左端を計算
	float left = barRect.position.x - barRect.size.x / 2.0f;
	// 右端を計算
	float right = barRect.position.x + barRect.size.x / 2.0f;
	// ゼロ割防止
	if (right == left) return 0.0f;
	// 矩形に対する割合を計算
	float ratio = (positionX - left) / (right - left);
	// clamp（念のため0～1範囲外のときも補正）
	ratio = Clamp(ratio, 0.0f, 1.0f);
	// 計算した割合を返す
	return ratio;
}
