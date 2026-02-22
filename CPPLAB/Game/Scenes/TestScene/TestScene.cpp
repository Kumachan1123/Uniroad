/*
*		@file TestScene.cpp
*		@brief テストシーンクラス
*/

#include "pch.h"
#include "TestScene.h"
/*
*	@brief コンストラクタ
*	@details テストシーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
TestScene::TestScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
	, m_generatedEnvironmentMap(false) // 環境マップを生成したか
{
}

/*
*	@brief デストラクタ
*	@details テストシーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
TestScene::~TestScene()
{    // メタルムーンの Finalize を呼ぶ
	if (m_pMetalMoon)
	{
		m_pMetalMoon->Finalize();
		m_pMetalMoon.reset(); // unique_ptr を破棄
	}

	// 他のリソースも同様に Finalize / reset する
	if (m_pStage)
	{
		m_pStage->Finalize();
		m_pStage.reset();
	}
}
/*
*	@brief 初期化
*	@details テストシーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void TestScene::Initialize(CommonResources* resources)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// DirectXのSimpleMath名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソースを取得
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// カメラを作成する
	CreateCamera();
	// フェードを作成する
	m_pFade = std::make_unique<Fade>();
	// フェードを初期化する
	m_pFade->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
	// フェードインに移行
	m_pFade->SetState(Fade::FadeState::FadeIn);
	// 天球モデルを作成する
	m_pSky = std::make_unique<Sky>(m_pCommonResources);
	// 天球モデルを初期化する
	m_pSky->Initialize();
	// 天球のスケールをセット
	m_pSky->SetScale(Vector3(10.f));
	// 天球の位置をセット
	m_pSky->SetPosition(Vector3(0.f, 0.f, 0.f));
	// UIテキストを作成する
	m_pUIText = std::make_unique<UIText>();
	// UIテキストを初期化する
	m_pUIText->Initialize(deviceResources);
	// 適当な文字をセット
	m_pUIText->AddString(L"使い方によっては");
	m_pUIText->AddString(L"こんなことも");
	m_pUIText->AddString(L"できちゃいます！");
	m_pUIText->AddString(L"いやぁ～便利ですねぇ～");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L"ユッケビビンバ");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L"科学の力ってすげー！");
	m_pUIText->InsertWaitPoint();
	m_pUIText->AddString(L".........");
	m_pUIText->AddString(L"返事がない");
	m_pUIText->AddString(L"ただの屍のようだ");


	// 文字の位置をセット
	m_pUIText->SetPosition({ 100.0f, 50.0f });
	// 文字の色をセット
	m_pUIText->SetColor(Color{ 1,1,1,1 });
	// 文字のスケールをセット
	m_pUIText->SetScale(1.0f);
	// 文字のアライメントをセット
	m_pUIText->SetAlignment(TextAlignment::LEFT);

	// ステージを作成する
	m_pStage = std::make_unique<Stage>();
	// ステージを初期化する
	m_pStage->Initialize(m_pCommonResources);
	// メタルムーンを作成する
	m_pMetalMoon = std::make_unique<MetalMoon>();
	// メタルムーンを初期化する
	m_pMetalMoon->Initialize(m_pCommonResources);

	// モデルを受け取る
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("MetalMoon");

}
/*
*	@brief 更新
*	@details テストシーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void TestScene::Update(float elapsedTime)
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// SinpleMathの名前空間のエイリアス
	using namespace DirectX::SimpleMath;
	// 時間を更新
	m_time += elapsedTime;
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// デバッグカメラを更新する
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// ビュー行列を取得
	m_view = m_debugCamera->GetViewMatrix();
	// カメラの位置を調整
	m_pFixedCamera->SetCameraDistance(CAMERA_POSITION);
	// フェードの更新
	if (m_time >= FADE_START_TIME) m_pFade->Update(elapsedTime);
	// フェードインが終わったらフェード状態をなくす
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)m_pFade->SetState(Fade::FadeState::None);
	// UIテキストを更新する
	if (m_pFade->GetState() == Fade::FadeState::None)m_pUIText->Update(elapsedTime);
	// 天球を更新する
	m_pSky->Update(elapsedTime);
	// キーボード入力を取得
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// スペースキーが押されたら（何らかのフラグがたったら）
	if (keyState.Space)
	{
		// フェードアウトに移行
		if (m_pUIText->IsFinishedAll())m_pFade->SetState(Fade::FadeState::FadeOut);
		// 次へ要求を出す
		else m_pUIText->RequestNext();
	}
	// フェードアウトが完了していたら、シーン遷移フラグを立てる
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;

	// 環境マップを生成する
	GenerateEnvironmentMap(Vector3::Zero);
}
/*
*	@brief 描画
*	@details テストシーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void TestScene::Render()
{
	// DirectXの名前空間のエイリアス
	using namespace DirectX;
	// DirectXのSimpleMath名前空間のエイリアス
	using namespace DirectX::SimpleMath;

	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();

	// 天球の描画
	m_pSky->Render(m_view, m_projection);
	// ステージの描画
	m_pStage->Render(m_view, m_projection);
	// メタルムーンの描画
	m_pMetalMoon->Render(m_view, m_projection);

	//// モデルの描画
	//m_pModel->Draw(context, *states, Matrix::Identity, m_view, m_projection);


	// UIテキストを描画する
	m_pUIText->Render();
	// フェードを描画する
	m_pFade->Render();

}
/*
*	@brief 終了
*	@details テストシーンクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void TestScene::Finalize()
{
}
/*
* 	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID TestScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// ステージセレクトへ
	return IScene::SceneID::TITLE;
}
/*
*	@brief カメラに関する設定をする
*	@details カメラの作成と設定を行う
*	@param なし
*	@return なし
*/
void TestScene::CreateCamera()
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	// 固定カメラを初期化する
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);
	// デバッグカメラを作成する
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	// デバッグカメラを初期化する
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(FOV),// 視野角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),// アスペクト比
		0.1f, 10000.0f);// ニアクリップ距離、ファークリップ距離
	// カメラに射影行列をセット
	m_pFixedCamera->SetProjectionMatrix(m_projection);

}
// TestScene::GenerateEnvironmentMap - MakeRotatedCubeVectors を使って回転補正を適用
// 修正版: mipmap を「確保」するだけでなく、D3D11 の GenerateMips で生成する
void TestScene::GenerateEnvironmentMap(const DirectX::SimpleMath::Vector3& position)
{
	using Microsoft::WRL::ComPtr;
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// 既に生成済みなら何もしない（パフォーマンス最適化）
	if (m_generatedEnvironmentMap) return;

	auto* device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	auto* context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	if (!device || !context) return;

	const UINT size = 256;
	HRESULT hr = S_OK;

	// --- 回転角 (Y軸) をここで指定 ---
	const float rotationDegrees = 90.0f; // 必要に応じて変更

	// 回転済み配列を作る（MakeRotatedCubeVectors を利用）
	SimpleMath::Vector3 rotatedForward[6];
	SimpleMath::Vector3 rotatedUp[6];
	MakeRotatedCubeVectors(rotationDegrees, rotatedForward, rotatedUp);

	// =========================================================================
	// Cubemap texture (with full mip chain + GenerateMips)
	// =========================================================================
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = size;
	texDesc.Height = size;

	// 重要: 0 を指定するとフル mip チェーンが作られる（256なら 9段: 256..1）
	texDesc.MipLevels = 0;

	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	// GenerateMips には RenderTarget と ShaderResource の両方が必要
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	// 重要: GenerateMips を呼ぶためのフラグを付ける
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ComPtr<ID3D11Texture2D> envCube;
	hr = device->CreateTexture2D(&texDesc, nullptr, envCube.GetAddressOf());
	if (FAILED(hr) || !envCube)
	{
		OutputDebugStringA("CreateTexture2D(envCube) failed\n");
		return;
	}
	envCube->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen("EnvCube") + 1, "EnvCube");

	// RTV を 6 個作成（各スライスごと / mip0 に描画）
	ComPtr<ID3D11RenderTargetView> cubeRTVs[6];
	for (UINT i = 0; i < 6; ++i)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;            // 描画は mip0 のみに行う
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		rtvDesc.Texture2DArray.ArraySize = 1;

		hr = device->CreateRenderTargetView(envCube.Get(), &rtvDesc, cubeRTVs[i].GetAddressOf());
		if (FAILED(hr) || !cubeRTVs[i])
		{
			OutputDebugStringA("CreateRenderTargetView failed for face\n");
			return;
		}
		char name[32];
		sprintf_s(name, "EnvCubeRTV_%u", i);
		cubeRTVs[i]->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(name) + 1, name);
	}

	// 深度バッファ作成（共通1つを各面で使う）
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = size;
	depthDesc.Height = size;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> depthTex;
	hr = device->CreateTexture2D(&depthDesc, nullptr, depthTex.GetAddressOf());
	if (FAILED(hr) || !depthTex)
	{
		OutputDebugStringA("CreateTexture2D(depth) failed\n");
		return;
	}
	depthTex->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen("EnvDepthTex") + 1, "EnvDepthTex");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ComPtr<ID3D11DepthStencilView> dsv;
	hr = device->CreateDepthStencilView(depthTex.Get(), &dsvDesc, dsv.GetAddressOf());
	if (FAILED(hr) || !dsv)
	{
		OutputDebugStringA("CreateDepthStencilView failed\n");
		return;
	}
	dsv->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen("EnvDSV") + 1, "EnvDSV");

	// ビューポートをセット
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)size;
	vp.Height = (FLOAT)size;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	context->RSSetViewports(1, &vp);

	// クリア色（水色）
	const FLOAT clearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

	// 各面をレンダリング（描画対象は Sky/Stage など、MetalMoon自身は除外）
	for (int face = 0; face < 6; ++face)
	{
		Matrix view = Matrix::CreateLookAt(position, position + rotatedForward[face], rotatedUp[face]);
		Matrix proj = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV2, 1.0f, 0.1f, 10000.0f);

		ID3D11RenderTargetView* rtvPtr = cubeRTVs[face].Get();
		context->OMSetRenderTargets(1, &rtvPtr, dsv.Get());
		context->ClearRenderTargetView(rtvPtr, clearColor);
		context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// シーン描画（ここで view/proj をシェーダに渡す必要あり）
		m_pStage->Render(view, proj);
		m_pSky->Render(view, proj);
	}

	// =========================================================================
	// SRV 作成（TextureCube / full mip chain）
	// =========================================================================
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

	// 重要: -1 を指定すると「全 mip」を SRV に含める
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = (UINT)-1;

	ComPtr<ID3D11ShaderResourceView> envCubeSRV;
	hr = device->CreateShaderResourceView(envCube.Get(), &srvDesc, envCubeSRV.GetAddressOf());
	if (FAILED(hr) || !envCubeSRV)
	{
		OutputDebugStringA("CreateShaderResourceView failed\n");
		return;
	}
	envCubeSRV->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen("EnvCubeSRV") + 1, "EnvCubeSRV");

	// =========================================================================
	// 重要: mipmap を生成する（mip0 から自動ダウンサンプル）
	// =========================================================================
	context->GenerateMips(envCubeSRV.Get());

	// Material 側に渡す（Material::SetEnvironmentCubeSRV は内部で ComPtr 保持を想定）
	if (m_pMetalMoon && m_pMetalMoon->GetMaterial())
	{
		m_pMetalMoon->GetMaterial()->SetEnvironmentCubeSRV(envCubeSRV.Get());
	}

	m_generatedEnvironmentMap = true;
}
void TestScene::MakeRotatedCubeVectors(float degrees, DirectX::SimpleMath::Vector3 outForward[6], DirectX::SimpleMath::Vector3 outUp[6])
{
	using namespace DirectX;
	float theta = XMConvertToRadians(degrees);
	XMMATRIX rot = XMMatrixRotationY(theta);

	for (int i = 0; i < 6; ++i)
	{
		XMVECTOR vF = XMVectorSet(kCubeForward[i].x, kCubeForward[i].y, kCubeForward[i].z, 0.0f);
		XMVECTOR vU = XMVectorSet(kCubeUp[i].x, kCubeUp[i].y, kCubeUp[i].z, 0.0f);

		XMVECTOR vFr = XMVector3TransformNormal(vF, rot);
		XMVECTOR vUr = XMVector3TransformNormal(vU, rot);

		// 正規化して SimpleMath::Vector3 に書き戻す
		vFr = XMVector3Normalize(vFr);
		vUr = XMVector3Normalize(vUr);

		outForward[i].x = XMVectorGetX(vFr);
		outForward[i].y = XMVectorGetY(vFr);
		outForward[i].z = XMVectorGetZ(vFr);

		outUp[i].x = XMVectorGetX(vUr);
		outUp[i].y = XMVectorGetY(vUr);
		outUp[i].z = XMVectorGetZ(vUr);
	}
}
