/*
	�[�x�l�`��p���_�V�F�[�_
*/
#include "Common.hlsli"
#include "Params.hlsli"


// ���C���֐�
PS_SM_Input main(VS_SM_Input input)
{
    /* �菇�P */

    PS_SM_Input output;
    
    output.Position = mul(input.Position, WorldViewProj);
    
    return output;
}