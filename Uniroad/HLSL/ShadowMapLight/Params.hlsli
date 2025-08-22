/*
    ���ʂŎg�p����p�����[�^
*/

// ���C�g�p�萔�o�b�t�@
cbuffer LightParameters : register(b1)
{
    float4x4 lightViewProjection    : packoffset(c0);
    float4 lightPosition            : packoffset(c4);
    float3 lightDirirection         : packoffset(c5);
    float3 lightAmbient             : packoffset(c6);
};

// ���_�V�F�[�_���͗p
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};

// �s�N�Z���V�F�[�_���͗p
struct PS_Input
{
    float2 TexCoord     : TEXCOORD0;
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float4 LightPosPS   : TEXCOORD3;
    float4 PositionPS   : SV_Position;
};

// �V���h�E�}�b�v�̒��_�V�F�[�_���͗p
struct VS_SM_Input
{
    float4 Position : SV_Position;
};

// �V���h�E�}�b�v�̃s�N�Z���V�F�[�_���͗p
struct PS_SM_Input
{
    float4 Position : SV_Position;
};
