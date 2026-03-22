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
	, m_pFixedCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
{
}
/*
*	@brief デストラクタ
*	@details ゲームづくりのリハビリ用シーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
RehabiliScene::~RehabiliScene()
{
}
/*
*	@brief 初期化
*	@details ゲームづくりのリハビリ用シーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void RehabiliScene::Initialize(CommonResources* resources)
{
	// 共通リソースを保存する
	m_pCommonResources = resources;

	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
	// カメラに関する設定をする
	CreateCamera();
	// モデルを受け取る
	//m_pModel = m_pCommonResources->GetModelManager()->GetModel("Medal");
	CreateSDKMesh(L"Wolf");
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
	// 固定カメラの更新
	m_pFixedCamera->Update();
	// デバッグカメラを更新する
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	// ビュー行列を取得
	m_view = m_debugCamera->GetViewMatrix();
	// シーン変更の入力をチェックする
	// キーボード入力を取得
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// スペースキーが押されたら（何らかのフラグがたったら）
	if (keyState.Space)
	{
		m_isChangeScene = true;
	}

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
	// モデルの描画
	if (!m_pModel) return;

	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(1.f);

	if (!m_pModel->bones.empty() && m_boneTransforms)
	{
		m_pModel->Draw(context, *states, m_pModel->bones.size(), m_boneTransforms.get(), world, m_view, m_projection);
	}
	else
	{
		m_pModel->Draw(context, *states, world, m_view, m_projection);
	}
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

	// フォルダパスを指定する
	m_pEffectFactory->SetDirectory(folderPath.c_str());
	// SDKメッシュを作成する
	m_pModel = DirectX::Model::CreateFromSDKMESH(device, filePath.c_str(), *m_pEffectFactory, DirectX::ModelLoader_Clockwise);

	if (m_pModel && !m_pModel->bones.empty())
	{
		m_boneTransforms = DirectX::ModelBone::MakeArray(m_pModel->bones.size());
		m_pModel->CopyAbsoluteBoneTransformsTo(m_pModel->bones.size(), m_boneTransforms.get());
	}
	else
	{
		m_boneTransforms.reset();
	}

}
