
// photosUndistortDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CphotosUndistortDlg �Ի���



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


// CphotosUndistortDlg ��Ϣ�������

BOOL CphotosUndistortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CphotosUndistortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CphotosUndistortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//������Ƭ
void CphotosUndistortDlg::OnBnClickedImportimages()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPCTSTR szFileFilter = _T("ͼƬ(*.jpg,*.tif)|*.jpg;*.tif|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, szFileFilter);

	const int nMaxFiles = 1000;  //����ļ�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("XML Files (*.xml)|*.xml||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //�ļ�����������FilePathName��

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
			MessageBox(_T("��ȡʧ��"), _T("��ƬУ��"), MB_OK | MB_ICONEXCLAMATION);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HWND hwnd = GetSafeHwnd();   //�õ����ھ��
	CString filePath =_T("");	//�õ��ļ�·��
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)	//ȡ��IMalloc�������ӿ�
	{
		BROWSEINFO bi;
		TCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = hwnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("ѡ���ļ���"); //ѡ��Ŀ¼�Ի�����ϲ��ֵı���
									//����½��ļ��а�ť BIF_NEWDIALOGSTYLE
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //ȡ��IMalloc�������ӿ�
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //���һ���ļ�ϵͳ·��
			{
				filePath = pszBuffer;
				undis.exportPath = CGeneralUtility::CStringToString(filePath);
				GetDlgItem(IDC_ExportPath)->SetWindowTextW(filePath);
				UpdateData(FALSE);
			}
			pMalloc->Free(pidl);	//�ͷ��ڴ�

		}
		pMalloc->Release();	//�ͷŽӿ�
	}
}


void CphotosUndistortDlg::OnBnClickedRunundistort()
{
	if (!this->canUse)
	{
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (undis.imageNameList.size() == 0)
		return;
	this->canUse = false;

	CString input = CGeneralUtility::StringToCString(undis.imageNameList[0]);
	input=input.Left(input.ReverseFind(_T('\\')) + 1);
	CString output = CGeneralUtility::StringToCString(undis.exportPath);
	if (input == output)
	{
		MessageBox(_T("����·�����������·����ͬ"), _T("��ƬУ��"), MB_OK | MB_ICONEXCLAMATION);
	}

	HWND hwnd = GetSafeHwnd();


	//AfxGetMainWnd()->BeginWaitCursor();//���õȴ����


	this->numImage = undis.getImageNum();
	m_Progress.SetRange(0, numImage);
	m_Progress.SetPos(0);


	if(!undis.runUndistort(hwnd))
	{
		MessageBox(_T("��Ƭ�ߴ�ͱ궨�����ļ�����ĳߴ粻��"), _T("��ƬУ��"), MB_OK | MB_ICONEXCLAMATION);
	}


	
	//AfxGetMainWnd()->EndWaitCursor();//�����ȴ����
	
}


LRESULT CphotosUndistortDlg::updateProgress(WPARAM wParam, LPARAM lParam)
{
	//int step = (int)lParam;
	this->currentImage += 1;
	m_Progress.SetPos(this->currentImage);

	if (this->currentImage == this->numImage)
	{
		GetDlgItem(IDC_UndistortOK)->SetWindowTextW(_T("���"));
		this->canUse = true;
	}

	return 0L;
}