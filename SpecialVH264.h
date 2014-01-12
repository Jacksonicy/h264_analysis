
// SpecialVH264.h : PROJECT_NAME 应用程序的主头文件
//
/*
 * 作者：雷霄骅
 * 中国传媒大学|信息工程学院|数字电视技术
 * 版权所有
 */
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CSpecialVH264App:
// 有关此类的实现，请参阅 SpecialVH264.cpp
//

class CSpecialVH264App : public CWinApp
{
public:
	CSpecialVH264App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSpecialVH264App theApp;