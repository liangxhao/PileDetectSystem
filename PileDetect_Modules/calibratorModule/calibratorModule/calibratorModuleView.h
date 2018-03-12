// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// calibratorModuleView.h : CcalibratorModuleView 类的接口
//

#pragma once
#include "calibratorModuleDoc.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"

using namespace cv;


class CcalibratorModuleView : public CView
{
protected: // 仅从序列化创建
	CcalibratorModuleView();
	DECLARE_DYNCREATE(CcalibratorModuleView)

// 特性
public:
	CcalibratorModuleDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CcalibratorModuleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()


private:
	CvvImage m_img;
	bool m_isViewflag=false;

public:
	void setImage(Mat img);

//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // calibratorModuleView.cpp 中的调试版本
inline CcalibratorModuleDoc* CcalibratorModuleView::GetDocument() const
   { return reinterpret_cast<CcalibratorModuleDoc*>(m_pDocument); }
#endif

