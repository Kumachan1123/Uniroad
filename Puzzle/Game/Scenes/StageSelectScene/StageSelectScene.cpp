/*
*	@file StageSelectScene.cpp
*	@brief �X�e�[�W�I���V�[���N���X
*/
#include "pch.h"
#include "StageSelectScene.h"

/*
*	@brief �R���X�g���N�^
*	@details �X�e�[�W�I���V�[���N���X�̃R���X�g���N�^
*	@param sceneID �V�[��ID
*	@return �Ȃ�
*/
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pFixedCamera(nullptr) // �J�����ւ̃|�C���^
	, m_pMiniCharacterBase(nullptr) // �~�j�L�����x�[�X�ւ̃|�C���^
	, m_view() // �r���[�s��
	, m_projection() // �ˉe�s��
	, m_isChangeScene(false) // �V�[���ύX�t���O
	, m_bDrawDebugPlane(false) // �f�o�b�O���ʕ`��t���O
	, m_stageNumber(0) // �X�e�[�W�ԍ�
	, m_nextSceneID(sceneID) // ���̃V�[��ID
{
}
/*
*	@brief �f�X�g���N�^
*	@details �X�e�[�W�I���V�[���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
StageSelectScene::~StageSelectScene()
{
	// �������Ȃ�
}

/*
*	@brief ������
*	@details �X�e�[�W�I���V�[���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void StageSelectScene::Initialize(CommonResources* resources)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �J�������쐬����
	CreateCamera();
	// �f�o�C�X���\�[�X���擾����
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto context = deviceResources->GetD3DDeviceContext();
	const auto device = deviceResources->GetD3DDevice();
	const auto states = m_pCommonResources->GetCommonStates();
	// �e��r���[�|�[�g���쐬����
	CreateViewports();
	// �O���b�h�����쐬����
	m_pGridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	// �~�j�L�������쐬����
	m_pMiniCharacterBase = std::make_unique<MiniCharacterBase>(nullptr, Vector3(0.0f, 0.0f, 0.0f), 0.0f);
	// �~�j�L�����x�[�X��CSV�}�b�v��ݒ�
	m_pMiniCharacterBase->SetCSVMap(nullptr);
	// �~�j�L�����x�[�X��CSV�A�C�e����ݒ�
	m_pMiniCharacterBase->SetCSVItem(nullptr);
	// �~�j�L�����x�[�X�Ɏ��̃^�C����ݒ�
	m_pMiniCharacterBase->SetNextTiles(nullptr);
	// �~�j�L����������������
	m_pMiniCharacterBase->Initialize(m_pCommonResources);
	// �~�j�L�����x�[�X�Ƀ~�j�L�������A�^�b�`
	m_pMiniCharacterBase->Attach(std::make_unique<MiniCharacterSelectStage>(m_pMiniCharacterBase.get(), Vector3(0.0f, 0.0f, 0.0f), 0.0f));
	// �X�e�[�W�Z���N�g���쐬����
	m_pStageSelect = std::make_unique<StageSelect>(m_pCommonResources);
	// �X�e�[�W�Z���N�g������������
	m_pStageSelect->Initialize();
}
/*
*	@brief �X�V
*	@details �X�e�[�W�I���V�[���N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void StageSelectScene::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �Œ�J�����̍X�V
	m_pFixedCamera->Update();
	// �~�j�L�����̍X�V
	Vector3 position(0.0f, 0.0f, 0.0f);
	Quaternion angle(Quaternion::Identity);
	m_pMiniCharacterBase->Update(elapsedTime, position, angle);
	const auto& keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	if (keyState.Space)
	{
		// �X�y�[�X�L�[�������ꂽ��V�[���ύX�t���O�𗧂Ă�
		m_isChangeScene = true;
	}
	// �}�E�X���W�擾
	int mouseX = m_pCommonResources->GetInputManager()->GetMouseState().x;
	int mouseY = m_pCommonResources->GetInputManager()->GetMouseState().y;
	// �E�B���h�E�n���h�����擾
	const HWND hwnd = m_pCommonResources->GetDeviceResources()->GetWindow();
	// �E�B���h�E�T�C�Y�擾
	RECT rect;
	// �N���C�A���g�̈�T�C�Y���擾
	GetClientRect(hwnd, &rect);
	int screenWidth = rect.right;
	int screenHeight = rect.bottom;
	// ���C����
	Ray ray = ScreenPointToRay(mouseX, mouseY, screenWidth, screenHeight, m_view, m_projection);
	// ���ʂ̒�`
	Plane plane(Vector3(0.0f, 1.0f, 0.0f), 0.0f); // Y=0�̕���
	// ���C�ƕ��ʂ̌�������
	Vector3 intersection;
	bool hit = RayIntersectPlane(ray, plane, intersection);
	// --- �f�o�b�O���---
	const auto debugString = m_pCommonResources->GetDebugString();
	// ���C�Ɠ�����������\��
	debugString->AddString("Ray Hit:%s", hit ? "true" : "false");
}

/*
*	@brief �`��
*	@details �X�e�[�W�I���V�[���N���X�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::Render()
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �r���[�s����擾
	m_view = m_pFixedCamera->GetViewMatrix();
	// �X�e�[�W�Z���N�g�̕`��
	m_pStageSelect->Render(m_view, m_projection);
	// �~�j�L�����̕`��
	m_pMiniCharacterBase->Render(m_view, m_projection);
	// �O���b�h���̕`��
	m_pGridFloor->Render(context, m_view, m_projection);
	// ���_�ݒ�
	Vector3 p0(-1, 0, -1);
	Vector3 p1(1, 0, -1);
	Vector3 p2(1, 0, 1);
	Vector3 p3(-1, 0, 1);
	// �F�ݒ�
	Color color = Color(1, 0, 0);
	// �e���_��z��ɓo�^
	m_debugPlaneVerticesPosition[0] = p0;
	m_debugPlaneVerticesPosition[1] = p1;
	m_debugPlaneVerticesPosition[2] = p2;
	m_debugPlaneVerticesPosition[3] = p3;
	DrawDebugLine(m_debugPlaneVerticesPosition, color, m_view, m_projection);


	// --- �f�o�b�O���i��j ---
	const auto debugString = m_pCommonResources->GetDebugString();
}

/*
*	@brief �I��
*	@details �X�e�[�W�I���V�[���N���X�̏I�����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::Finalize()
{
}


/*
*	@brief �V�[���ύX
*	@details �V�[���ύX�̗L�����擾����
*	@param �Ȃ�
*	@return ���̃V�[��ID
*/
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		// �Q�[���I�[�o�[�V�[����
		return IScene::SceneID::PLAY;
	}
	// �V�[���ύX���Ȃ��ꍇ�������Ȃ�
	return IScene::SceneID::NONE;
}

