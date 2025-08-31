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
// UVスクロール用定数バッファ
struct UVScrollBuffer
{
	DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
	DirectX::SimpleMath::Matrix matView;    // ビュー行列
	DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
	DirectX::SimpleMath::Vector4 time;     // 時間
};
// パーティクル用定数バッファ
struct ParticleBuffer
{
	DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
	DirectX::SimpleMath::Matrix matView;    // ビュー行列
	DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
	DirectX::SimpleMath::Vector4 color;    // 色
	DirectX::SimpleMath::Vector4 count;     // カウント
	DirectX::SimpleMath::Vector4 height;    // 高さ
	DirectX::SimpleMath::Vector4 width;     // 幅
};
// 背景用定数バッファ
struct BackgroundBuffer
{
	DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
	DirectX::SimpleMath::Matrix matView;    // ビュー行列
	DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
	DirectX::SimpleMath::Vector4 colors;    // カラー
	DirectX::SimpleMath::Vector4 time;		// 時間 
};
// フェード用定数バッファ
struct FadeBuffer
{
	float smoothness = 0;                       // フェードの滑らかさ
	float fadeAmount = 0;                       // フェードの進行度（0.0～1.0）
	DirectX::SimpleMath::Vector2 padding;		// パディング
};
// ダミー頂点構造体
struct DummyVertex
{
	DirectX::SimpleMath::Vector4 position;//wは使わない
	DirectX::SimpleMath::Vector4 color;// RGBA
	DirectX::SimpleMath::Vector4 tex;//z w は使わない
};
// ライトの定数バッファ用構造体
struct cbLight
{
	DirectX::XMMATRIX lightViewProjection;	// ライトの投影空間へ座標変換する行列
	DirectX::XMVECTOR lightPosition;		// ライトの位置
	DirectX::XMVECTOR lightDirection;		// ライトの方向
	DirectX::XMVECTOR lightAmbient;			// ライトの環境光
};