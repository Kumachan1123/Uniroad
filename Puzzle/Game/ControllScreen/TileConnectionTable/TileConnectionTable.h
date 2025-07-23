/*
*	@file TileConnectionTable.h
*	@brief �ڑ��\�ȃ^�C���̃e�[�u���N���X
*/
#pragma once
// �W�����C�u����
#include <unordered_map>
#include <string>
#include <vector>

// �v���C���[�̐i�s������\���񋓌^
enum class Direction
{
	UP,		// �����
	DOWN,	// ������
	LEFT,	// ������
	RIGHT	// �E����
};

// �^�C���ڑ��e�[�u���N���X
class TileConnectionTable
{
public:
	// �ÓIpublic�֐�
	// �ڑ��\�ȃ^�C���̃e�[�u�����擾����
	static const std::unordered_map<std::string, std::unordered_map<Direction, std::vector<std::string>>>& GetTable();
	// ���̔z���Ԃ��֐�
	static const std::vector<std::string>& GetDummyArray();
	// �w�肳�ꂽ�^�C�����ƕ����Ɋ�Â��Đڑ��\�ȃ^�C���̃��X�g���擾����
	static const std::vector<std::string>& GetConnectableTiles(const std::string& tileName, Direction dir);
};