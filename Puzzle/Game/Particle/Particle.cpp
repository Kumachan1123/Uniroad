/*
*	@file Particle.cpp
*	@brief パーティクルクラスの実装
*/
#include "pch.h"
#include "Particle.h"
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@detail クラスの生成処理を行う
*	@param type パーティクルのタイプ
*	@param size パーティクルのサイズ
*	@param limit パーティクルの生成数の制限(デフォルトは100)
*	@return なし
*/
Particle::Particle(Utility::Type type, float size, size_t limit)
	: m_pCommonResources{}// 共通リソース
	, m_timer(0.0f)// 経過時間
	, m_elapsedTime(0.0f)// フレーム時間
	, m_pDR{}// デバイスリソース
	, m_pCBuffer{}// コンスタントバッファ
	, m_pInputLayout{}// 入力レイアウト
	, m_pTexture{}// テクスチャ
	, m_pVertexShader{}// 頂点シェーダー
	, m_pPixelShader{}// ピクセルシェーダー
	, m_pGeometryShader{}// ジオメトリシェーダー
	, m_world{}// ワールド行列
	, m_view{}// ビュー行列
	, m_proj{}// プロジェクション行列
	, m_billboard{}// ビルボード行列
	, m_type{ type }// パーティクルのタイプ
	, m_size{ size }// パーティクルのサイズ(渡した値の十倍）
	, m_params{}// パーティクルのパラメーター
	, m_anim{ 0 }// フレーム数
	, m_animTime{ 0.0f }// アニメーション時間
	, m_animSpeed{ 30.0f }// アニメーションの速度
	, m_frameRows{ 1 }// フレームの行数
	, m_frameCols{ 1 }// フレームの列数
	, m_isCreate{ true }// パーティクルの生成フラグ
	, m_limit{ limit }// パーティクルの生成数の制限
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
}
/*
*	@brief	デストラクタ
*	@detail クラスの破棄処理を行う
*	@param なし
*	@return なし
*/
Particle::~Particle() {/*do nothing.*/ }
/*
*	@brief	初期化
*	@detail クラスの初期化処理を行う
*	@param CommonResources* resources 共通リソース
*	@return なし
*/
void Particle::Initialize(CommonResources* resources)
{
	// 共通リソースを取得
	m_pCommonResources = resources;
	// デバイスリソースを取得
	m_pDR = m_pCommonResources->GetDeviceResources();

	// 画像の読み込み
	switch (m_type)
	{
	case Utility::Type::STEAM:// 水蒸気
		//	アニメーションの速度
		m_animSpeed = 1;
		//	フレームの列数
		m_frameCols = 1;
		//	フレームの行数
		m_frameRows = 1;
		// テクスチャの取得
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Dust"));
		// ピクセルシェーダーの作成
		m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Dust.cso", m_pPixelShader);
		break;
	case Utility::Type::SHINE:// 光
		//	アニメーションの速度
		m_animSpeed = 1;
		//	フレームの列数
		m_frameCols = 1;
		//	フレームの行数
		m_frameRows = 1;
		// テクスチャの取得
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Shine"));// メインテクスチャ
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Shine_Gradation"));// サブテクスチャ
		// ピクセルシェーダーの作成
		m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Shine.cso", m_pPixelShader);
		break;
	default:// それ以外のパーティクル
		break;
	}
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// シェーダーの作成
	CreateShaders();
	// 板ポリゴン描画用
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief 更新
*	@detail クラスの更新処理を行う
*	@param float elapsedTime 経過時間
*	@return なし
*/
void Particle::Update(float elapsedTime)
{
	// フレーム時間をセット
	m_elapsedTime = elapsedTime;
	// タイマーを更新
	m_timer += elapsedTime;
	// アニメーションの更新
	m_animTime += elapsedTime * m_animSpeed;
	// アニメーションタイマーが一定時間を超えたらリセット
	if (m_animTime >= 2.0f)
	{
		// フレーム数を更新
		m_anim++;
		// タイマーをリセット
		m_animTime = 0.0f;
	}
	// 生成可能ならパーティクルを生成する
	if (m_isCreate && m_particleUtility.size() <= m_limit)
	{
		// パーティクルの生成
		Utility pU(m_params);
		// 生成したパーティクルをリストに追加
		m_particleUtility.push_back(pU);
	}
	//// タイマーをリセット
	//m_timer = 0.0f;
	//	timerを渡して更新処理を行う
	for (std::list<Utility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// パーティクルの更新
		if (!(ite)->Update(elapsedTime))
		{
			// パーティクルの更新がfalseを返した場合、つまりライフが0以下になった場合
			ite = m_particleUtility.erase(ite);
			// イテレータを更新
			if (ite == m_particleUtility.end()) break;
		}
	}
}
/*
*	@brief 描画
*	@detail クラスの描画処理を行う
*	@param const DirectX::SimpleMath::Matrix& view ビュー行列
*	@param const DirectX::SimpleMath::Matrix& proj プロジェクション行列
*	@return なし
*/
void Particle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 一定期間が過ぎたら描画しない

	// カメラの方向を取得
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	// カメラの方向を正規化
	cameraDir.Normalize();
	// カメラの前方に近い順にソート
	m_particleUtility.sort(
		[&](Utility lhs, Utility  rhs)
		{
			// 内積でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	// 頂点をクリア
	m_vertices.clear();
	// リストのパーティクルを全て描画する
	for (Utility& li : m_particleUtility)
	{
		// 内積がマイナスの場合はカメラの後ろなので表示する必要なし
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) continue;
		// 頂点の構造体を作成
		VertexPositionColorTexture vPCT{};
		// 座標
		vPCT.position = XMFLOAT3(li.GetPosition());
		// テクスチャの色
		vPCT.color = XMFLOAT4(li.GetNowColor());
		// 現在のテクスチャのスケールをXMFLOAT2のXに入れる
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);
		// 頂点を追加
		m_vertices.push_back(vPCT);
	}
	// 表示する点がない場合は描画を終わる
	if (m_vertices.empty())return;
	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	// ビュー行列を転置してセットする
	m_constantBuffer.matView = view.Transpose();
	// プロジェクション行列を転置してセットする
	m_constantBuffer.matProj = proj.Transpose();
	// ビルボード行列をワールド行列としてセットする
	m_constantBuffer.matWorld = m_billboard.Transpose();
	// 色をセットする
	m_constantBuffer.color = Vector4(1, 1, 1, 1);
	// フレーム数をセットする
	m_constantBuffer.count = Vector4((float)(m_anim));
	// 行数をセットする
	m_constantBuffer.height = Vector4((float)(m_frameRows));
	// 列数をセットする
	m_constantBuffer.width = Vector4((float)(m_frameCols));
	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// シェーダーにバッファを渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE); // 深度ステンシルステート
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// 指定した座標を中心に、シェーダ側で板ポリゴンを生成・描画させる
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief ビルボード行列の作成
*	@detail ビルボード行列の作成処理を行う
*	@param target 注視点
*	@param eye カメラの位置
*	@param up カメラの上方向
*	@return なし
*/
void Particle::CreateBillboard(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& up)
{
	// 名前空間の使用
	using namespace DirectX::SimpleMath;
	// ビルボード行列の作成
	m_billboard = Matrix::CreateBillboard(Vector3::Zero, eye - target, up);
	// 単位行列
	Matrix rot = Matrix::Identity;
	// X軸反転
	rot._11 = -1;
	// Z軸反転
	rot._33 = -1;
	// カメラの位置
	m_cameraPosition = eye;
	// カメラの注視点
	m_cameraTarget = target;
	// ビルボード行列の反転
	m_billboard = rot * m_billboard;
}
/*
*	@brief	シェーダーの作成
*	@detail シェーダーの作成処理を行う(ピクセルシェーダーは事前にタイプごとに分岐して作成する）
*	@param なし
*	@return なし
*/
void Particle::CreateShaders()
{
	// 頂点シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_pVertexShader);
	// ジオメトリシェーダーの作成
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_pGeometryShader);

	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// 定数バッファ作成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ParticleBuffer));
	// シェーダーの構造体に頂点シェーダーをセットする
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーをセットする
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーをセットする
	m_shaders.gs = m_pGeometryShader.Get();
}

