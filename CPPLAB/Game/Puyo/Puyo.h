#ifndef PUYO_H
#define PUYO_H
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Image/Image.h"

// 単体のぷよを表すクラス
// 盤面上の論理座標（行・列）と、画面上の描画座標（Vector2）の両方を保持する。
// 描画時はスプライトシートのフレーム番号として色情報を使う。
class Puyo
{
public:
	// ぷよの色定義
	// スプライトシートの列番号と対応しているため、値の順番を変更すると表示色がずれる。
	enum class PuyoColor
	{
		Red = 0,
		Green,
		Blue,
		Yellow,
		Purple,
		None	// 空マス用。Renderでは描画しない。
	};
public:
	// アクセサ
	// 現在の色を取得する。盤面判定（連結判定・空判定）で利用する。
	PuyoColor GetColor() const { return m_color; }
	// 画面上の描画座標を取得する。
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// 画面上の描画座標を設定する。
	// 描画矩形も同時に同期し、見た目と内部座標がズレないようにする。
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; m_rect.position = position; }
	// 描画サイズを取得する。
	DirectX::SimpleMath::Vector2 GetSize() const { return m_size; }
	// 描画サイズを設定する。矩形サイズも同時に同期する。
	void SetSize(const DirectX::SimpleMath::Vector2& size) { m_size = size; m_rect.size = size; }
	// 盤面上の行番号を取得する。
	int GetRow() const { return m_row; }
	// 盤面上の列番号を取得する。
	int GetCol() const { return m_col; }
	// 盤面上の行列を同時設定する。
	void SetRowCol(int row, int col) { m_row = row; m_col = col; }
	// 自由落下フラグを取得する。
	bool IsFalling() const { return m_isFalling; }
	// 自由落下フラグを設定する。
	void SetIsFalling(bool isFalling) { m_isFalling = isFalling; }
public:
	// 色を指定して生成する。
	Puyo(PuyoColor color);
	~Puyo();
	// 描画に必要なGPUリソースを初期化する。
	void Initialize(CommonResources* resources, int width, int height);
	// 内部状態を更新する（定数バッファ、必要なら簡易落下）。
	void Update(float elapsedTime);
	// 色がNoneでなければスプライトを描画する。
	void Render();
	void Finalize();
private:
	// 描画用定数バッファを最新状態に更新する。
	void UpdateConstantBuffer();
private:
	// private定数
	// 生成直後のデフォルト表示位置（左上基準）。
	static constexpr DirectX::SimpleMath::Vector2 POSITION = { 0.4f, 0.5f };
	// 1マス分の表示サイズ。
	static constexpr DirectX::SimpleMath::Vector2 SIZE = { 0.05,0.05 };
private:
	// privateメンバ変数
	// 共通リソース（デバイス・テクスチャ取得に使用）
	CommonResources* m_pCommonResources;
	// 実際の描画を担当する画像オブジェクト
	std::unique_ptr<Image> m_pImage;
	// 描画時に使用する矩形（位置・サイズ）
	Rect m_rect;
	// シェーダへ渡す定数バッファ
	SpriteSheetBuffer m_spriteSheetBuffer;
	// スプライトシートの行数
	int m_frameRows;
	// スプライトシートの列数
	int m_frameCols;
	// 画面上の描画位置
	DirectX::SimpleMath::Vector2 m_position;
	// 画面上の描画サイズ
	DirectX::SimpleMath::Vector2 m_size;
	// ぷよの色
	Puyo::PuyoColor m_color;
	// 盤面上の行番号
	int m_row;
	// 盤面上の列番号
	int m_col;
	// 自由落下中かどうか
	bool m_isFalling;
	// 落下用タイマー
	float m_time;
};
#endif // PUYO_H
