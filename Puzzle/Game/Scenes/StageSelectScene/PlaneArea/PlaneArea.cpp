/*
*	@file PlaneArea.cpp
*	@brief 平面エリアクラス
*/
#include "pch.h"
#include "PlaneArea.h"

/*
*	@brief コンストラクタ
*	@details 平面エリアクラスのコンストラクタ
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
PlaneArea::PlaneArea(CommonResources* resources)
	: m_pCommonResources(resources) // 共通リソースへのポインタ
	, m_pBasicEffect(nullptr) // ベーシックエフェクトへのポインタ
	, m_pBatch(nullptr) // プリミティブバッチへのポインタ
	, m_pInputLayout(nullptr) // 入力レイアウトへのポインタ
	, m_view(DirectX::SimpleMath::Matrix::Identity) // ビュー行列
	, m_projection(DirectX::SimpleMath::Matrix::Identity) // 射影行列
	, m_color(DirectX::SimpleMath::Color(1, 0, 1)) // 色
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスを取得
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ベーシックエフェクトを初期化
	m_pBasicEffect = std::make_unique<BasicEffect>(device);
	// 頂点カラーを有効にする
	m_pBasicEffect->SetVertexColorEnabled(true);
	// プリミティブバッチの作成
	m_pBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	// シェーダーバイトコードを宣言
	void const* shaderByteCode;
	// バイトコードの長さを取得
	size_t byteCodeLength;
	// ベーシックエフェクトからシェーダーバイトコードを取得
	m_pBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// 入力レイアウトの生成
	device->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		&m_pInputLayout);
}
/*
*	@brief デストラクタ
*	@details 平面エリアクラスのデストラクタ
*	@param なし
*	@return なし
*/
PlaneArea::~PlaneArea()
{
	// 終了処理
	Finalize();
}
/*
*	@brief 初期化
*	@details 平面エリアクラスの初期化を行う
*	@param なし
*	@return なし
*/
void PlaneArea::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 頂点設定
	Vector3 p0(-1, 0, -1);
	Vector3 p1(1, 0, -1);
	Vector3 p2(1, 0, 1);
	Vector3 p3(-1, 0, 1);
	// 色設定
	m_color = Color(1, 0, 0);
	// 各頂点を配列に登録
	m_debugPlaneVerticesPosition[0] = p0;
	m_debugPlaneVerticesPosition[1] = p1;
	m_debugPlaneVerticesPosition[2] = p2;
	m_debugPlaneVerticesPosition[3] = p3;
}
/*
*	@brief 更新
*	@details 平面エリアクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void PlaneArea::Update(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// DirectX::SimpleMath名前空間を使用
	using namespace DirectX::SimpleMath;
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
	Ray ray = ScreenPointToRay(mouseX, mouseY, screenWidth, screenHeight);
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
*	@details 平面エリアクラスの描画を行う
*	@param なし
*	@return なし
*/
void PlaneArea::Render()
{
	// 平面を描画
	DrawDebugLine(m_debugPlaneVerticesPosition);
}
/*
*	@brief 終了処理
*	@details 平面エリアクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void PlaneArea::Finalize()
{
	// 何もしない
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
DirectX::SimpleMath::Ray PlaneArea::ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight)
{
	using namespace DirectX::SimpleMath;
	// スクリーン座標をNDC(-1～1)の範囲に変換
	float px = ((2.0f * mouseX) / screenWidth - 1.0f);
	float py = (1.0f - (2.0f * mouseY) / screenHeight);
	// Clip空間でnear/far平面の座標を計算
	Vector3 rayStartNDC(px, py, 0.0f);
	Vector3 rayEndNDC(px, py, 1.0f);
	// 逆行列でワールド行列に変換
	Matrix invViewProj = (m_view * m_projection).Invert();
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
bool PlaneArea::RayIntersectPlane(const DirectX::SimpleMath::Ray& ray, const DirectX::SimpleMath::Plane& plane, DirectX::SimpleMath::Vector3& outIntersection)
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

void PlaneArea::DrawDebugLine(const DirectX::SimpleMath::Vector3 p[4])
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ベーシックエフェクトにワールド行列を設定
	m_pBasicEffect->SetWorld(Matrix::Identity);
	// ビュー行列を設定
	m_pBasicEffect->SetView(m_view);
	// 射影行列を設定
	m_pBasicEffect->SetProjection(m_projection);
	// 設定を適用
	m_pBasicEffect->Apply(context);
	// 入力レイアウトを設定
	context->IASetInputLayout(m_pInputLayout.Get());
	// 深度情報をなしにする
	context->OMSetDepthStencilState(m_pCommonResources->GetCommonStates()->DepthNone(), 0);
	// ワイヤーフレーム矩形を描画
	// 描画開始
	m_pBatch->Begin();
	// 各頂点を描画
	m_pBatch->DrawLine(VertexPositionColor(p[0], m_color), VertexPositionColor(p[1], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[1], m_color), VertexPositionColor(p[2], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[2], m_color), VertexPositionColor(p[3], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[3], m_color), VertexPositionColor(p[0], m_color));
	// 描画終了
	m_pBatch->End();
}
