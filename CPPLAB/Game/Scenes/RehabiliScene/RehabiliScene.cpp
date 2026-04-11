/*
*	@file RehabiliScene.cpp
*	@brief ゲームづくりのリハビリ用シーンクラス
*/

#include "pch.h"
#include "RehabiliScene.h"
/*
*	@brief コンストラクタ
*	@details ゲームづくりのリハビリ用シーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
RehabiliScene::RehabiliScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pTPCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
{}
/*
*	@brief デストラクタ
*	@details ゲームづくりのリハビリ用シーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
RehabiliScene::~RehabiliScene()
{}
/*
*	@brief 初期化
*	@details ゲームづくりのリハビリ用シーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void RehabiliScene::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを保存する。
	// 以降のテクスチャ取得やデバイス参照はこのポインタ経由で統一する。
	m_pCommonResources = resources;
	auto deviceResources = m_pCommonResources->GetDeviceResources();

	// カメラを生成して、ビュー・射影の準備を完了させる。
	CreateCamera();

	// ぷよマネージャーを生成して、初期化する。
	m_pPuyoManager = std::make_unique<PuyoManager>();
	m_pPuyoManager->Initialize(m_pCommonResources);




}
/*
*	@brief 更新
*	@details ゲームづくりのリハビリ用シーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void RehabiliScene::Update(float elapsedTime)
{

	// 次の落下ペア生成用タイマーを進める。
	m_time += elapsedTime;


	// カメラ更新（ゲームロジックとは独立）。
	m_pTPCamera->SetTime(m_time);
	m_pTPCamera->Update();
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	m_view = m_pTPCamera->GetViewMatrix();

	// シーン遷移入力。
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	if (keyState.Enter)m_isChangeScene = true;

	// ぷよマネージャーの更新。
	m_pPuyoManager->Update(elapsedTime);


}
/*
*	@brief 描画
*	@details ゲームづくりのリハビリ用シーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto states = m_pCommonResources->GetCommonStates();
	auto context = deviceResources->GetD3DDeviceContext();

	// ぷよマネージャーの描画。
	m_pPuyoManager->Render();
}
/*
*	@brief 終了
*	@details ゲームづくりのリハビリ用シーンクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::Finalize()
{

}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID RehabiliScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// ステージセレクトへ
	return IScene::SceneID::TEST;
}
/*
* 	@brief カメラに関する設定をする
*	@details カメラの作成と設定を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::CreateCamera()
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 固定カメラを作成する
	m_pTPCamera = std::make_unique<TPCamera>();
	// 固定カメラを初期化する
	m_pTPCamera->SetCommonResources(m_pCommonResources);
	m_pTPCamera->Initialize((int)(rect.right), rect.bottom);
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
	m_pTPCamera->SetProjectionMatrix(m_projection);
}
/*
*	@brief SDKメッシュを作る
*	@details SDKメッシュの作成を行う
*	@param name SDKメッシュの名前
*	@return なし
*/
void RehabiliScene::CreateSDKMesh(std::wstring name)
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// deviceを取得する
	ID3D11Device* device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();

	// ファイルパス
	std::wstring filePath = L"Resources/SDKMeshes/" + name + L"/" + name + L".sdkmesh";
	// フォルダパス
	std::wstring folderPath = L"Resources/SDKMeshes/" + name;



}
