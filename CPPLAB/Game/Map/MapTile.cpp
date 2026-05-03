#include "pch.h"
#include "MapTile.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MapTile::MapTile()
	: m_pDrawPolygon(DrawPolygon::GetInstance())
	, m_pCreateShader(CreateShader::GetInstance())
	, m_pTexture(nullptr)
	, m_vertices{}
	, m_constBuffer{}
	, m_frameRows(36)
	, m_frameCols(40)
	, m_chipNum(41)
	, m_position(Vector3::Zero)
	, m_row(0)
	, m_col(0)
{
	Initialize();
}

void MapTile::Initialize()
{
	auto device = MyResourecs::Get().GetDeviceResources()->GetD3DDevice();
	auto context = MyResourecs::Get().GetDeviceResources()->GetD3DDeviceContext();

	// DrawPolygonの初期化
	m_pDrawPolygon->InitializePositionTexture(MyResourecs::Get().GetDeviceResources());

	// 入力レイアウト定義
	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// CreateShaderの初期化
	m_pCreateShader->Initialize(device, &inputLayout[0],
								static_cast<UINT>(inputLayout.size()), m_inputLayout);

	// シェーダーの作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);

	m_inputLayout = m_pCreateShader->GetInputLayout();

	// コンスタントバッファの作成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));

	// シェーダーの設定
	m_shaders.vs = m_pVertexShader.Get();
	m_shaders.ps = m_pPixelShader.Get();
	m_shaders.gs = nullptr;

	// テクスチャマネージャーからOverWorldテクスチャを取得
	auto textureManager = MyResourecs::Get().GetTextureManager();
	m_pTexture = textureManager->GetTexture("OverWorld");
}

void MapTile::Update(float elapsedTime)
{}

void MapTile::Render(const Matrix& view, const Matrix& projection)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// XZ平面に板ポリゴンを配置（Y = -1）
	const float size = .5f;
	const float y = -.5f;

	// 頂点情報を設定（XZ平面）
	m_vertices[0] = { VertexPositionTexture(Vector3(-size, y, -size), Vector2(0, 0)) };	// 左奥
	m_vertices[1] = { VertexPositionTexture(Vector3(size, y, -size), Vector2(1, 0)) };	// 右奥
	m_vertices[2] = { VertexPositionTexture(Vector3(size, y, size), Vector2(1, 1)) };	// 右手前
	m_vertices[3] = { VertexPositionTexture(Vector3(-size, y, size), Vector2(0, 1)) };	// 左手前

	// ワールド行列を設定
	// タイルの位置に移動
	m_position.x = m_col; // 列に基づいてX座標を設定
	m_position.z = m_row; // 行に基づいてZ座標を設定
	Matrix world = Matrix::CreateTranslation(m_position);
	m_constBuffer.matWorld = world.Transpose();
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = projection.Transpose();

	// アニメーション用パラメータ（使用しないため0を設定）
	m_constBuffer.count = Vector4(float(m_chipNum));
	// テクスチャの列を設定
	m_constBuffer.height = Vector4((float)(m_frameRows));
	// テクスチャの行を設定
	m_constBuffer.width = Vector4((float)(m_frameCols));

	// コンスタントバッファを更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);

	// シェーダーバッファを設定
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);

	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::POINT_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_COUNTERCLOCKWISE,
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);

	// テクスチャを設定
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	if (m_pTexture != nullptr)
	{
		textures.push_back(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(m_pTexture));
	}

	// 描画開始
	m_pDrawPolygon->DrawStart(m_inputLayout.Get(), textures);

	// シェーダーを設定
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);

	// テクスチャ付きポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_vertices);

	// シェーダーを解放
	m_pDrawPolygon->ReleaseShader();
}

void MapTile::Finalize()
{
	m_pVertexShader.Reset();
	m_pPixelShader.Reset();
	m_inputLayout.Reset();
	m_pCBuffer.Reset();
	m_pTexture = nullptr;
}
