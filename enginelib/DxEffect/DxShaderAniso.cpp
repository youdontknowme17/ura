#include "stdafx.h"

#include "DxEffectAniso.h"

//	Note : ���� ���� ����.
//
DWORD	DxEffectAniso::m_dwEffect;

/*
//	Note : Anisotropic 
//
char DxEffectAniso::m_strEffect[] =
{
	"vs.1.0\n"

	//	Note : Vertex�� �Ѹ���.
	//
	"m4x4 oPos, v0, c22	\n"

	//	Note : r0 �� World ��ǥ�� ����� Normal���͸� �ִ´�.
	//

	//	Normal ���Ͱ� World ��Ʈ������ ���� �ȴ�.
	"m3x3 r0, v3, c10	\n"

	//	���� ���ͷ� �����.
	"dp3 r0.w, r0, r0	\n"
	"rsq r0.w, r0.w		\n"
	"mul r0, r0, r0.w	\n"

	//	Note : r1 �� Vertex�� World ��ǥ�� ����� ���͸� �ִ´�.
	//
	"m4x3 r1, v0, c10	\n"
	"mov r1.w, v0.w		\n"

	//	Note : r2 �� r1 ���� ī�޶� ��ġ������ �Ÿ� ���͸� ���Ѵ�.
	//
	"add r2, c8, -r1	\n"

	//	Note : r2 �� ���� ���ͷ� �����.
	//
	"dp3 r2.w, r2, r2	\n"
	"rsq r2.w, r2.w		\n"
	"mul r2, r2, r2.w	\n"

	//	Note : r1 �� r2�� �������� ���Ѵ�.
	//
	"add r1, r2, -c0		\n"

	//	Note : r1 �� ���� ���ͷ� �����.
	//
	"dp3 r1.w, r1, r1	\n"
	"rsq r1.w, r1.w		\n"
	"mul r1, r1, r1.w	\n"

	//l dot n
	"dp3 oT0.x, r0, -c0	\n"

	//h dot n
	"dp3 oT0.y, r1, r0	\n"
};
*/

DWORD DxEffectAniso::dwAnisoVertexShader[] = {
	0xfffe0100, 0x00000014, 0xc00f0000, 0x90e40000,
	0xa0e40016, 0x00000017, 0x800f0000, 0x90e40003,
	0xa0e4000a, 0x00000008, 0x80080000, 0x80e40000,
	0x80e40000, 0x00000007, 0x80080000, 0x80ff0000,
	0x00000005, 0x800f0000, 0x80e40000, 0x80ff0000,
	0x00000015, 0x800f0001, 0x90e40000, 0xa0e4000a,
	0x00000001, 0x80080001, 0x90ff0000, 0x00000002,
	0x800f0002, 0xa0e40008, 0x81e40001, 0x00000008,
	0x80080002, 0x80e40002, 0x80e40002, 0x00000007,
	0x80080002, 0x80ff0002, 0x00000005, 0x800f0002,
	0x80e40002, 0x80ff0002, 0x00000002, 0x800f0001,
	0x80e40002, 0xa1e40000, 0x00000008, 0x80080001,
	0x80e40001, 0x80e40001, 0x00000007, 0x80080001,
	0x80ff0001, 0x00000005, 0x800f0001, 0x80e40001,
	0x80ff0001, 0x00000008, 0xe0010000, 0x80e40000,
	0xa1e40000, 0x00000008, 0xe0020000, 0x80e40001,
	0x80e40000, 0x0000ffff
};

DWORD	DxEffectAniso::m_dwSavedStateBlock(0x00000000);
DWORD	DxEffectAniso::m_dwEffectStateBlock(0x00000000);