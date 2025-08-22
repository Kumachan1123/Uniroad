/*
	���C�gwith�V���h�E�}�b�v�p���_�V�F�[�_
*/
#include "Common.hlsli"
#include "Params.hlsli"


PS_Input main(VS_Input input)
{
	/* �菇�W */

    PS_Input output;

    // ���_�̈ʒu�𓊉e��Ԃ�
    output.PositionPS = mul(input.Position, WorldViewProj);
    
    // ���_�̈ʒu�����[���h��Ԃ�
    output.PositionWS = mul(input.Position, World);
    
    // �@���x�N�g�������[���h��Ԃցi�g��k���̉e����ł��������ߋt�]�u�s���������j
    output.NormalWS = mul(input.Normal, WorldInverseTranspose);
    
    // �e�N�X�`����UV���W
    output.TexCoord = input.TexCoord;
    
    // ���_�̈ʒu�����C�g�̓��e��Ԃ�
    output.LightPosPS = mul(output.PositionWS, lightViewProjection);
 
    return output;
}
