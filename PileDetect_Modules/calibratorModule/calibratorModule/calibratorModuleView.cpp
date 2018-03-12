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

// calibratorModuleView.cpp : CcalibratorModuleView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "calibratorModule.h"
#endif

#include "calibratorModuleDoc.h"
#include "calibratorModuleView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcalibratorModuleView

IMPLEMENT_DYNCREATE(CcalibratorModuleView, CView)

BEGIN_MESSAGE_MAP(CcalibratorModuleView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CcalibratorModuleView 构造/析构

CcalibratorModuleView::CcalibratorModuleView()
{
	// TODO: 在此处添加构造代码

}

CcalibratorModuleView::~CcalibratorModuleView()
{
}

BOOL CcalibratorModuleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CcalibratorModuleView 绘制

void CcalibratorModuleView::OnDraw(CDC* pDC)
{
	CcalibratorModuleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	if (m_isViewflag)
	{
		CRect r;
		GetClientRect(&r);
		int w = m_img.Width();
		int h = m_img.Height();
		int viewW = r.Width();
		int viewH = r.Height();
		CRect region;
		if (viewH / viewW <= h / w)
		{
			int x = int(viewH*w / h);
			region = CRect((viewW - x) / 2, 0, (viewW + x) / 2, viewH);
		}
		else
		{
			int x = int(viewW*h / w);
			region = CRect(0,(viewH - x) / 2, viewW, (viewH + x) / 2);
		}
		m_img.DrawToHDC(pDC->GetSafeHdc(), region);
	}

}

void CcalibratorModuleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CcalibratorModuleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CcalibratorModuleView 诊断

#ifdef _DEBUG
void CcalibratorModuleView::AssertValid() const
{
	CView::AssertValid();
}

void CcalibratorModuleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcalibratorModuleDoc* CcalibratorModuleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcalibratorModuleDoc)));
	return (CcalibratorModuleDoc*)m_pDocument;
}
#endif //_DEBUG


// CcalibratorModuleView 消息处理程序
void CcalibratorModuleView::setImage(Mat img)
{
	IplImage iplImg = img;

	m_img.CopyOf(&iplImg);

	m_isViewflag = true;
	Invalidate();
	//PostMessage(WM_PAINT);
}



//void CcalibratorModuleView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	//CView::OnMouseMove(nFlags, point);
//}
