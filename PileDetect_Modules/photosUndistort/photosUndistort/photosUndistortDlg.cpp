
// photosUndistortDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "photosUndistort.h"
#include "photosUndistortDlg.h"
#include "afxdialogex.h"
#include "GeneralUtility.h"
#include "ParamsFiles.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CphotosUndistortDlg 对话框



CphotosUndistortDlg::CphotosUndistortDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PHOTOSUNDISTORT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CphotosUndistortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

BEGIN_MESSAGE_MAP(CphotosUndistortDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ImportImages, &CphotosUndistortDlg::OnBnClickedImportimages)
	ON_BN_CLICKED(IDC_ImportParms, &CphotosUndistortDlg::OnBnClickedImportparms)
	ON_BN_CLICKED(IDC_SetExport, &CphotosUndistortDlg::OnBnClickedSetexport)
	ON_BN_CLICKED(IDC_RunUndistort, &CphotosUndistortDlg::OnBnClickedRunundistort)

	ON_MESSAGE(WM_PROCESS_STEP, &CphotosUndistortDlg::updateProgress)
END_MESSAGE_MAP()


// CphotosUndistortDlg 消息处理程序

BOOL CphotosUndistortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CphotosUndistortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CphotosUndistortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CphotosUndistortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//导入相片
void CphotosUndistortDlg::OnBnClickedImportimages()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCTSTR szFileFilter = _T("图片(*.jpg,*.tif)|*.jpg;*.tif|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, szFileFilter);

	const int nMaxFiles = 1000;  //最大文件个数
	const int nMaxPathBuffer = (nMaxFiles * (MAX_PATH + 1)) + 1;
	LPWSTR pc = (LPWSTR)malloc(nMaxPathBuffer * sizeof(WCHAR));

	if (pc)
	{
		dlg.GetOFN().lpstrFile = pc;
		dlg.GetOFN().lpstrFile[0] = NULL;
		dlg.m_ofn.nMaxFile = nMaxPathBuffer;

		if (dlg.DoModal() == IDOK)
		{
			undis.imageNameList.clear();

			POSITION pos = dlg.GetStartPosition();

			while (pos != NULL)
			{
				CString fullName = dlg.GetNextPathName(pos);

				undis.imageNameList.push_back(CGeneralUtility::CStringToString(fullName));

			}

			int num = (int) undis.imageNameList.size();
			CString str;
			str.Format(_T("%d Photos"), num);
			GetDlgItem(IDC_PhotoNum)->SetWindowTextW(str);
			UpdateData(FALSE);
		}
	}
}


void CphotosUndistortDlg::OnBnClickedImportparms()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("XML Files (*.xml)|*.xml||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里

		String filename = CGeneralUtility::CStringToString(FilePathName);

		CParamsFiles parms;
		if (parms.readCalibratorFiles(filename, undis.imageSize, undis.cameraMatrix, undis.distCoeffs))
		{
			CString f;
			f= FilePathName.Right(FilePathName.GetLength() - FilePathName.ReverseFind('\\') - 1);
		
			GetDlgItem(IDC_ParmsName)->SetWindowTextW(f);
			UpdateData(FALSE);
		}
		else
		{
			MessageBox(_T("读取失败"), _T("相片校正"), MB_OK | MB_ICONEXCLAMATION);
		}

	}
	else
	{
		return;
	}

}




void CphotosUndistortDlg::OnBnClickedSetexport()
{

	if (!this->canUse)
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码

	HWND hwnd = GetSafeHwnd();   //得到窗口句柄
	CString filePath =_T("");	//得到文件路径
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)	//取得IMalloc分配器接口
	{
		BROWSEINFO bi;
		TCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = hwnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("选择文件夹"); //选择目录对话框的上部分的标题
									//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //取得IMalloc分配器接口
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //获得一个文件系统路径
			{
				filePath = pszBuffer;
				undis.exportPath = CGeneralUtility::CStringToString(filePath);
				GetDlgItem(IDC_ExportPath)->SetWindowTextW(filePath);
				UpdateData(FALSE);
			}
			pMalloc->Free(pidl);	//释放内存

		}
		pMalloc->Release();	//释放接口
	}
}


void CphotosUndistortDlg::OnBnClickedRunundistort()
{
	if (!this->canUse)
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	if (undis.imageNameList.size() == 0)
		return;
	this->canUse = false;

	CString input = CGeneralUtility::StringToCString(undis.imageNameList[0]);
	input=input.Left(input.ReverseFind(_T('\\')) + 1);
	CString output = CGeneralUtility::StringToCString(undis.exportPath);
	if (input == output)
	{
		MessageBox(_T("输入路径不能与输出路径相同"), _T("相片校正"), MB_OK | MB_ICONEXCLAMATION);
	}

	HWND hwnd = GetSafeHwnd();


	//AfxGetMainWnd()->BeginWaitCursor();//设置等待光标


	this->numImage = undis.getImageNum();
	m_Progress.SetRange(0, numImage);
	m_Progress.SetPos(0);


	if(!undis.runUndistort(hwnd))
	{
		MessageBox(_T("相片尺寸和标定参数文件定义的尺寸不符"), _T("相片校正"), MB_OK | MB_ICONEXCLAMATION);
	}


	
	//AfxGetMainWnd()->EndWaitCursor();//结束等待光标
	
}


LRESULT CphotosUndistortDlg::updateProgress(WPARAM wParam, LPARAM lParam)
{
	//int step = (int)lParam;
	this->currentImage += 1;
	m_Progress.SetPos(this->currentImage);

	if (this->currentImage == this->numImage)
	{
		GetDlgItem(IDC_UndistortOK)->SetWindowTextW(_T("完成"));
		this->canUse = true;
	}

	return 0L;
}