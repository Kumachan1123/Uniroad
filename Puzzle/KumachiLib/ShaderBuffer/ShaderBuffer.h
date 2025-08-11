/*
*	@file ShaderBuffer.h
*	@brief シェーダーバッファの構造体をまとめたヘッダーファイル
*/
#pragma once

// 構造体
// スプライトシート用定数バッファ
struct SpriteSheetBuffer
{
	DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
	DirectX::SimpleMath::Matrix matView;    // ビュー行列
	DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
	DirectX::SimpleMath::Vector4 count;     // カウント
	DirectX::SimpleMath::Vector4 height;    // 高さ
	DirectX::SimpleMath::Vector4 width;     // 幅
};
// フェード用定数バッファ
struct FadeBuffer
{
	float smoothness = 0;                       // フェードの滑らかさ
	float fadeAmount = 0;                       // フェードの進行度（0.0～1.0）
	DirectX::SimpleMath::Vector2 padding;		// パディング
};