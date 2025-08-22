/*
*	@file ShadowMapLight.cpp
*	@brief �V���h�E�}�b�v�p���C�g�N���X
*/
#include "pch.h"
#include "ShadowMapLight.h"
// �V���h�E�}�b�v�̃T�C�Y
const int ShadowMapLight::SHADOWMAP_SIZE = 1024;
/*
*	@brief �R���X�g���N�^
*	@details �V���h�E�}�b�v�p���C�g�N���X�̃R���X�g���N�^
*	@param commonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
ShadowMapLight::ShadowMapLight(CommonResources* commonResources)
	: m_pCommonResources(commonResources)// ���ʃ��\�[�X
	, m_pDevice(commonResources->GetDeviceResources()->GetD3DDevice())// Direct3D�f�o�C�X
	, m_pVS(nullptr)// ���_�V�F�[�_
	, m_pPS(nullptr)// �s�N�Z���V�F�[�_
	, m_pConstantBuffer(nullptr)// �萔�o�b�t�@
	, m_pShadowMapRT(nullptr)// �����_�[�e�N�X�`��
	, m_pShadowMapDS(nullptr)// �[�x�X�e���V��
	, m_pShadowMapSampler(nullptr)// �V���h�E�}�b�v�̃T���v���[
	, m_lightTheta(100.0f)// ���C�g�̉�p (100�x�������l�Ƃ��Đݒ�
	, m_viewport(commonResources->GetDeviceResources()->GetScreenViewport()) // �r���[�|�[�g
{
	// ���O���
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���C�g�̍��W
	m_lightPosition = Vector3(0.0f, 10.0f, 0.0f);
	// ���C�g�̉�]
	m_lightQuaternion = Quaternion::CreateFromYawPitchRoll(
		XMConvertToRadians(-90.0f),	// Y�����v���90�x
		XMConvertToRadians(90.0f),	// X�������v���45�x
		0.0f
	);
	LoadShader(m_pDevice);
	CreateConstanBuffer(m_pDevice);
	CreateShadowMapObject(m_pDevice);
}
/*
*	@brief �f�X�g���N�^
*	@details �V���h�E�}�b�v�p���C�g�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ShadowMapLight::~ShadowMapLight()
{
}
/*
*	@brief �X�V
*	@details �V���h�E�}�b�v�p���C�g�N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ShadowMapLight::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}
/*
*	@brief �e�ɂȂ郂�f����`��
*	@details �V���h�E�}�b�v�������_�����O����
*	@param view �r���[�s��
*	@param proj �ˉe�s��
*	@return �Ȃ�
*/
void ShadowMapLight::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ���C�g�̌����Ă�������𒲐�����
	const Vector3 lightDir = Vector3::Transform(Vector3(-1.0f, 0.0f, 1.0f), m_lightQuaternion);
	// ���C�g��Ԃ̃r���[�s����v�Z����
	const Matrix lightView = Matrix::CreateLookAt(
		m_lightPosition,			// eye
		m_lightPosition + lightDir,	// target
		Vector3::UnitY 				// up
	);
	// ���C�g��Ԃ̎ˉe�s����v�Z����
	const Matrix lightProjection = Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(m_lightTheta),	// fov
		1.0f,	// aspectRatio
		0.1f,	// nearPlane
		1000.0f	// farPlane
	);
	// GPU���g�p���郊�\�[�X�̃�������CPU����A�N�Z�X����ۂɗ��p����\����
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// �萔�o�b�t�@���}�b�v����
	context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// �萔�o�b�t�@�ɑ���f�[�^����������
	cbLight* cb = static_cast<cbLight*>(mappedResource.pData);
	const Matrix viewProj = lightView * lightProjection;
	cb->lightViewProjection = XMMatrixTranspose(viewProj);	// �r���[�s��Ǝˉe�s������Z��������
	cb->lightPosition = m_lightPosition;					// ���C�g�̍��W
	cb->lightDirection = lightDir;							// ���C�g���Ƃ炷����
	cb->lightAmbient = Color(0.125f, 0.125f, 0.125f, 0.001f);				// ����
	// �}�b�v����������
	context->Unmap(m_pConstantBuffer.Get(), 0);
	// �V���h�E�}�b�v�p��RTV��DSV��ݒ肷��
	auto rtv = m_pShadowMapRT->GetRenderTargetView();
	auto dsv = m_pShadowMapDS->GetDepthStencilView();
	// �V���h�E�}�b�v�p��RTV��DSV���N���A����
	context->ClearRenderTargetView(rtv, Colors::White);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// �V���h�E�}�b�vSRV���o�C���h����Ă������������
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv); // SRV�X���b�g1��NULL�ŃN���A
	// RTV��DSV���V���h�E�}�b�v�p�ɕύX����
	context->OMSetRenderTargets(1, &rtv, dsv);
	// �r���[�|�[�g���V���h�E�}�b�v�p�ɕύX����
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);
	// �e�ɂȂ郂�f����`�悷��
	for (const auto& shadowInfo : m_pShadowInfos)
	{
		// ���[���h�s��𒊏o
		Matrix world = shadowInfo.second;
		// ���f�����擾
		DirectX::Model* model = shadowInfo.first;
		// �e��`��
		model->Draw(context, *states, world, lightView, lightProjection, false, [&]
			{
				context->VSSetShader(m_pVSShadowMap.Get(), nullptr, 0);
				context->PSSetShader(m_pPSShadowMap.Get(), nullptr, 0);
			}
		);
	}
	// RTV��DSV��ʏ�`��p�̐ݒ�ɖ߂�����
	auto defaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();
	auto defaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();
	// �`���ɒʏ�`��p��RTV��DSV��ݒ肷��
	context->OMSetRenderTargets(1, &defaultRTV, defaultDSV);
	// �r���[�|�[�g��ʏ�`��p�ɐ؂�ւ���
	auto const& defaultVP = m_viewport;
	context->RSSetViewports(1, &defaultVP);
	// ���\�[�X�̊��蓖�Ă���������
	context->PSSetShaderResources(1, 1, nullsrv);
	// ���f���ƃ��[���h�s��̃y�A���N���A����
	m_pShadowInfos.clear();

}


