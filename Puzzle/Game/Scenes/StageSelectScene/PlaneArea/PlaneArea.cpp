/*
*	@file PlaneArea.cpp
*	@brief ���ʃG���A�N���X
*/
#include "pch.h"
#include "PlaneArea.h"

/*
*	@brief �R���X�g���N�^
*	@details ���ʃG���A�N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
PlaneArea::PlaneArea(CommonResources* resources)
	: m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pBasicEffect(nullptr) // �x�[�V�b�N�G�t�F�N�g�ւ̃|�C���^
	, m_pBatch(nullptr) // �v���~�e�B�u�o�b�`�ւ̃|�C���^
	, m_pInputLayout(nullptr) // ���̓��C�A�E�g�ւ̃|�C���^
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�s��
	, m_projection(DirectX::SimpleMath::Matrix::Identity) // �ˉe�s��
	, m_color(DirectX::SimpleMath::Color(1, 0, 1)) // �F
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X���擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �x�[�V�b�N�G�t�F�N�g��������
	m_pBasicEffect = std::make_unique<BasicEffect>(device);
	// ���_�J���[��L���ɂ���
	m_pBasicEffect->SetVertexColorEnabled(true);
	// �v���~�e�B�u�o�b�`�̍쐬
	m_pBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	// �V�F�[�_�[�o�C�g�R�[�h��錾
	void const* shaderByteCode;
	// �o�C�g�R�[�h�̒������擾
	size_t byteCodeLength;
	// �x�[�V�b�N�G�t�F�N�g����V�F�[�_�[�o�C�g�R�[�h���擾
	m_pBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// ���̓��C�A�E�g�̐���
	device->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		&m_pInputLayout);
}
/*
*	@brief �f�X�g���N�^
*	@details ���ʃG���A�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
PlaneArea::~PlaneArea()
{
	// �I������
	Finalize();
}
/*
*	@brief ������
*	@details ���ʃG���A�N���X�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlaneArea::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_�ݒ�
	Vector3 p0(-1, 0, -1);
	Vector3 p1(1, 0, -1);
	Vector3 p2(1, 0, 1);
	Vector3 p3(-1, 0, 1);
	// �F�ݒ�
	m_color = Color(1, 0, 0);
	// �e���_��z��ɓo�^
	m_debugPlaneVerticesPosition[0] = p0;
	m_debugPlaneVerticesPosition[1] = p1;
	m_debugPlaneVerticesPosition[2] = p2;
	m_debugPlaneVerticesPosition[3] = p3;
}
/*
*	@brief �X�V
*	@details ���ʃG���A�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void PlaneArea::Update(float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// DirectX::SimpleMath���O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
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
	Ray ray = ScreenPointToRay(mouseX, mouseY, screenWidth, screenHeight);
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
*	@details ���ʃG���A�N���X�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlaneArea::Render()
{
	// ���ʂ�`��
	DrawDebugLine(m_debugPlaneVerticesPosition);
}
/*
*	@brief �I������
*	@details ���ʃG���A�N���X�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void PlaneArea::Finalize()
{
	// �������Ȃ�
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
DirectX::SimpleMath::Ray PlaneArea::ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight)
{
	using namespace DirectX::SimpleMath;
	// �X�N���[�����W��NDC(-1�`1)�͈̔͂ɕϊ�
	float px = ((2.0f * mouseX) / screenWidth - 1.0f);
	float py = (1.0f - (2.0f * mouseY) / screenHeight);
	// Clip��Ԃ�near/far���ʂ̍��W���v�Z
	Vector3 rayStartNDC(px, py, 0.0f);
	Vector3 rayEndNDC(px, py, 1.0f);
	// �t�s��Ń��[���h�s��ɕϊ�
	Matrix invViewProj = (m_view * m_projection).Invert();
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
bool PlaneArea::RayIntersectPlane(const DirectX::SimpleMath::Ray& ray, const DirectX::SimpleMath::Plane& plane, DirectX::SimpleMath::Vector3& outIntersection)
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

void PlaneArea::DrawDebugLine(const DirectX::SimpleMath::Vector3 p[4])
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �x�[�V�b�N�G�t�F�N�g�Ƀ��[���h�s���ݒ�
	m_pBasicEffect->SetWorld(Matrix::Identity);
	// �r���[�s���ݒ�
	m_pBasicEffect->SetView(m_view);
	// �ˉe�s���ݒ�
	m_pBasicEffect->SetProjection(m_projection);
	// �ݒ��K�p
	m_pBasicEffect->Apply(context);
	// ���̓��C�A�E�g��ݒ�
	context->IASetInputLayout(m_pInputLayout.Get());
	// �[�x�����Ȃ��ɂ���
	context->OMSetDepthStencilState(m_pCommonResources->GetCommonStates()->DepthNone(), 0);
	// ���C���[�t���[����`��`��
	// �`��J�n
	m_pBatch->Begin();
	// �e���_��`��
	m_pBatch->DrawLine(VertexPositionColor(p[0], m_color), VertexPositionColor(p[1], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[1], m_color), VertexPositionColor(p[2], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[2], m_color), VertexPositionColor(p[3], m_color));
	m_pBatch->DrawLine(VertexPositionColor(p[3], m_color), VertexPositionColor(p[0], m_color));
	// �`��I��
	m_pBatch->End();
}
