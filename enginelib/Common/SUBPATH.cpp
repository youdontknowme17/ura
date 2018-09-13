#include "stdafx.h"
#include "./SubPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace SUBPATH
{
	TCHAR APP_ROOT[MAX_PATH]			= _T("");

	TCHAR SAVE_ROOT[MAX_PATH]			= _T("\\Ran Online\\");

	TCHAR PLAYINFO_ROOT[MAX_PATH]		= _T("\\Ran Online\\PlayData\\");
	TCHAR DEBUGINFO_ROOT[MAX_PATH]		= _T("\\Ran Online\\GameLogs\\");
	TCHAR CAPTURE[MAX_PATH]				= _T("\\Ran Online\\ScreenShots\\");
	TCHAR CHATLOG[MAX_PATH]				= _T("\\Ran Online\\ChatLogs\\");

	TCHAR CACHE[MAX_PATH]				= _T("\\cache\\");

	TCHAR OBJ_FILE_ROOT[MAX_PATH]		= _T("\\Data\\");
	TCHAR OBJ_FILE_EDIT[MAX_PATH]		= _T("\\Data\\Editor\\");
	TCHAR OBJ_FILE_PIECE[MAX_PATH]		= _T("\\Data\\Piece\\");
	TCHAR OBJ_FILE_PIECEEDIT[MAX_PATH]	= _T("\\Data\\Piece\\Edit\\");
	TCHAR OBJ_FILE_SKIN[MAX_PATH]		= _T("\\Data\\Skin\\");
	TCHAR OBJ_FILE_SKELETON[MAX_PATH]	= _T("\\Data\\Skeleton\\");
	TCHAR OBJ_FILE_ANIMATION[MAX_PATH]	= _T("\\Data\\Animation\\");
	TCHAR OBJ_FILE_SKINOBJECT[MAX_PATH]	= _T("\\Data\\SkinObject\\");

	TCHAR MAP_FILE[MAX_PATH]			= _T("\\Data\\Map\\");
	TCHAR GLMAP_FILE[MAX_PATH]			= _T("\\Data\\GLMap\\");
	TCHAR GLOGIC_FILE[MAX_PATH]			= _T("\\Data\\GLogic\\");
	TCHAR GLOGIC_SERVER_FILE[MAX_PATH]	= _T("\\Data\\GLogicServer\\");

	TCHAR TILE_FILE_DATA[MAX_PATH]		= _T("\\Data\\Tiles\\");
	TCHAR TILE_FILE_TEXTURE[MAX_PATH]	= _T("\\Textures\\Tiles\\");

	TCHAR OBJ_FILE_OBJECT[MAX_PATH]		= _T("\\Data\\Object\\");
	TCHAR OBJ_FILE_STATICMESH[MAX_PATH]	= _T("\\Data\\StaticMesh\\");

	TCHAR TEXTURE_FILE_ROOT[MAX_PATH]	= _T("\\Textures");
	TCHAR TEXTURE_FILE_SHADOW[MAX_PATH]	= _T("\\Textures\\Shadow\\");
	TCHAR TEXTURE_FILE_CLUB[MAX_PATH]	= _T("\\Textures\\Club\\");
	TCHAR TEXTURE_FILE_EFF[MAX_PATH]	= _T("\\Textures\\Effect\\");
	TCHAR SHADER_FILE_ROOT[MAX_PATH]	= _T("\\Shaders\\");

	TCHAR EFF_FILE_SINGLE[MAX_PATH]		= _T("\\Data\\Effect\\");
	TCHAR EFF_FILE_CHAR[MAX_PATH]		= _T("\\Data\\Effect\\Char\\");

	TCHAR GUI_FILE_ROOT[MAX_PATH]		= _T("\\Data\\GUI\\");
	TCHAR NTK_FILE_ROOT[MAX_PATH]		= _T("\\Data\\GLogic\\NPCTALK\\");
	TCHAR QST_FILE_ROOT[MAX_PATH]		= _T("\\Data\\GLogic\\Quest\\");

	TCHAR SOUND_FILE_ROOT[MAX_PATH]		= _T("\\Sounds\\Sfx\\");
	TCHAR BGM_FILE_ROOT[MAX_PATH]		= _T("\\Sounds\\BGM\\");
};