
// SpecialVH264.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
/*
 * ���ߣ�������
 * �й���ý��ѧ|��Ϣ����ѧԺ|���ֵ��Ӽ���
 * ��Ȩ����
 */
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSpecialVH264App:
// �йش����ʵ�֣������ SpecialVH264.cpp
//

class CSpecialVH264App : public CWinApp
{
public:
	CSpecialVH264App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSpecialVH264App theApp;