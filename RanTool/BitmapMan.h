#pragma	  once

class CBitmapMan
{
protected:
	TCHAR	m_szFilePath[_MAX_PATH];	// �ε��� ������ �н�
	TCHAR	m_szFileName[_MAX_FNAME];	// ���� �̸�
	TCHAR	m_szExt[_MAX_EXT];			// Ȯ����

	HBITMAP m_hBitmap;					// �ε��� �̹��� �ڵ�
	BITMAP	m_sBitmap;					// ��Ʈ�� ����ü

protected:
	CBitmapMan();

public:
	~CBitmapMan();

public:
	void	Delete();
	BOOL	LOAD( CString & strFileName );	// ���� �̸����� ��Ʈ���� �д´�.

	HBITMAP	GetHBitmap()	{return m_hBitmap;}		// �ε��� �̹��� �ڵ� ����
	BITMAP	GetBitmap()		{return m_sBitmap;}		// ��Ʈ�� ������ ����
	PTCHAR	GetFileName()	{return m_szFileName;}	// ���ϸ��� �ѱ��.
	PTCHAR	GetFilePath()	{return m_szFilePath;}

public:
	static	CBitmapMan	m_cBitmapMan;
};