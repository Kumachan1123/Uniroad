/*
*	@file MedalCounter.cpp
*	@brief メダルカウンターの処理を定義するクラス
*/
#include "pch.h"
#include "MedalCounter.h"
/*
*	@brief	インプットレイアウト
*	@return なし
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  MedalCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief コンストラクタ
*	@details メダルカウンタークラスのコンストラクタ
*	@param なし
*	@return なし
*/
MedalCounter::MedalCounter()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_collectedMedalCountSave(0)// 収集したメダルの数を保存する変数
	, m_viewportWidth(0)// ビューポートの幅
	, m_viewportHeight(0)// ビューポートの高さ
	, m_frameRows{ 1 }//	画像の行数
	, m_frameCols{ 10 }//	画像の列数
	, m_verticesMedal{} // メダルの頂点配列
	, m_verticesX{} // 「×」の頂点配列
	, m_verticesNumber1{} // 1の位の数字の頂点配列
	, m_verticesNumber10{} // 10の位の数字の頂点配列
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
{
}
/*
*	@brief デストラクタ
*	@details メダルカウンタークラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
MedalCounter::~MedalCounter()
{
	// メダルカウンターの終了処理を行う
	// 共通リソースへのポインタをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief デストラクタ
*	@details メダルカウンタークラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
void MedalCounter::Initialize(CommonResources* resources, int width, int height)
{
	// 共通リソースへのポインタを設定
	m_pCommonResources = resources;
	// ウィンドウの幅と高さを設定
	m_viewportWidth = width;
	m_viewportHeight = height;
	// メダル画像
	m_pMedalTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Medal"));
	// 「×」画像
	m_pXTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Multiply"));
	// 数字画像
	m_pNumberTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Number"));
	// 描画クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダーの作成
	CreateShaders();
}
/*
*	@brief シェーダーの作成
*	@details メダルカウンターのシェーダーを作成する
*	@param なし
*	@return なし
*/
void MedalCounter::CreateShaders()
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
/*
*	@brief 更新する
*	@details メダルカウンターの更新処理を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void MedalCounter::Update(float elapsedTime)
{
	// 未使用変数の警告を無視
	UNREFERENCED_PARAMETER(elapsedTime);
	// 10の位の計算
	m_collectedMedalCount.unit10 = m_collectedMedalCountSave / 10;
	// 1の位の計算
	m_collectedMedalCount.unit1 = m_collectedMedalCountSave % 10;
}

void MedalCounter::Render()
{
	// メダル画像描画
	DrawQuad(m_pMedalTextures, m_verticesMedal, MEDAL_POS_X, MEDAL_POS_Y, MEDAL_SIZE_X, MEDAL_SIZE_Y, 0, 1, 1);
	// 「×」画像描画
	DrawQuad(m_pXTextures, m_verticesX, X_POS_X, X_POS_Y, X_SIZE_X, X_SIZE_Y, 0, 1, 1);
	// 10の位の数字画像描画
	DrawQuad(m_pNumberTextures, m_verticesMedal, NUMBER10_POS_X, NUMBER10_POS_Y, NUMBER10_SIZE_X, NUMBER10_SIZE_Y, m_collectedMedalCount.unit10, m_frameCols, m_frameRows);
	// 1の位の数字画像描画
	DrawQuad(m_pNumberTextures, m_verticesMedal, NUMBER1_POS_X, NUMBER1_POS_Y, NUMBER1_SIZE_X, NUMBER1_SIZE_Y, m_collectedMedalCount.unit1, m_frameCols, m_frameRows);

}

void MedalCounter::DrawQuad(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
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