/*
*	@brief �V�F�[�_�̓ǂݍ���
*	@details �V���h�E�}�b�v�p�̃V�F�[�_��ǂݍ���
*	@param device Direct3D�f�o�C�X
*	@return �Ȃ�
*/
void ShadowMapLight::LoadShader(ID3D11Device* device)
{
	// �V�F�[�_��ǂݍ��ނ��߂�blob
	std::vector<uint8_t> blob;
	// ���C�g�{�e�̒��_�V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/VS_Light.cso");
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_pVS.ReleaseAndGetAddressOf());
	// ���C�g�{�e�̃s�N�Z���V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/PS_Light.cso");
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_pPS.ReleaseAndGetAddressOf());
	// �V���h�E�}�b�v�p���_�V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/VS_ShadowMap.cso");
	device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_pVSShadowMap.ReleaseAndGetAddressOf());
	// �V���h�E�}�b�v�p�s�N�Z���V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/ShadowMapLight/PS_ShadowMap.cso");
	device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_pPSShadowMap.ReleaseAndGetAddressOf());
}
/*
*	@brief �萔�o�b�t�@�̍쐬
*	@details �V���h�E�}�b�v�p�̒萔�o�b�t�@���쐬����
*	@param device Direct3D�f�o�C�X
*	@return �Ȃ�
*/
void ShadowMapLight::CreateConstanBuffer(ID3D11Device* device)
{
	// �萔�o�b�t�@�p�̃o�b�t�@�I�u�W�F�N�g���쐬����
	D3D11_BUFFER_DESC bufferDesc{};
	// �o�b�t�@�T�C�Y��ݒ�
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(cbLight));
	// �o�b�t�@�̎g�p���@��ݒ�
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	// �o�b�t�@�̃o�C���h�t���O��ݒ�
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// �o�b�t�@��CPU�A�N�Z�X�t���O��ݒ�
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �o�b�t�@�̍\����ݒ�
	device->CreateBuffer(&bufferDesc, nullptr, m_pConstantBuffer.ReleaseAndGetAddressOf());

}
/*
*	@brief �V���h�E�}�b�v�p�̊e��I�u�W�F�N�g���쐬����
*	@details �V���h�E�}�b�v�p�̃����_�[�e�N�X�`���Ɛ[�x�X�e���V�����쐬����
*	@param device Direct3D�f�o�C�X
*	@return �Ȃ�
*/
void ShadowMapLight::CreateShadowMapObject(ID3D11Device* device)
{
	// �V���h�E�}�b�v�̑傫�����w�肷��
	const RECT rectShadow = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };
	// �����_�[�e�N�X�`�����쐬����
	m_pShadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	// Direct3D�f�o�C�X��ݒ肷��
	m_pShadowMapRT->SetDevice(device);
	// �V���h�E�}�b�v�̑傫����ݒ肷��
	m_pShadowMapRT->SetWindow(rectShadow);
	// �[�x�X�e���V�����쐬����
	m_pShadowMapDS = std::make_unique<mylib::DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	// Direct3D�f�o�C�X��ݒ肷��
	m_pShadowMapDS->SetDevice(device);
	// �V���h�E�}�b�v�̑傫����ݒ肷��
	m_pShadowMapDS->SetWindow(rectShadow);
	// �V���h�E�}�b�v�p�̃T���v���[���쐬����
	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	// �T���v���[�̐ݒ���s��
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	// �A�h���X���[�h��ݒ�
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	// ��r�֐���ݒ�
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&samplerDesc, m_pShadowMapSampler.ReleaseAndGetAddressOf());
}

/*
*	@brief ���f���`�掞�̃����_�����ŌĂԏ������܂Ƃ߂��֐�
*	@details ���f���`�掞�ɕK�v�Ȑݒ���s��
*	@param context Direct3D�f�o�C�X�R���e�L�X�g
*	@param states ���ʃX�e�[�g
*	@return �Ȃ�
*/
void ShadowMapLight::ApplyShader(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states)
{
	m_pSRV = m_pShadowMapRT->GetShaderResourceView();
	// �萔�o�b�t�@���w�肷��
	ID3D11Buffer* cbuf[] = { m_pConstantBuffer.Get() };
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
	// �V�F�[�_��ݒ肷��
	context->VSSetShader(m_pVS.Get(), nullptr, 0);
	context->PSSetShader(m_pPS.Get(), nullptr, 0);
	// �T���v���[�X�e�[�g���w�肷��
	ID3D11SamplerState* sampler[] = { states->LinearWrap(), m_pShadowMapSampler.Get() };
	context->PSSetSamplers(0, 2, sampler);
	// �쐬�����[�x�}�b�v���V�F�[�_���\�[�X�Ƃ��Đݒ肷��
	context->PSSetShaderResources(1, 1, &m_pSRV);
}