/*
	�[�x�l�`��p�s�N�Z���V�F�[�_
*/
#include "Common.hlsli"
#include "Params.hlsli"


// ���C���֐�
float4 main(PS_SM_Input input) : SV_Target0
{
    /* �菇�Q */

    // Z�l���o�͂���
    return input.Position.z;
    //return float4(input.Position.z, input.Position.z, input.Position.z, 1.0f);
}