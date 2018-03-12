// SetParmas.cpp : 实现文件
//

#include "stdafx.h"
#include "calibratorModule.h"
#include "SetParmasDialog.h"
#include "afxdialogex.h"


// CSetParmas 对话框

IMPLEMENT_DYNAMIC(CSetParmasDialog, CDialog)

CSetParmasDialog::CSetParmasDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETPARMAS, pParent)
{

}

CSetParmasDialog::~CSetParmasDialog()
{
}

void CSetParmasDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SQUARESIZE, m_squareSize);
	DDX_Control(pDX, IDC_RATIONALMODEL, m_IsFixK3);
	DDX_Control(pDX, IDC_AUTOCHESSSIZE, m_AutoChessSize);
	DDX_Control(pDX, IDC_CHESSHEIGHT, m_ChessSizeHeight);
	DDX_Control(pDX, IDC_CHEEWIDTH, m_ChessSizeWidth);
}


BEGIN_MESSAGE_MAP(CSetParmasDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSetParmasDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_AUTOCHESSSIZE, &CSetParmasDialog::OnBnClickedAutochesssize)
END_MESSAGE_MAP()


// CSetParmas 消息处理程序


BOOL CSetParmasDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//只能输入数字，小数点保留两位
	if (pMsg->message == WM_CHAR // 字符消息
		&& (pMsg->hwnd == m_squareSize.m_hWnd))
	{
		CString str;
		GetDlgItemText(IDC_SQUARESIZE, str); // 获取edit中文本
		int nPos = 0;
		nPos = str.Find('.'); // 查找.的位置

		//只允许输入一个小数点
		if (pMsg->wParam == '.')
		{
			if (nPos >= 0)
			{
				return TRUE; // 如果存在. 返回，即不再允许.输入
			}
		}
		//只允许输入. 和数字 	
		if (pMsg->wParam != '.' && (pMsg->wParam > '9' || pMsg->wParam < '0'))
		{
			return TRUE;
		}
		if (nPos >= 0)
		{
			CString strTemp = str.Mid(nPos + 1);
			if (strTemp.GetLength() == 2)
			{
				return TRUE;
			}

		}		

	}
	//响应删除键
	if (pMsg->message == WM_KEYDOWN
		&& (pMsg->hwnd == m_squareSize.m_hWnd))
	{
		
		if (pMsg->wParam == VK_BACK)          //接受Backspace和delete键  
		{
			CString str;
			GetDlgItemText(IDC_SQUARESIZE, str); // 获取edit中文本
			CString tmp;
			tmp=str.Left(str.GetLength() - 1);
			m_squareSize.SetWindowTextW(tmp);
			
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSetParmasDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	params.IsFixK3 =m_IsFixK3.GetCheck();

	CString str;
	m_squareSize.GetWindowTextW(str);
	params.squareSize =(float) _wtof(str);

	params.isSizeAuto = m_AutoChessSize.GetCheck();

	m_ChessSizeHeight.GetWindowTextW(str);
	int height = _ttoi(str);

	m_ChessSizeWidth.GetWindowTextW(str);
	int width = _ttoi(str);

	if (height < 2 || width < 2) {
		MessageBox(_T("棋盘尺寸不能小于2"), _T("参数设置"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	params.boardSize.width = width;
	params.boardSize.height = height;

	params.writeSettingFiles();

	CDialog::OnOK();
}


BOOL CSetParmasDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	params.readSettingFiles();
	CString tmp;
	tmp.Format(_T("%.2f"), params.squareSize);
	m_squareSize.SetWindowTextW(tmp);
	m_IsFixK3.SetCheck(params.IsFixK3);
	m_AutoChessSize.SetCheck(params.isSizeAuto);

	tmp.Format(_T("%d"), params.boardSize.height);
	m_ChessSizeHeight.SetWindowTextW(tmp);
	tmp.Format(_T("%d"), params.boardSize.width);
	m_ChessSizeWidth.SetWindowTextW(tmp);

	if (params.isSizeAuto)
	{
		m_ChessSizeHeight.EnableWindow(FALSE);
		m_ChessSizeWidth.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




void CSetParmasDialog::OnBnClickedAutochesssize()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	if (m_AutoChessSize.GetCheck())
	{
		m_ChessSizeHeight.EnableWindow(FALSE);
		m_ChessSizeWidth.EnableWindow(FALSE);
	}
	else
	{
		m_ChessSizeHeight.EnableWindow(TRUE);
		m_ChessSizeWidth.EnableWindow(TRUE);
	}
	UpdateData(TRUE);
}
