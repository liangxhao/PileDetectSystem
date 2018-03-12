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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "calibratorModule.h"

#include "MainFrm.h"
#include "SetParmasDialog.h"
#include "ParamsFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_ImoprtData, &CMainFrame::OnImoprtdata)
	ON_COMMAND(ID_Detect, &CMainFrame::OnDetect)
	ON_COMMAND(ID_SELECT, &CMainFrame::OnSelect)
	ON_COMMAND(ID_Run, &CMainFrame::OnRun)
	ON_COMMAND(ID_PARAMS, &CMainFrame::OnParams)
	ON_COMMAND(ID_EXPORT, &CMainFrame::OnExport)


	ON_MESSAGE(WM_PROCESS_STEP, &CMainFrame::updateProgress)
	
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	//CMFCRibbonQuickAccessToolBar* pQAToolbar = m_wndRibbonBar.GetQuickAccessToolbar();
	//pQAToolbar->RemoveAll();


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	//CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	//ASSERT(bNameValid);

	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	
	
	//设置进度条
	CRect rect;
	GetClientRect(&rect);

	m_Progress = new CMFCRibbonProgressBar(5000, rect.Width() / 3);

	m_wndStatusBar.AddElement(m_Progress, _T(""));

	m_Progress->SetRange(0, 0);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	//CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;



	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	return 0;
}




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = _T("相机标定");


	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu* pMenuPopup)
{
	// TODO: 在此添加专用代码和/或调用基类
	return 0;
	//return CFrameWndEx::OnShowPopupMenu(pMenuPopup);
}


void CMainFrame::OnImoprtdata()
{
	m_wndFileView.imoprtChessBoardImages();
}


void CMainFrame::OnDetect()
{
	if (!this->canUse)
	{
		return;
	}

	if (m_wndFileView.m_calib.imageNameList.size() == 0)
		return;
	
	HWND hwnd = GetSafeHwnd();


	this->numImage = m_wndFileView.m_calib.getNumImage();
	m_Progress->SetRange(0, this->numImage+1);
	m_Progress->SetPos(0);
	this->canUse = false;

	m_wndFileView.m_calib.runDetect(hwnd);


}

void CMainFrame::OnSelect()
{
	if (!this->canUse)
	{
		return;
	}

	// TODO: 在此添加命令处理程序代码
	if (m_wndFileView.m_calib.boardSizeList.size() == 0)
		return;

	CParamsFiles parms;
	parms.readSettingFiles();
	m_wndFileView.m_calib.IsSizeAuto = parms.isSizeAuto == 1;
	if (!m_wndFileView.m_calib.IsSizeAuto)
	{
		m_wndFileView.m_calib.boardSize = parms.boardSize;
	}

	m_wndFileView.m_calib.autoGetChessSize();
	m_wndFileView.showVaildImagesTree();

}


void CMainFrame::OnParams()
{
	// TODO: 在此添加命令处理程序代码
	CSetParmasDialog dlg;
	dlg.DoModal();
}

void CMainFrame::OnRun()
{
	if (!this->canUse)
	{
		return;
	}

	if (m_wndFileView.m_calib.imagePointsList.size() == 0)
	{
		return;
	}

	int count = 0;
	for (const auto &p : m_wndFileView.m_calib.imageIsVaild)
	{
		if (p)
			count++;
	}

	if (count == 0) {
		MessageBox(_T("没有可用的棋盘点"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
	}

	CParamsFiles parms;
	parms.readSettingFiles();
	m_wndFileView.m_calib.squareSize = parms.squareSize;
	m_wndFileView.m_calib.IsFixK3 = parms.IsFixK3==1;

	
	HWND hwnd = GetSafeHwnd();
	m_wndFileView.m_calib.runCalibration(hwnd);


}


void CMainFrame::OnExport()
{
	if (!this->canUse)
	{
		return;
	}

	// TODO: 在此添加命令处理程序代码
	if (m_wndFileView.m_calib.rms < 0)
		return;


	CString strFilter = _T("XML文件(*.xml)|*.xml||");
	CFileDialog dlg(FALSE, NULL, _T("calibrator.xml"), NULL, strFilter);
	if (IDOK != dlg.DoModal())
		return;    
	CString strFileName;
	strFileName = dlg.m_ofn.lpstrFile;

	CString ext=strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('.') - 1).MakeLower();
	if (ext != _T("xml"))
	{
		strFileName = strFileName + _T(".xml");
	}
  

	String f = CGeneralUtility::CStringToString(strFileName);
	CParamsFiles parms;
	parms.readSettingFiles();
	if (parms.writeCalibratorFiles(f,
		m_wndFileView.m_calib.imageSize, m_wndFileView.m_calib.boardSize,
		m_wndFileView.m_calib.cameraMatrix, m_wndFileView.m_calib.distCoeffs,
		m_wndFileView.m_calib.rms, m_wndFileView.m_calib.totalAvgErr,
		m_wndFileView.m_calib.reprojErrs))
	{
		MessageBox(_T("导出成功"), _T("相机标定"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("导出失败"), _T("相机标定"), MB_OK | MB_ICONEXCLAMATION);
	}
	
}


LRESULT CMainFrame::updateProgress(WPARAM wParam, LPARAM lParam)
{
	int flag = (int)lParam;
	if (flag == 1)
	{
		this->currentStep += 1;
		m_Progress->SetPos(this->currentStep);

		if (this->currentStep == this->numImage + 1)
		{
			this->canUse = true;
			//m_Progress->SetPos(m_Progress->GetRangeMax());
		}

	}

	else
	{
		if (m_wndFileView.m_calib.calibrationSucess)
		{
			CString str;
			str.Format(_T("计算完成\nRMS: %.6f\nAvg Reprojection Errors：%.6f"),
				m_wndFileView.m_calib.rms, m_wndFileView.m_calib.totalAvgErr);

			MessageBox(str, _T("标定结果"), MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_T("计算失败，请检查您的数据"), _T("标定结果"), MB_OK | MB_ICONEXCLAMATION);
		}
	}


	return 0L;
}
