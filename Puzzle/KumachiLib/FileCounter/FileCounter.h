/*
*	@file FileCounter.h
*	@brief �t�@�C���J�E���^�[�N���X
*/
#pragma once
// �W�����C�u����
#include <string>
#include <Windows.h>

// �t�@�C���J�E���^�[�N���X
class FileCounter
{
public:
	// �ÓIpublic�֐�
	// �w�肵���t�H���_�̒��̃t�@�C�������J�E���g����
	static int CountFilesInFolder(const std::string& folderPath, const std::string& extension);

};