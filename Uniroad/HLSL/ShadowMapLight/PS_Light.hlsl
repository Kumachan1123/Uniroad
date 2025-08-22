/*
	���C�gwith�V���h�E�}�b�v�p�s�N�Z���V�F�[�_
*/
#include "Common.hlsli"
#include "Params.hlsli"

// �V���h�E�}�b�v�e�N�X�`��
Texture2D ShadowMapTexture : register(t1);

// �V���h�E�}�b�v�p�e�N�X�`���T���v���[
SamplerComparisonState ShadowMapSampler : register(s1);

// �}�b�n�o���h�΍�
static const float SHADOW_EPSILON = 0.0001f;


float4 main(PS_Input input) : SV_TARGET0
{
    // �@���𐳋K������
    float3 normal = normalize(input.NormalWS);

    // ----------------------------------------------------------- //
    // �f�B�t���[�Y
    // ----------------------------------------------------------- //
 
    // ���C�g���Ƃ炷�������v�Z����
    float3 lightDir = normalize(lightDirirection);
    
    // ���C�g�֌������x�N�g��
    float3 toLight = (-lightDir);

    // ���̋�������ς���Z�o����
    float intensity1 = max(dot(normal, toLight), 0.0f);

    // ���K���f�o�C�X���W�ɂ���
    input.LightPosPS.xyz /= input.LightPosPS.w;

    // ���݂̃s�N�Z�����V���h�E�}�b�v��łǂ̈ʒu�ɂ����邩�v�Z����
    float2 uv = input.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

    // �ʏ�`��̐[�x�l�ƃ��C�g��Ԃ̐[�x�l���r���ĉe�ɂȂ邩���ׂ�
    // ShadowMapTexture�̊Y������ꏊ�̐[�x�l�ƌ��݂̃s�N�Z���̐[�x�l���r���āA�e�ɂȂ邩���ׂ�
    // shadow�@0.0f�F�e������A1.0f�F�e���Ȃ�
    float shadow = ShadowMapTexture.SampleCmpLevelZero(ShadowMapSampler, uv, input.LightPosPS.z - SHADOW_EPSILON).x;

    // ���C�g�Ɖe�ɂ�閾�邳���v�Z����
    float3 lightAmount = lerp(lightAmbient, float3(1.5f, 1.5f, 1.5f), shadow);
    lightAmount *= intensity1;

    // �g�U���ːF�����߂� 
    float3 diffuse = DiffuseColor.rgb * lightAmount;

        
    // ----------------------------------------------------------- //
    // �X�y�L����
    // ----------------------------------------------------------- //
    

    
    // �����x�N�g��
    float3 toEye = normalize(EyePosition - input.PositionWS.xyz);

    // �n�[�t�x�N�g�����J�������W�{���C�g���W
    float3 halfVector = normalize(toEye + toLight);

    // ���ʔ��˂̉e����������ς��g���v�Z����
    float intensity2 = max(dot(normal, halfVector), 0.0f);

    // �X�y�L�����p���[���w���Ƃ��Ďg���n�C���C�g�̂������𒲐�����
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
    
    // �e�̉e�����l������
    specular *= shadow;
 
    
    // ----------------------------------------------------------- //
    // �ŏI�J���[
    // ----------------------------------------------------------- //

    // �e�N�X�`���J���[���T���v������
    float4 textureColor = Texture.Sample(Sampler, input.TexCoord);
    
    // �e�N�X�`���̐F�Ɋg�U���˂Ƌ��ʔ��˂𔽉f����
    float3 finalColor = textureColor.rgb * diffuse + specular + lightAmbient;
    
    return float4(finalColor, 1.0f);
}
