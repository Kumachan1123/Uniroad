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
	// 共通リソースを保存する
	m_pCommonResources = resources;
	auto deviceResources = m_pCommonResources->GetDeviceResources();

	// カメラに関する設定をする
	CreateCamera();
	// モデルを受け取る
	//m_pModel = m_pCommonResources->GetModelManager()->GetModel("Medal");
	//CreateSDKMesh(L"Android");
	//m_pAndroid = std::make_unique<Android>();
	//m_pAndroid->Initialize(m_pCommonResources);
	// ぷよのグリッドを作成する
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyoGrid[col][row] = std::make_unique<PuyoGrid>();
			m_pPuyoGrid[col][row]->SetPosition(Vector2(0.4f + col * 0.05f, 0.045f + row * 0.0826f));
			m_pPuyoGrid[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
		}
	}
	// ぷよぷよを作成する
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			int randomColor = KumachiLib::GenerateRandomMultiplier(0, 4); // 0から4のランダムな整数を生成
			m_pPuyo[col][row] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(randomColor));
			m_pPuyo[col][row]->SetPosition(Vector2(0.4f + col * 0.05f, 0.045f + row * 0.0826f));
			m_pPuyo[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
		}
	}
	//m_pPuyo = std::make_unique<Puyo>(Puyo::PuyoColor::Green);
	//m_pPuyo->SetPosition(Vector2(0.4, 0.1));
	//m_pPuyo->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
}
/*
*	@brief 更新
*	@details ゲームづくりのリハビリ用シーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void RehabiliScene::Update(float elapsedTime)
{
	// 時間を更新する
	m_time += elapsedTime;
	m_pTPCamera->SetTime(m_time);
	// 三人称カメラの更新
	m_pTPCamera->Update();
	// デバッグカメラを更新する
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// ビュー行列を取得
	m_view = m_pTPCamera->GetViewMatrix();
	// シーン変更の入力をチェックする
	// キーボード入力を取得
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// スペースキーが押されたら（何らかのフラグがたったら）
	if (keyState.Enter)m_isChangeScene = true;
	// ぷよぷよの更新
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyo[col][row]->Update(elapsedTime);
		}
	}
	//m_pPuyo->Update(elapsedTime);
	//m_pAndroid->Update(elapsedTime);

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
	// デバイスリソースを取得する
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	// コモンステートを取得する
	const auto states = m_pCommonResources->GetCommonStates();
	// コンテキストを取得する
	auto context = deviceResources->GetD3DDeviceContext();
	//// モデルの描画
	//m_pAndroid->Render(m_view, m_projection);
	// ぷよのグリッドの描画
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyoGrid[col][row]->Render();
		}
	}

	// ぷよぷよの描画
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyo[col][row]->Render();
		}
	}
	//m_pPuyo->Render();
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
