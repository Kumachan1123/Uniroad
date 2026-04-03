#include "pch.h"
#include "Android.h"

Android::Android()
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 時間
	, m_world(DirectX::SimpleMath::Matrix::Identity) // ワールド行列
	, m_position(DirectX::SimpleMath::Vector3::Zero) //座標
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // 速度
	, m_isGrounded(true) // 接地状態
	, m_wasJumpKeyDown(false) // 前フレームのジャンプキー状態
{}

void Android::Initialize(CommonResources* resources)
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// 共通リソースを保存する
	m_pCommonResources = resources;

	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
	// deviceを取得する
	ID3D11Device* device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	std::wstring name = L"Android";
	// ファイルパス
	std::wstring filePath = L"Resources/SDKMeshes/" + name + L"/" + name + L".sdkmesh";
	// フォルダパス
	std::wstring folderPath = L"Resources/SDKMeshes/" + name;

	// フォルダパスを指定する
	m_pEffectFactory->SetDirectory(folderPath.c_str());
	// SDKメッシュを作成する
	m_pModel = DirectX::Model::CreateFromSDKMESH(device, filePath.c_str(), *m_pEffectFactory, DirectX::ModelLoader_CounterClockwise);

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

void Android::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 時間を更新する
	m_time += elapsedTime;

	// 左右移動
	Move(elapsedTime);
	// ジャンプ
	Jump(elapsedTime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(Vector3(-1, 1, 1));
	world *= Matrix::CreateRotationY(XMConvertToRadians(0));
	world *= Matrix::CreateTranslation(m_position);
	m_world = world;
}

void Android::Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj)
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


	if (!m_pModel->bones.empty() && m_boneTransforms)
	{
		m_pModel->Draw(context, *states, m_pModel->bones.size(), m_boneTransforms.get(), m_world, view, proj);
	}
	else
	{
		m_pModel->Draw(context, *states, m_world, view, proj);
	}
}

void Android::Finalize()
{}

void Android::Move(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// WASDキーで移動する
	auto keyboard = m_pCommonResources->GetInputManager()->GetKeyboardState();
	// 移動速度
	float speed = 5.0f;

	Vector3 move = Vector3::Zero;
	// 左右移動
	if (keyboard.IsKeyDown(Keyboard::Keys::A))
	{
		move += Vector3::Left * speed * elapsedTime;
	}
	if (keyboard.IsKeyDown(Keyboard::Keys::D))
	{
		move += Vector3::Right * speed * elapsedTime;
	}
	// 前後移動
	if (keyboard.IsKeyDown(Keyboard::Keys::W))
	{
		move += Vector3::Forward * speed * elapsedTime;
	}
	if (keyboard.IsKeyDown(Keyboard::Keys::S))
	{
		move += Vector3::Backward * speed * elapsedTime;
	}
	m_position += move;
}

void Android::Jump(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto keyboard = m_pCommonResources->GetInputManager()->GetKeyboardState();

	const float gravity = 20.0f;
	const float jumpSpeed = 15.5f;
	const bool isJumpKeyDown = keyboard.IsKeyDown(Keyboard::Keys::Space);

	if (m_isGrounded && isJumpKeyDown && !m_wasJumpKeyDown)
	{
		m_velocity.y = jumpSpeed;
		m_isGrounded = false;
	}

	if (!m_isGrounded)
	{
		m_velocity.y -= gravity * elapsedTime;
		m_position.y += m_velocity.y * elapsedTime;

		if (m_position.y <= 0.0f)
		{
			m_position.y = 0.0f;
			m_velocity.y = 0.0f;
			m_isGrounded = true;
		}
	}

	m_wasJumpKeyDown = isJumpKeyDown;
}
