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

// MainFrm.cpp : CMainFrame ���ʵ��
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

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	//CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	//ASSERT(bNameValid);

	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	
	
	//���ý�����
	CRect rect;
	GetClientRect(&rect);

	m_Progress = new CMFCRibbonProgressBar(5000, rect.Width() / 3);

	m_wndStatusBar.AddElement(m_Progress, _T(""));

	m_Progress->SetRange(0, 0);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	//CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;



	// �������ڻ��������û�����Ԫ�ص��Ӿ�������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	return 0;
}




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = _T("����궨");


	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������


BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu* pMenuPopup)
{
	// TODO: �ڴ����ר�ô����/����û���
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

	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
		MessageBox(_T("û�п��õ����̵�"), _T("��ʾ"), MB_OK | MB_ICONEXCLAMATION);
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

	// TODO: �ڴ���������������
	if (m_wndFileView.m_calib.rms < 0)
		return;


	CString strFilter = _T("XML�ļ�(*.xml)|*.xml||");
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
		MessageBox(_T("�����ɹ�"), _T("����궨"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("����ʧ��"), _T("����궨"), MB_OK | MB_ICONEXCLAMATION);
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
			str.Format(_T("�������\nRMS: %.6f\nAvg Reprojection Errors��%.6f"),
				m_wndFileView.m_calib.rms, m_wndFileView.m_calib.totalAvgErr);

			MessageBox(str, _T("�궨���"), MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_T("����ʧ�ܣ�������������"), _T("�궨���"), MB_OK | MB_ICONEXCLAMATION);
		}
	}


	return 0L;
}
