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

#pragma once

#include "ViewTree.h"
#include "iostream"
#include <vector>

#include "GeneralUtility.h"
#include "ImageManage.h"
#include "CameraCalibration.h"

using namespace std;



class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 特性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

// 实现
public:
	virtual ~CFileView();
	virtual BOOL CanBeClosed() const;
	virtual BOOL CanFloat() const;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

public:
	void imoprtChessBoardImages();
	void showVaildImagesTree();

	vector<string> m_imageListName;
	CImageManage m_imgManage;
	CCameraCalibration m_calib;

private:
	HTREEITEM hRoot;
	void setRootItem();
	void clearLastImport();

	int flag = 0;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

