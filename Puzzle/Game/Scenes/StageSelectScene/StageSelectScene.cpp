/*
*	@file StageSelectScene.cpp
*	@brief ステージ選択シーンクラス
*/
#include "pch.h"
#include "StageSelectScene.h"

/*
*	@brief コンストラクタ
*	@details ステージ選択シーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_pMiniCharacterBase(nullptr) // ミニキャラベースへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_bDrawDebugPlane(false) // デバッグ平面描画フラグ
	, m_stageNumber(0) // ステージ番号
	, m_nextSceneID(sceneID) // 次のシーンID
{
}
/*
*	@brief デストラクタ
*	@details ステージ選択シーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelectScene::~StageSelectScene()
{
	// 何もしない
}

/*
*	@brief 初期化
*	@details ステージ選択シーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	// 名前空間のエイリアス
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースをセット
	m_pCommonResources = resources;
	// カメラを作成する
	CreateCamera();
	// デバイスリソースを取得する
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto context = deviceResources->GetD3DDeviceContext();
	const auto device = deviceResources->GetD3DDevice();
	const auto states = m_pCommonResources->GetCommonStates();
	// 各種ビューポートを作成する
	CreateViewports();
	// グリッド床を作成する
	m_pGridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	// ミニキャラを作成する
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// ミニキャラベースにCSVマップを設定
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// ミニキャラベースにCSVアイテムを設定
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// ミニキャラベースに次のタイルを設定
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// ミニキャラを初期化する
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// ミニキャラベースにミニキャラをアタッチ
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// ステージセレクトを作成する
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// ステージセレクトを初期化する
	m_pStageSelect->Initialize();
}
/*
*	@brief 更新
*	@details ステージ選択シーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void StageSelectScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// ミニキャラの更新
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	const auto& keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	if (keyState.Space)
	{
		// スペースキーが押されたらシーン変更フラグを立てる
		m_isChangeScene = true;
	}
	// マウス座標取得
	int mouseX = m_pCommonResources->GetInputManager()->GetMouseState().x;
	int mouseY = m_pCommonResources->GetInputManager()->GetMouseState().y;
	// ウィンドウハンドルを取得
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// ウィンドウサイズ取得
	RECT rect;
	// クライアント領域サイズを取得
	GetClientRect(hwnd, &rect);
	int screenWidth = rect.right;
	int screenHeight = rect.bottom;
	// レイ生成
	Ray ray = ScreenPointToRay(mouseX, mouseY, screenWidth, screenHeight, m_view, m_projection);
	// 平面の定義
	Plane plane(Vector3(0.0f, 1.0f, 0.0f), 0.0f); // Y=0の平面
	// レイと平面の交差判定
	Vector3 intersection;
	bool hit = RayIntersectPlane(ray, plane, intersection);
	// --- デバッグ情報---
	const auto debugString = m_pCommonResources->GetDebugString();
	// レイと当たったかを表示
	debugString->AddString("Ray Hit:%s", hit ? "true" : "false");
}

/*
*	@brief 描画
*	@details ステージ選択シーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Render()
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ビュー行列を取得
	m_view = m_pFixedCamera->GetViewMatrix();
	// ステージセレクトの描画
	m_pStageSelect->Render(m_view, m_projection);
	// ミニキャラの描画
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// グリッド床の描画
	m_pGridFloor->Render(context, m_view, m_projection);
	// 頂点設定
	Vector3 p0(-1, 0, -1);
	Vector3 p1(1, 0, -1);
	Vector3 p2(1, 0, 1);
	Vector3 p3(-1, 0, 1);
	// 色設定
	Color color = Color(1, 0, 0);
	// 各頂点を配列に登録
	m_debugPlaneVerticesPosition[0] = p0;
	m_debugPlaneVerticesPosition[1] = p1;
	m_debugPlaneVerticesPosition[2] = p2;
	m_debugPlaneVerticesPosition[3] = p3;
	DrawDebugLine(m_debugPlaneVerticesPosition, color, m_view, m_projection);


	// --- デバッグ情報（例） ---
	const auto debugString = m_pCommonResources->GetDebugString();
}

/*
*	@brief 終了
*	@details ステージ選択シーンクラスの終了を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::Finalize()
{
}


/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return 次のシーンID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// ゲームオーバーシーンへ
		return IScene::SceneID::PLAY;
	}
	// シーン変更がない場合何もしない
	return IScene::SceneID::NONE;
}

/*
*	@brief カメラに関する設定をする
*	@details ステージ選択シーンクラスのカメラ設定を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	//// デバッグカメラを作成する
	//m_debugCamera = std::make_unique<mylib::DebugCamera>();
	//m_debugCamera->Initialize(rect.right * 0.7f, rect.bottom);
	// 固定カメラを作成する
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);

	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief 各種ビューポートを設定する
*	@details ステージ選択シーンクラスのビューポート設定を行う
*	@param なし
*	@return なし
*/
void StageSelectScene::CreateViewports()
{
}
/*
*	@brief マウス座標からワールドレイを生成
*	@details マウス座標からワールドレイを生成する
*	@param mouseX マウスのX座標
*	@param mouseY マウスのY座標
*	@param screenWidth 画面の幅
*	@param screenHeight 画面の高さ
*	@param view ビュー行列
*	@param projection 射影行列
*	@return 生成されたレイ
*/
DirectX::SimpleMath::Ray StageSelectScene::ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	using namespace DirectX::SimpleMath;
	// スクリーン座標をNDC(-1～1)の範囲に変換
	float px = ((2.0f * mouseX) / screenWidth - 1.0f);
	float py = (1.0f - (2.0f * mouseY) / screenHeight);
	// Clip空間でnear/far平面の座標を計算
	Vector3 rayStartNDC(px, py, 0.0f);
	Vector3 rayEndNDC(px, py, 1.0f);
	// 逆行列でワールド行列に変換
	Matrix invViewProj = (view * projection).Invert();
	Vector3 rayStartWorld = Vector3::Transform(rayStartNDC, invViewProj);
	Vector3 rayEndWorld = Vector3::Transform(rayEndNDC, invViewProj);
	// レイを生成
	Vector3 dir = (rayEndWorld - rayStartWorld);
	dir.Normalize();
	return Ray(rayStartWorld, dir);

}
/*
*	@brief Planeとレイの交差判定
*	@details レイと平面の交差判定を行う
*	@param ray レイ
*	@param planePoint 平面上の点
*	@param planeNormal 平面の法線
*	@param outIntersection 交差点の出力
*	@return 交差したかどうかの真偽値
*/
bool StageSelectScene::RayIntersectPlane(const DirectX::SimpleMath::Ray& ray, const DirectX::SimpleMath::Plane& plane, DirectX::SimpleMath::Vector3& outIntersection)
{
	using namespace DirectX::SimpleMath;
	const Vector3& n = plane.Normal();
	float d = plane.D();

	float denom = ray.direction.Dot(n);
	if (fabs(denom) < 1e-6f) return false; // 平行

	// 平面上の点は -d * n で取れる（ax+by+cz+d=0より）
	Vector3 planePoint = -d * n;

	float t = (planePoint - ray.position).Dot(n) / denom;
	if (t < 0) return false;

	outIntersection = ray.position + t * ray.direction;
	if (outIntersection.x < -1.0f || outIntersection.x > 1.0f ||
		outIntersection.z < -1.0f || outIntersection.z > 1.0f)
	{
		return false;
	}

	return true;
}

