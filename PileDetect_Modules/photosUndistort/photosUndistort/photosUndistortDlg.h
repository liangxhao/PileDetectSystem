
// photosUndistortDlg.h : 头文件
//

#pragma once

#include "Undistorts.h"
#include "afxwin.h"
#include "afxcmn.h"

#define  WM_PROCESS_STEP WM_USER+101

// CphotosUndistortDlg 对话框
class CphotosUndistortDlg : public CDialogEx
{
// 构造
public:
	CphotosUndistortDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHOTOSUNDISTORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImportimages();


	afx_msg void OnBnClickedImportparms();
	afx_msg void OnBnClickedSetexport();
	afx_msg void OnBnClickedRunundistort();
	
	afx_msg LRESULT updateProgress(WPARAM wParam, LPARAM lParam);
	int numImage = 0;
	int currentImage = 0;

	bool canUse = true;
	

	CProgressCtrl m_Progress;
	CUndistorts undis;

};
