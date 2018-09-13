#include "stdafx.h"

#include "ijl.h"
#include <string>
#include "bmp2jpeg.h"

#pragma comment(lib, "../=Library/ijl15.lib") // lib ������ ����ؾ� �մϴ�.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BOOL BitmapToJPG ( LPCTSTR lpszPathName, BOOL bBitmapDelete )
{
    CBitmap csBitmap;
    BITMAP  stBitmap;
    bool bRes = true; // ���ϰ�

	// ��Ʈ�� �ε�
	csBitmap.m_hObject = LoadImage(NULL, lpszPathName, IMAGE_BITMAP, NULL, NULL,
									LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	csBitmap.GetBitmap(&stBitmap); // ��Ʈ�� ������ ��´�.
	
	IJLERR jerr;
	DWORD dib_pad_bytes;
	
	// Allocate the IJL JPEG_CORE_PROPERTIES structure.
	JPEG_CORE_PROPERTIES jcprops;
	BYTE * imageData; // �̹��� �ȼ� ����

	while(true)
	{
		// Initialize the IntelR JPEG Library.
		jerr = ijlInit(&jcprops);

		if(IJL_OK != jerr)
		{
			TRACE0("JCP �ʱ�ȭ�� �����߽��ϴ�.\n");
			bRes = false;
			break;
		}

		int nDIBChannels = 3;
		if ( stBitmap.bmBitsPixel != 24 && stBitmap.bmBitsPixel != 32 )
		{
			TRACE0("24��Ʈ, 32��Ʈ ��Ʈ���� �ƴմϴ�.");
            bRes = false;
			break;
		}

		if ( stBitmap.bmBitsPixel == 32 )

		dib_pad_bytes = IJL_DIB_PAD_BYTES(stBitmap.bmWidth, 3);
	
		// Set up information to write from the pixel buffer.
		jcprops.DIBWidth    = stBitmap.bmWidth;
		jcprops.DIBHeight   = stBitmap.bmHeight;
		jcprops.DIBChannels = 3;
		jcprops.DIBPadBytes = dib_pad_bytes;
		jcprops.DIBColor    = IJL_BGR;
		
		int imageSize = (stBitmap.bmWidth*stBitmap.bmBitsPixel/8+dib_pad_bytes) * stBitmap.bmHeight;

		imageData = new BYTE[imageSize]; // �̹��� ������ ��ŭ �����͸� �ʱ�ȭ
        if ( imageData==NULL )
        {
            TRACE( "�̹����� ���� �޸� �Ҵ翡 �����߽��ϴ�.\n" );
            AfxThrowUserException();
            bRes = false;
			break;
        }

		DWORD dwRead = csBitmap.GetBitmapBits ( imageSize , imageData); // ��Ʈ�� ������ ������ ��´�.
		if ( dwRead==0 )
		{
            TRACE( "GetBitmapBits() dib �б⿡ �����Ͽ����ϴ�.\n" );
			AfxThrowUserException();
            bRes = false;
			break;
		}

		if ( stBitmap.bmBitsPixel==32 )
		{
			for ( int i=0; i<imageSize/4; ++i )
			{
				DWORD dwDATA = * (DWORD*) ( imageData + i*4 );

				*(imageData+i*3+0) = BYTE(dwDATA>>0&0xFF);
				*(imageData+i*3+1) = BYTE(dwDATA>>8&0xFF);
				*(imageData+i*3+2) = BYTE(dwDATA>>16&0xFF);
			}
		}

		jcprops.DIBBytes        = imageData;
				
		string JPGPathName(lpszPathName);
		string::size_type idx = JPGPathName.find_last_of(".");
		JPGPathName.replace(idx+1, string::npos, "jpg");

		jcprops.JPGFile         = const_cast<LPTSTR>(JPGPathName.c_str());
	    		
		// Specify JPEG file creation parameters.
		jcprops.JPGWidth        = stBitmap.bmWidth;
		jcprops.JPGHeight       = stBitmap.bmHeight;
		
		// Note: the following are default values and thus
		// do not need to be set.
		jcprops.JPGChannels     = 3;
		jcprops.JPGColor        = IJL_YCBCR;
		jcprops.JPGSubsampling  = IJL_411; // 4:1:1 subsampling.
		jcprops.jquality        = 90; // �̹����� ȭ��, ������ 75, �ְ� 100
		
		// Write the actual JPEG image from the pixel buffer.
		jerr = ijlWrite(&jcprops,IJL_JFILE_WRITEWHOLEIMAGE);
		
		if(IJL_OK != jerr)
		{
			TRACE0("�̹����� ���Ͽ� �����ϴµ� �����߽��ϴ�.\n");
			bRes = false;
			break;
		}
		
		break; // while ������ �������ɴϴ�.
	}

	// Clean up the IntelR JPEG Library.
	ijlFree(&jcprops);

    if( bBitmapDelete == TRUE )		// ���� �÷��װ� true�̸�
        DeleteFile(lpszPathName);	// ��Ʈ�� ������ �����մϴ�.

	if ( imageData )
		delete [] imageData; // �������� �Ҵ�� �޸� ����

	return bRes;
}
