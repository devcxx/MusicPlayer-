#pragma once
#include "Common.h"
class CFilePathHelper
{
public:
	CFilePathHelper(const wstring& file_path);
	CFilePathHelper(){}
	~CFilePathHelper();

	void SetFilePath(const wstring& file_path) { m_file_path = file_path; }

	wstring GetFileExtension(bool upper = false) const;		//��ȡ�ļ�����չ������������.����
	wstring GetFileName() const;							//��ȡ�ļ���
	wstring GetFileNameWithoutExtension() const;			//��ȡ�ļ�����������չ����
	wstring GetFolderName() const;							//��ȡ�ļ�����
	wstring GetDir() const;									//��ȡĿ¼
	wstring GetParentDir() const;							//��ȡ�ϼ�Ŀ¼
	wstring GetFilePath() const { return m_file_path; }		//��ȡ����·��
	const wstring& ReplaceFileExtension(const wchar_t* new_extension);		//�滻�ļ�����չ���������ļ�����·��
protected:
	wstring m_file_path;
};

