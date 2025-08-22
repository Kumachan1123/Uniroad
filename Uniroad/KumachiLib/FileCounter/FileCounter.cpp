/*
*   @file FileCounter.cpp
*   @brief �t�@�C���J�E���^�[�N���X�̎���
*/
#include "pch.h"
#include "FileCounter.h"
/*
*   @brief �w�肵���t�H���_�̒��̃t�@�C�������J�E���g����
*   @details �w�肳�ꂽ�t�H���_�p�X���̃t�@�C�������J�E���g����֐�
*   @param folderPath �J�E���g����t�H���_�̃p�X
*	@param extension �t�@�C���̊g���q�i��: ".csv"�j
*   @return �t�@�C����
*/
int FileCounter::CountFilesInFolder(const std::string& folderPath, const std::string& extension)
{
	// �J�E���g�p�ϐ�
	int count = 0;
	// �����p�̃p�X
	std::string searchPath = folderPath + "\\*" + extension;
	// WIN32_FIND_DATA�\���̂̃C���X�^���X���쐬
	WIN32_FIND_DATAA findFileData{};
	// FindFirstFileA�֐����g�p���ăt�@�C���������J�n
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);
	// �t�@�C�������������ꍇ
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// �t�@�C�����f�B���N�g���łȂ��ꍇ�A�J�E���g�𑝂₷
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				count++;
			}
		} while (FindNextFileA(hFind, &findFileData) != 0); // ���̃t�@�C��������
		// �����n���h�������
		FindClose(hFind);
	}
	// �t�@�C������Ԃ�
	return count;
}
