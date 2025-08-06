/*
*	@file ResultAnimation.cpp
*	@brief 結果アニメーションクラス
*/
#include "pch.h"
#include "ResultAnimation.h"
/*
*	@brief	インプットレイアウト
*	@return なし
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  ResultAnimation::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*	@brief コンストラクタ
*	@details 結果アニメーションクラスのコンストラクタ
*	@param なし
*	@return なし
*/
ResultAnimation::ResultAnimation()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_gameover(false) // ゲームオーバーフラグ
	, m_gameclear(false) // ゲームクリアフラグ
	, m_animationEnable(false) // アニメーション有効フラグ
	, m_animationEnd(false) // アニメーション終了フラグ
	, m_decideTexture(true) // 画像決定フラグ
	, m_frameRows(1) // 画像の行数
	, m_frameCols(1) // 画像の列数
	, m_pCreateShader(CreateShader::GetInstance()) // シェーダー作成クラス
	, m_pDrawPolygon(DrawPolygon::GetInstance()) // 板ポリゴン描画クラス
	, m_time(0.0f) // 時間
	, m_vertices{}// 頂点配列
	, m_pTextures{} // テクスチャ配列
{
}
/*
*	@brief デストラクタ
*	@details 結果アニメーションクラスのデストラクタ
*	@param なし
*	@return なし
*/
ResultAnimation::~ResultAnimation()
{
}
/*
*	@brief 初期化
*	@details 結果アニメーションクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void ResultAnimation::Initialize(CommonResources* resources)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// 描画クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダー作成
	CreateShaders();
}
/*
*	@brief 更新
*	@details 結果アニメーションクラスの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void ResultAnimation::Update(float elapsedTime)
{
	// アニメーションが終了している場合は何もしない
	if (m_animationEnd) return;
	// 時間を加算
	m_time += elapsedTime;
	// 各画像のY座標を更新
	for (int i = 0; i < m_frameCols; ++i)m_positionsY[i] = m_gameclear ? POS_Y * Easing::EaseOutExpo(m_time) : POS_Y * Easing::RandomJitter(m_time);
	// 5秒経ったらアニメーションを終了
	if (m_time >= 3.0f)m_animationEnd = true;
}
/*
*	@brief 描画
*	@details 結果アニメーションクラスの描画処理を行う
*	@param なし
*	@return なし
*/
void ResultAnimation::Render()
{

	// 画像描画
	for (int i = 0; i < m_frameCols; ++i)
	{
		DrawQuad(m_pTextures, m_vertices, m_positionX + (SIZE_X * i), m_positionsY[i], SIZE_X, SIZE_Y, i, m_frameCols, m_frameRows);
	}

}
/*
*	@brief テクスチャ決定
*	@details 結果アニメーションクラスで使うテクスチャを決定する
*	@param なし
*	@return なし
*/
void ResultAnimation::DecideTexture()
{
	// ゲームオーバーかゲームクリアかでテクスチャ名を決定
	std::string textureName = m_gameclear ? "GameClear" : "GameOver";
	// テクスチャの行数と列数を設定
	// 行数は1
	m_frameRows = 1;
	// 列数はゲームクリアなら9、ゲームオーバーなら8
	m_frameCols = m_gameclear ? 9 : 8;
	// 画像の位置Xを設定
	m_positionX = m_gameclear ? POS_X_CLEAR : POS_X_OVER;
	// 画像の位置Yの数を設定
	m_positionsY.resize(m_frameCols);
	// 画像の位置Yを設定
	for (int i = 0; i < m_frameCols; ++i)m_positionsY[i] = POS_Y;
	// マネージャーからテクスチャを取得
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture(textureName));
	// 決定済みにする
	m_decideTexture = false;
}
/*
*	@brief シェーダーの作成
*	@details 結果アニメーションクラスのシェーダーを作成する
*	@param なし
*	@return なし
*/
void ResultAnimation::CreateShaders()
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーをセット
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセット
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをセット（使わないのでnullptr）
	m_shaders.gs = nullptr;
}

void ResultAnimation::DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture, DirectX::VertexPositionTexture* vertices, float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点座標の設定
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };// 左上
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };// 右上
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };// 右下
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };// 左下
	// コンスタントバッファに渡すデータを設定
	// ワールド行列を単位行列に設定
	m_constBuffer.matWorld = Matrix::Identity;
	// ビュー行列を単位行列に設定
	m_constBuffer.matView = Matrix::Identity;
	// プロジェクション行列を単位行列に設定
	m_constBuffer.matProj = Matrix::Identity;
	// アニメーションのコマを設定
	m_constBuffer.count = Vector4((float)(frameIndex));
	// 高さを設定
	m_constBuffer.height = Vector4((float)(frameRows));
	// 幅を設定
	m_constBuffer.width = Vector4((float)(frameCols));
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBufferからID3D11Bufferへの変換
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// デプスステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 板ポリゴンを描画
	m_pDrawPolygon->DrawTexture(vertices);
	//	シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief 結果を設定
*	@details 結果アニメーションクラスにゲームオーバーかゲームクリアかを設定する
*	@param gameover ゲームオーバーかどうか
*	@param gameclear ゲームクリアかどうか
*	@return なし
*/
void ResultAnimation::SetResult(bool gameover, bool gameclear)
{
	// ゲームオーバーフラグとゲームクリアフラグを設定
	m_gameover = gameover;
	m_gameclear = gameclear;


	// ゲームオーバーかゲームクリアなら
	if (m_gameover || m_gameclear)
	{
		// アニメーションを有効にする
		m_animationEnable = true;
		// テクスチャを決定
		if (m_decideTexture)DecideTexture();
	}
	// どちらでもない場合はアニメーションを無効にする
	else
	{
		m_animationEnable = false;
	}



}