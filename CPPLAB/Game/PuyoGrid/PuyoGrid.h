#ifndef PUYO_GRID_H
#define PUYO_GRID_H
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"

class PuyoGrid
{

public:
	// アクセサ
	// ぷよの座標を取得する
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// ぷよの座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	// ぷよのサイズを取得する
	DirectX::SimpleMath::Vector2 GetSize() const { return m_size; }
	// ぷよのサイズを設定する
	void SetSize(const DirectX::SimpleMath::Vector2& size) { m_size = size; }
public:
	PuyoGrid();
	~PuyoGrid();
	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	void UpdateConstantBuffer();
private:
	// private定数
	// 表示位置（左上）
	static constexpr DirectX::SimpleMath::Vector2 POSITION = { 0.4f, 0.5f };
	// 表示サイズ
	static constexpr DirectX::SimpleMath::Vector2 SIZE = { 0.05,0.05 };
private:
	// privateメンバ変数
// 共通リソース
	CommonResources* m_pCommonResources;
	// 画像
	std::unique_ptr<Image> m_pImage;
	// ロゴの矩形
	Rect m_rect;
	// 定数バッファ
	SpriteSheetBuffer m_spriteSheetBuffer;
	// 画像の行数
	int m_frameRows;
	// 画像の列数
	int m_frameCols;
	// ロゴの位置
	DirectX::SimpleMath::Vector2 m_position;
	// ロゴのサイズ
	DirectX::SimpleMath::Vector2 m_size;
};
#endif // PUYO_GRID_H