/*
*	@brief �J�����Ɋւ���ݒ������
*	@details �X�e�[�W�I���V�[���N���X�̃J�����ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::CreateCamera()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �o�̓T�C�Y���擾����
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	//// �f�o�b�O�J�������쐬����
	//m_debugCamera = std::make_unique<mylib::DebugCamera>();
	//m_debugCamera->Initialize(rect.right * 0.7f, rect.bottom);
	// �Œ�J�������쐬����
	m_pFixedCamera = std::make_unique<FixedCamera>();
	m_pFixedCamera->Initialize((int)(rect.right), rect.bottom);

	// �ˉe�s����쐬����
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}
/*
*	@brief �e��r���[�|�[�g��ݒ肷��
*	@details �X�e�[�W�I���V�[���N���X�̃r���[�|�[�g�ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void StageSelectScene::CreateViewports()
{
}
/*
*	@brief �}�E�X���W���烏�[���h���C�𐶐�
*	@details �}�E�X���W���烏�[���h���C�𐶐�����
*	@param mouseX �}�E�X��X���W
*	@param mouseY �}�E�X��Y���W
*	@param screenWidth ��ʂ̕�
*	@param screenHeight ��ʂ̍���
*	@param view �r���[�s��
*	@param projection �ˉe�s��
*	@return �������ꂽ���C
*/
DirectX::SimpleMath::Ray StageSelectScene::ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	using namespace DirectX::SimpleMath;
	// �X�N���[�����W��NDC(-1�`1)�͈̔͂ɕϊ�
	float px = ((2.0f * mouseX) / screenWidth - 1.0f);
	float py = (1.0f - (2.0f * mouseY) / screenHeight);
	// Clip��Ԃ�near/far���ʂ̍��W���v�Z
	Vector3 rayStartNDC(px, py, 0.0f);
	Vector3 rayEndNDC(px, py, 1.0f);
	// �t�s��Ń��[���h�s��ɕϊ�
	Matrix invViewProj = (view * projection).Invert();
	Vector3 rayStartWorld = Vector3::Transform(rayStartNDC, invViewProj);
	Vector3 rayEndWorld = Vector3::Transform(rayEndNDC, invViewProj);
	// ���C�𐶐�
	Vector3 dir = (rayEndWorld - rayStartWorld);
	dir.Normalize();
	return Ray(rayStartWorld, dir);

}
/*
*	@brief Plane�ƃ��C�̌�������
*	@details ���C�ƕ��ʂ̌���������s��
*	@param ray ���C
*	@param planePoint ���ʏ�̓_
*	@param planeNormal ���ʂ̖@��
*	@param outIntersection �����_�̏o��
*	@return �����������ǂ����̐^�U�l
*/
bool StageSelectScene::RayIntersectPlane(const DirectX::SimpleMath::Ray& ray, const DirectX::SimpleMath::Plane& plane, DirectX::SimpleMath::Vector3& outIntersection)
{
	using namespace DirectX::SimpleMath;
	const Vector3& n = plane.Normal();
	float d = plane.D();

	float denom = ray.direction.Dot(n);
	if (fabs(denom) < 1e-6f) return false; // ���s

	// ���ʏ�̓_�� -d * n �Ŏ���iax+by+cz+d=0���j
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

		// ���̓��C�A�E�g�̐���
		void const* shaderByteCode;
		size_t byteCodeLength;
		s_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
		device->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&s_inputLayout);
	}

	// �s��ݒ�
	s_effect->SetWorld(Matrix::Identity);
	s_effect->SetView(m_view);
	s_effect->SetProjection(m_projection);

	s_effect->Apply(context);
	context->IASetInputLayout(s_inputLayout.Get());

	// ���C���[�t���[����`��`��
	s_batch->Begin();
	s_batch->DrawLine(VertexPositionColor(p[0], color), VertexPositionColor(p[1], color));
	s_batch->DrawLine(VertexPositionColor(p[1], color), VertexPositionColor(p[2], color));
	s_batch->DrawLine(VertexPositionColor(p[2], color), VertexPositionColor(p[3], color));
	s_batch->DrawLine(VertexPositionColor(p[3], color), VertexPositionColor(p[0], color));
	s_batch->End();
}