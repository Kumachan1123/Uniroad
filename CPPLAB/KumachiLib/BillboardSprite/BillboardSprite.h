/*
*	@brief	: BillboardSprite
*	@details	: 3D空間に配置された2D画像を描画するクラス
*/
#pragma once
#include <Game\GameObject\IGameObject.h>
#include <KumachiLib\DrawPolygon\DrawPolygon.h>

// 前方宣言
class CreateShader;
struct DrawPolygon::Shaders;
class BillboardSprite : public IGameObject
{
public:
	// 構造体
	// シェーダーに渡す定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ワールド行列
		DirectX::SimpleMath::Matrix matView;    // ビュー行列
		DirectX::SimpleMath::Matrix matProj;    // プロジェクション行列
		DirectX::SimpleMath::Vector4 count;     // カウント
		DirectX::SimpleMath::Vector4 height;    // 高さ
		DirectX::SimpleMath::Vector4 width;     // 幅
	};
public:
	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// テクスチャをセット
	void SetTexture(ID3D11ShaderResourceView* textures) { m_pTextures = textures; }
	// 位置をセット
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// スケールをセット
	void SetScale(float scale) { m_scale = scale; }
	// ビルボード機能を有効/無効にセット
	void SetBillboard(bool enable) { m_isBillboard = enable; }
	// 向きを変える
	void SetDirection(int direction) { m_direction = direction; }
public:
	BillboardSprite(IGameObject* owner);
	~BillboardSprite();
	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;
private:
	// 定数
	// 入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 板ポリゴンの頂点座標
	static const float m_vertexMinX;//左
	static const float m_vertexMaxX;//右
	static const float m_vertexMinY;//下
	static const float m_vertexMaxY;//上
private:
	// privateメンバ変数
	// このオブジェクトを持つクラス
	IGameObject* m_pOwner;
	// エフェクトを再生する座標
	DirectX::SimpleMath::Vector3 m_position;
	// エフェクトのスケール
	float m_scale;
	// 描画クラス
	DrawPolygon* m_pDrawPolygon;
	// シェーダー作成クラス
	CreateShader* m_pCreateShader;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// シェーダーの構造体
	DrawPolygon::Shaders m_shaders;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	// コンスタントバッファ
	ConstBuffer m_constBuffer;
	// テクスチャハンドル 
	ID3D11ShaderResourceView* m_pTextures;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	//	プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// フレームの頂点情報
	DirectX::DX11::VertexPositionTexture m_vertices[4];
	// フレーム数
	int m_anim;
	// 向き番号
	int m_direction;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーションの経過時間
	float m_animTime;
	// フレームの行数と列数
	int m_frameRows;//行
	int m_frameCols;//列
	// ビルボード機能のON/OFFフラグ
	bool m_isBillboard;
	// アニメ切替フラグ
	bool m_animSwitch;
};
