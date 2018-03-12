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

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"

#define  WM_PROCESS_STEP WM_USER+101

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	//CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CFileView         m_wndFileView;


// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);




public:
	virtual BOOL OnShowPopupMenu(CMFCPopupMenu* pMenuPopup);
	afx_msg void OnImoprtdata();
	afx_msg void OnDetect();
	afx_msg void OnRun();
	afx_msg void OnParams();
	afx_msg void OnExport();
	afx_msg void OnSelect();

public:
	CMFCRibbonProgressBar* m_Progress;

private:
	afx_msg LRESULT updateProgress(WPARAM wParam, LPARAM lParam);
	int numImage = 0;
	int currentStep = 0;

	bool canUse = true;
	
};


