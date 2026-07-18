/*
*	@file Button.h
*	@brief ボタンの大元となるクラス
*/
#pragma once
// 自作ヘッダーファイル
#include "KumachiLib/Image/Image.h"
#include "KumachiLib/Rect/Rect.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "KumachiLib/MouseClick/MouseClick.h"

// 前方宣言
namespace DX
{
	class DeviceResources;
}
// ボタンの大元となるクラス
class Button
{
public:
	// アクセサ
	// 頂点シェーダーのファイルパスを設定
	void SetVertexShaderFilePath(std::string filePath) { m_pImage->SetVertexShaderFilePath(filePath); }
	// ピクセルシェーダーのファイルパスを設定
	void SetPixelShaderFilePath(std::string filePath) { m_pImage->SetPixelShaderFilePath(filePath); }
	// テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pImage->SetTexture(pTexture); }
	// シェーダーバッファサイズを設定
	void SetShaderBufferSize(int size) { m_pImage->SetShaderBufferSize(size); }
public:
	// public関数
	// コンストラクタ
	Button();
	// デストラクタ
	~Button();
	// 初期化
	void Initialize(int width, int height);
	// 更新
	void Update(const float elapsedTime);
	// 画像を表示
	template<typename T>
	void DrawQuadWithBuffer(const Rect& rect, const T& buffer);
	// 当たり判定
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& buttonRect);

private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private変数
	// 画像クラス
	std::unique_ptr<Image> m_pImage;
};

/*
*	@brief 描画
*	@details ボタンの描画を行う
*	@param rect ボタンの矩形
*	@param buffer 描画に使用するバッファ
*	@return なし
*/
template<typename T>
inline void Button::DrawQuadWithBuffer(const Rect& rect, const T& buffer)
{
	// 画像を描画
	m_pImage->DrawQuadWithBuffer(rect, buffer);
}
