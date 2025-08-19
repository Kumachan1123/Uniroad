/*
*	@file Utility.cpp
*	@brief �p�[�e�B�N�����[�e�B���e�B�N���X�̎���
*/
#include "pch.h"
#include "Utility.h"
/*
*	@brief �R���X�g���N�^
*	@details �p�[�e�B�N�����[�e�B���e�B�N���X�̏��������s��
*	@param params �p�[�e�B�N���̃p�����[�^�[
*	@return �Ȃ�
*/
Utility::Utility(const ParticleParams& params)
	: m_position(params.pos) // ���W
	, m_velocity(params.velocity) // ���x
	, m_accele(params.accele) // �����x
	, m_rotateAccele(params.rotateAccele) // ��]�����x
	, m_rotate(params.rotate) // ��]
	, m_startScale(params.startScale) // �����T�C�Y
	, m_endScale(params.endScale) // �ŏI�I�ȃT�C�Y
	, m_startColor(params.startColor) // �����J���[
	, m_endColor(params.endColor) // �ŏI�I�ȃJ���[
	, m_type(params.type) // �^�C�v
	, m_life(params.life) // ��������
	, m_startLife(params.life) // ������������
	, m_elapsedTime(0.0f) // �o�ߎ���
{
}
/*
*	@brief �f�X�g���N�^
*	@details �p�[�e�B�N�����[�e�B���e�B�N���X�̔j���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Utility::~Utility() {/* do nothing. */ }
/*
*	@brief �X�V
*	@details �p�[�e�B�N���̍X�V�������s��
*	@param elapsedTime �t���[������
*	@return �X�V��̃p�[�e�B�N�����������Ă��邩�ǂ��� false: �������Ă��Ȃ�, true: �������Ă���
*/
bool Utility::Update(float elapsedTime)
{
	// DirectX::SimpleMath::Vector3��DirectX::SimpleMath::Color���g�p���邽�߂̖��O���
	using namespace DirectX;
	// �X�P�[���̕��
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// �F�̕��
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// �����x�̓K�p
	m_velocity += m_accele * elapsedTime;
	// ���W�X�V
	m_position += m_velocity * elapsedTime;
	// ���C�t�̌���
	m_life -= elapsedTime;
	// ���C�t��0�ȉ��ɂȂ�����false��Ԃ��A���C�t��0���傫���ꍇ��true��Ԃ�
	if (m_life < 0.0f)return false;
	else return true;
}
