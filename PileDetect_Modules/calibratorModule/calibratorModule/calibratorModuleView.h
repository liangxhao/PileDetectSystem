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

// calibratorModuleView.h : CcalibratorModuleView ��Ľӿ�
//

#pragma once
#include "calibratorModuleDoc.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"

using namespace cv;


class CcalibratorModuleView : public CView
{
protected: // �������л�����
	CcalibratorModuleView();
	DECLARE_DYNCREATE(CcalibratorModuleView)

// ����
public:
	CcalibratorModuleDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CcalibratorModuleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // calibratorModuleView.cpp �еĵ��԰汾
inline CcalibratorModuleDoc* CcalibratorModuleView::GetDocument() const
   { return reinterpret_cast<CcalibratorModuleDoc*>(m_pDocument); }
#endif

