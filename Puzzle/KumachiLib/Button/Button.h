/*
*	@file Button.h
*	@brief ボタンの大元となるクラス
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
public:
	// public関数
	// コンストラクタ
	Button();
	// デストラクタ
	~Button();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height);
	// 更新
	void Update(const float elapsedTime);
	// 画像を表示
	void Render(const  Rect& buttonRect, int frameIndex, int frameCols, int frameRows);
	// 当たり判定
	bool Hit(const DirectX::SimpleMath::Vector2& mousePosition, const Rect& buttonRect);

private:
	// private定数
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	// private変数
	// 共通リソースへのポインタ
	CommonResources* m_pCommonResources;
	// 画像クラス
	std::unique_ptr<Image> m_pImage;
};