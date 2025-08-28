/*
*	@file ScrollBar.h
*	@brief スクロールバーの大元となるオブジェクトのヘッダーファイル
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <cassert>
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"

// 前方宣言
class CommonResources;

// スクロールバーの大元となるオブジェクトのクラス
class ScrollBar
{
public:
	// アクセサ
	// 頂点シェーダーのファイルパスを設定
	void SetVertexShaderFilePath(std::string filePath)
	{
		m_pController->SetVertexShaderFilePath(filePath);
		m_pBar->SetVertexShaderFilePath(filePath);
	}
	// ピクセルシェーダーのファイルパスを設定
	void SetPixelShaderFilePath(std::string filePath)
	{
		m_pController->SetPixelShaderFilePath(filePath);
		m_pBar->SetPixelShaderFilePath(filePath);
	}
	// 操作部分テクスチャの設定
	void SetControllerTexture(ID3D11ShaderResourceView* pTexture) { m_pController->SetTexture(pTexture); }
	// 棒部分テクスチャの設定
	void SetBarTexture(ID3D11ShaderResourceView* pTexture) { m_pBar->SetTexture(pTexture); }
	// シェーダーバッファサイズを設定
	void SetShaderBufferSize(int size)
	{
		m_pController->SetShaderBufferSize(size);
		m_pBar->SetShaderBufferSize(size);
	}
public:
	// public関数
	// コンストラクタ
	ScrollBar();
	// デストラクタ
	~ScrollBar();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height);
	// 更新
	void Update(const float elapsedTime);
	// 画像を表示
	template<typename T>
	void DrawQuadWithBuffer(const Rect& barRect, const Rect& controllerRect, const T& buffer);
	// 当たり判定
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& scrollBarRect, float& outRatioX);
	// 渡された値を矩形サイズに合わせる
	float MapRatioToRect(const Rect& barRect, float ratio01);
	// 渡された値を0~1の範囲に収める
	float UnmapRectToRatio(const Rect& barRect, float positionX);
private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 操作部分画像クラス
	std::unique_ptr<Image> m_pController;
	// 棒画像クラス
	std::unique_ptr<Image> m_pBar;

};
/*
*	@brief 描画
*	@details スクロールバーの描画を行う
*	@param barRect 棒部分の矩形
*	@param controllerRect 操作部分の矩形
*	@param buffer 描画に使用するバッファ
*	@return なし
*/
template<typename T>
inline void ScrollBar::DrawQuadWithBuffer(const Rect& barRect, const Rect& controllerRect, const T& buffer)
{
	// 棒部分画像を描画
	m_pBar->DrawQuadWithBuffer(barRect, buffer);
	// 操作部分画像を描画
	m_pController->DrawQuadWithBuffer(controllerRect, buffer);
}
