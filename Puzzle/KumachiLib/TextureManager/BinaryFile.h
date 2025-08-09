#pragma once
/// <summary>
/// �o�C�i���t�@�C���������N���X
/// </summary>


#include <memory>
namespace KumachiLib
{
	class BinaryFile
	{
	protected:

		//	�f�[�^
		std::unique_ptr<char[]> m_data;

		//	�T�C�Y
		unsigned int m_size;

	public:

		//	�t�@�C�������w�肵�ă��[�h
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);

		BinaryFile();

		//	���[�u�R���X�g���N�^
		BinaryFile(BinaryFile&& in);

		//	�A�N�Z�T
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() const { return m_size; }
	};
}