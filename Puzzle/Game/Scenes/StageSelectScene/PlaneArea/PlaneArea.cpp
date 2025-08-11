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
	, m_hitPlaneIndex(PlaneArea::NO_HIT_PLANE_INDEX) // 当たった平面の番号
	, m_isHitPlane(false) // 何らかの平面と当たっているか
{

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
	// 当たり判定を初期化
	m_isHitPlane = false;
	// 当たった平面の番号を-1にする
	SetHitPlaneIndex(-1);
	// 平面の頂点配列をループして交差判定
	for (int i = 0; i < m_debugPlaneVerticesPosition.size(); i++)
	{
		// レイと平面の交差判定を行い、当たっていたらヒットフラグをtrueにする
		if (RayIntersectPlane(i, ray, plane, m_debugPlaneVerticesPosition[i], intersection))m_isHitPlane = true;
	}

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
	for (int i = 0; i < m_debugPlaneVerticesPosition.size(); i++)
		DrawDebugLine(m_debugPlaneVerticesPosition[i], m_debugPlaneVerticesColor[i]);
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
*	@brief Planeとレイの交差判定（矩形領域対応版）
*	@details レイと平面の交差判定を行い、交点が矩形上か判定する
*	@param ray レイ
*	@param plane 平面
*	@param rectVertices 矩形の4頂点（順番は一周するように）
*	@param outIntersection 交差点の出力
*	@return 交差したかどうかの真偽値
*/
bool PlaneArea::RayIntersectPlane(
	int index,
	const DirectX::SimpleMath::Ray& ray,
	const DirectX::SimpleMath::Plane& plane,
	const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
	DirectX::SimpleMath::Vector3& outIntersection)
{
	// DirectX::SimpleMath名前空間を使用
	using namespace DirectX::SimpleMath;
	// 平面との交点計算
	// 平面の法線を取得
	const Vector3& n = plane.Normal();
	// 平面のD値を取得
	float d = plane.D();
	// レイの方向ベクトルと平面の法線の内積を計算
	float denom = ray.direction.Dot(n);
	// 交差判定
	if (fabs(denom) < 1e-6f) return false;
	// 平面のD値を使って平面上の点を計算
	Vector3 planePoint = -d * n;
	// レイと平面の交点を計算
	float t = (planePoint - ray.position).Dot(n) / denom;
	// 交点がレイの前方にあるか判定(falseの場合は平面の裏側)
	if (t < 0) return false;
	// 交点の位置を計算
	outIntersection = ray.position + t * ray.direction;
	// 交点が矩形上か判定
	// 三角形2つに分割して両方で判定
	if (rectVertices.size() >= 4)
	{
		// 矩形の4頂点を取得
		const Vector3& v0 = rectVertices[0];
		const Vector3& v1 = rectVertices[1];
		const Vector3& v2 = rectVertices[2];
		const Vector3& v3 = rectVertices[3];
		// 2Dで内包判定
		auto PointInTriangle = [](const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c)
			{
				// 三角形の頂点a, b, cと点pが与えられたとき、点pが三角形内にあるかを判定
				Vector3 v0 = c - a;// 三角形の辺aからcへのベクトル
				Vector3 v1 = b - a;// 三角形の辺aからbへのベクトル
				Vector3 v2 = p - a;// 三角形の頂点aから点pへのベクトル
				// ベクトルの内積を計算
				float dot00 = v0.Dot(v0);// 辺aからcへのベクトルの長さの二乗
				float dot01 = v0.Dot(v1);// 辺aからcへのベクトルと辺aからbへのベクトルの内積
				float dot02 = v0.Dot(v2);// 辺aからcへのベクトルと点pへのベクトルの内積
				float dot11 = v1.Dot(v1);// 辺aからbへのベクトルの長さの二乗
				float dot12 = v1.Dot(v2);// 辺aからbへのベクトルと点pへのベクトルの内積
				// バリデーション
				float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);// 逆数を計算
				float u = (dot11 * dot02 - dot01 * dot12) * invDenom;// uパラメータを計算
				float v = (dot00 * dot12 - dot01 * dot02) * invDenom;// vパラメータを計算
				// u, vが0以上かつu + vが1以下ならば、点pは三角形内にある
				return (u >= 0) && (v >= 0) && (u + v <= 1);
			};
		// rectVertices[0,1,2]三角形 or [0,2,3]三角形のどちらかに入ってるか判定
		if (PointInTriangle(outIntersection, v0, v1, v2) ||// rectVertices[0,1,2]三角形
			PointInTriangle(outIntersection, v0, v2, v3))// rectVertices[0,2,3]三角形
		{
			// 当たった平面の色を白にする
			m_debugPlaneVerticesColor[index] = Color(1, 1, 1);
			// 当たった平面の番号を記録
			SetHitPlaneIndex(index);
			// 矩形内にある
			return true;
		}
	}
	// 当たってない平面の色は赤に戻す
	m_debugPlaneVerticesColor[index] = Color(1, 0, 0);

	// 矩形の頂点が4つ未満の場合は、矩形として扱わない
	return false;
}
/*
*	@brief ワイヤーフレーム矩形を描画
*	@details ワイヤーフレーム矩形を描画する
*	@param vertices 頂点の配列（4つの頂点）
*	@param color 描画する色
*	@return なし
*/
void PlaneArea::DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const DirectX::SimpleMath::Color& color)
{
	// DirectXとSimpleMath名前空間を使用
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストを取得
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
	m_pBatch->DrawLine(VertexPositionColor(vertices[0], color), VertexPositionColor(vertices[1], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[1], color), VertexPositionColor(vertices[2], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[2], color), VertexPositionColor(vertices[3], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[3], color), VertexPositionColor(vertices[0], color));
	// 描画終了
	m_pBatch->End();
}
/*
*	@brief 平面の位置を取得
*	@details 指定したインデックスの平面の位置を取得する
*	@param index 平面のインデックス
*	@return 指定したインデックスの平面の位置
*/
DirectX::SimpleMath::Vector3 PlaneArea::GetPlanePosition(int index) const
{
	if (index < 0)
		return m_positions[0]; // インデックスが負の場合は最初の位置を返す
	// 指定したインデックスの平面の位置を返す
	return m_positions[index];
}