void StageSelectScene::DrawDebugLine(
	const DirectX::SimpleMath::Vector3 p[],
	const DirectX::SimpleMath::Color& color,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	static std::unique_ptr<BasicEffect> s_effect;
	static std::unique_ptr<PrimitiveBatch<VertexPositionColor>> s_batch;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> s_inputLayout;
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	if (!s_effect)
	{
		s_effect = std::make_unique<BasicEffect>(device);
		s_effect->SetVertexColorEnabled(true);

		s_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

		// 入力レイアウトの生成
		void const* shaderByteCode;
		size_t byteCodeLength;
		s_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
		device->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&s_inputLayout);
	}

	// 行列設定
	s_effect->SetWorld(Matrix::Identity);
	s_effect->SetView(m_view);
	s_effect->SetProjection(m_projection);

	s_effect->Apply(context);
	context->IASetInputLayout(s_inputLayout.Get());

	// ワイヤーフレーム矩形を描画
	s_batch->Begin();
	s_batch->DrawLine(VertexPositionColor(p[0], color), VertexPositionColor(p[1], color));
	s_batch->DrawLine(VertexPositionColor(p[1], color), VertexPositionColor(p[2], color));
	s_batch->DrawLine(VertexPositionColor(p[2], color), VertexPositionColor(p[3], color));
	s_batch->DrawLine(VertexPositionColor(p[3], color), VertexPositionColor(p[0], color));
	s_batch->End();
}