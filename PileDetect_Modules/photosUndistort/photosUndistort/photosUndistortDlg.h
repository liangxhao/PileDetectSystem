
// photosUndistortDlg.h : ͷ�ļ�
//

#pragma once

#include "Undistorts.h"
#include "afxwin.h"
#include "afxcmn.h"

#define  WM_PROCESS_STEP WM_USER+101

// CphotosUndistortDlg �Ի���
class CphotosUndistortDlg : public CDialogEx
{
// ����
public:
	CphotosUndistortDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHOTOSUNDISTORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
