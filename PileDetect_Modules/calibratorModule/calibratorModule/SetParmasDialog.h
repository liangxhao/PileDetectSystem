#pragma once
#include "afxwin.h"

#include "ParamsFiles.h"
// CSetParmas 对话框

class CSetParmasDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetParmasDialog)

public:
	CSetParmasDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetParmasDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETPARMAS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();

	CEdit m_squareSize;
	CParamsFiles params;
	CButton m_IsFixK3;
	afx_msg void OnBnClickedAutochesssize();
	CButton m_AutoChessSize;
	CEdit m_ChessSizeHeight;
	CEdit m_ChessSizeWidth;
};
