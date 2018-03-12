// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// calibratorModuleView.cpp : CcalibratorModuleView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CcalibratorModuleView ����/����

CcalibratorModuleView::CcalibratorModuleView()
{
	// TODO: �ڴ˴���ӹ������

}

CcalibratorModuleView::~CcalibratorModuleView()
{
}

BOOL CcalibratorModuleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CcalibratorModuleView ����

void CcalibratorModuleView::OnDraw(CDC* pDC)
{
	CcalibratorModuleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

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


// CcalibratorModuleView ���

#ifdef _DEBUG
void CcalibratorModuleView::AssertValid() const
{
	CView::AssertValid();
}

void CcalibratorModuleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcalibratorModuleDoc* CcalibratorModuleView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcalibratorModuleDoc)));
	return (CcalibratorModuleDoc*)m_pDocument;
}
#endif //_DEBUG


// CcalibratorModuleView ��Ϣ�������
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
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	//CView::OnMouseMove(nFlags, point);
//}
