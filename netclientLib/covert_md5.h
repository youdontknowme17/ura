#include "StdAfx.h"
#include "china_md5.h"
#include <string>;
using namespace std;
namespace convert_md5
{
	char* CovertMD5(
		const TCHAR *pass
	)
	{
		CHINA_MD5::MD5 md5;
		size_t nLength = 0;
		md5.update( reinterpret_cast<const unsigned char*> (pass), nLength );
		std::string strPass = md5.asString();
		return strPass;
	}
}

