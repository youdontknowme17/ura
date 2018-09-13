// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

//#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.
//
//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
//#endif
//
//#include <afx.h>
//#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//	dx ���ð� ��Ÿ �ʼ� ��Ŭ��� �� ����.
#include "dxstdafx.h"

#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>     // WIN32 and ANSI 
#include <Mmsystem.h>
// #include <varargs.h> // UNIX

#include <fstream>
#include <string>
#include <vector>

#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE

// Must Install Platform SDK
// Visit and install http://www.microsoft.com/msdownload/platformsdk/sdkupdate/
#include <strsafe.h> // Safe string function

