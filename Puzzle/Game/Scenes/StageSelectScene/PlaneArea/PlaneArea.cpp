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
	, m_hitPlaneIndex(PlaneArea::NO_HIT_PLANE_INDEX) // �����������ʂ̔ԍ�
	, m_isHitPlane(false) // ���炩�̕��ʂƓ������Ă��邩
{

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
	// �����蔻���������
	m_isHitPlane = false;
	// �����������ʂ̔ԍ���-1�ɂ���
	SetHitPlaneIndex(-1);
	// ���ʂ̒��_�z������[�v���Č�������
	for (int i = 0; i < m_debugPlaneVerticesPosition.size(); i++)
	{
		// ���C�ƕ��ʂ̌���������s���A�������Ă�����q�b�g�t���O��true�ɂ���
		if (RayIntersectPlane(i, ray, plane, m_debugPlaneVerticesPosition[i], intersection))m_isHitPlane = true;
	}

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
	for (int i = 0; i < m_debugPlaneVerticesPosition.size(); i++)
		DrawDebugLine(m_debugPlaneVerticesPosition[i], m_debugPlaneVerticesColor[i]);
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
*	@brief Plane�ƃ��C�̌�������i��`�̈�Ή��Łj
*	@details ���C�ƕ��ʂ̌���������s���A��_����`�ォ���肷��
*	@param ray ���C
*	@param plane ����
*	@param rectVertices ��`��4���_�i���Ԃ͈������悤�Ɂj
*	@param outIntersection �����_�̏o��
*	@return �����������ǂ����̐^�U�l
*/
bool PlaneArea::RayIntersectPlane(
	int index,
	const DirectX::SimpleMath::Ray& ray,
	const DirectX::SimpleMath::Plane& plane,
	const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
	DirectX::SimpleMath::Vector3& outIntersection)
{
	// DirectX::SimpleMath���O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���ʂƂ̌�_�v�Z
	// ���ʂ̖@�����擾
	const Vector3& n = plane.Normal();
	// ���ʂ�D�l���擾
	float d = plane.D();
	// ���C�̕����x�N�g���ƕ��ʂ̖@���̓��ς��v�Z
	float denom = ray.direction.Dot(n);
	// ��������
	if (fabs(denom) < 1e-6f) return false;
	// ���ʂ�D�l���g���ĕ��ʏ�̓_���v�Z
	Vector3 planePoint = -d * n;
	// ���C�ƕ��ʂ̌�_���v�Z
	float t = (planePoint - ray.position).Dot(n) / denom;
	// ��_�����C�̑O���ɂ��邩����(false�̏ꍇ�͕��ʂ̗���)
	if (t < 0) return false;
	// ��_�̈ʒu���v�Z
	outIntersection = ray.position + t * ray.direction;
	// ��_����`�ォ����
	// �O�p�`2�ɕ������ė����Ŕ���
	if (rectVertices.size() >= 4)
	{
		// ��`��4���_���擾
		const Vector3& v0 = rectVertices[0];
		const Vector3& v1 = rectVertices[1];
		const Vector3& v2 = rectVertices[2];
		const Vector3& v3 = rectVertices[3];
		// 2D�œ����
		auto PointInTriangle = [](const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c)
			{
				// �O�p�`�̒��_a, b, c�Ɠ_p���^����ꂽ�Ƃ��A�_p���O�p�`���ɂ��邩�𔻒�
				Vector3 v0 = c - a;// �O�p�`�̕�a����c�ւ̃x�N�g��
				Vector3 v1 = b - a;// �O�p�`�̕�a����b�ւ̃x�N�g��
				Vector3 v2 = p - a;// �O�p�`�̒��_a����_p�ւ̃x�N�g��
				// �x�N�g���̓��ς��v�Z
				float dot00 = v0.Dot(v0);// ��a����c�ւ̃x�N�g���̒����̓��
				float dot01 = v0.Dot(v1);// ��a����c�ւ̃x�N�g���ƕ�a����b�ւ̃x�N�g���̓���
				float dot02 = v0.Dot(v2);// ��a����c�ւ̃x�N�g���Ɠ_p�ւ̃x�N�g���̓���
				float dot11 = v1.Dot(v1);// ��a����b�ւ̃x�N�g���̒����̓��
				float dot12 = v1.Dot(v2);// ��a����b�ւ̃x�N�g���Ɠ_p�ւ̃x�N�g���̓���
				// �o���f�[�V����
				float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);// �t�����v�Z
				float u = (dot11 * dot02 - dot01 * dot12) * invDenom;// u�p�����[�^���v�Z
				float v = (dot00 * dot12 - dot01 * dot02) * invDenom;// v�p�����[�^���v�Z
				// u, v��0�ȏォ��u + v��1�ȉ��Ȃ�΁A�_p�͎O�p�`���ɂ���
				return (u >= 0) && (v >= 0) && (u + v <= 1);
			};
		// rectVertices[0,1,2]�O�p�` or [0,2,3]�O�p�`�̂ǂ��炩�ɓ����Ă邩����
		if (PointInTriangle(outIntersection, v0, v1, v2) ||// rectVertices[0,1,2]�O�p�`
			PointInTriangle(outIntersection, v0, v2, v3))// rectVertices[0,2,3]�O�p�`
		{
			// �����������ʂ̐F�𔒂ɂ���
			m_debugPlaneVerticesColor[index] = Color(1, 1, 1);
			// �����������ʂ̔ԍ����L�^
			SetHitPlaneIndex(index);
			// ��`���ɂ���
			return true;
		}
	}
	// �������ĂȂ����ʂ̐F�͐Ԃɖ߂�
	m_debugPlaneVerticesColor[index] = Color(1, 0, 0);

	// ��`�̒��_��4�����̏ꍇ�́A��`�Ƃ��Ĉ���Ȃ�
	return false;
}
/*
*	@brief ���C���[�t���[����`��`��
*	@details ���C���[�t���[����`��`�悷��
*	@param vertices ���_�̔z��i4�̒��_�j
*	@param color �`�悷��F
*	@return �Ȃ�
*/
void PlaneArea::DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const DirectX::SimpleMath::Color& color)
{
	// DirectX��SimpleMath���O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
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
	m_pBatch->DrawLine(VertexPositionColor(vertices[0], color), VertexPositionColor(vertices[1], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[1], color), VertexPositionColor(vertices[2], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[2], color), VertexPositionColor(vertices[3], color));
	m_pBatch->DrawLine(VertexPositionColor(vertices[3], color), VertexPositionColor(vertices[0], color));
	// �`��I��
	m_pBatch->End();
}
/*
*	@brief ���ʂ̈ʒu���擾
*	@details �w�肵���C���f�b�N�X�̕��ʂ̈ʒu���擾����
*	@param index ���ʂ̃C���f�b�N�X
*	@return �w�肵���C���f�b�N�X�̕��ʂ̈ʒu
*/
DirectX::SimpleMath::Vector3 PlaneArea::GetPlanePosition(int index) const
{
	if (index < 0)
		return m_positions[0]; // �C���f�b�N�X�����̏ꍇ�͍ŏ��̈ʒu��Ԃ�
	// �w�肵���C���f�b�N�X�̕��ʂ̈ʒu��Ԃ�
	return m_positions[index];
}
