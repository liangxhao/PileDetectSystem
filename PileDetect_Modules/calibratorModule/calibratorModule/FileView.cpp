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

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "calibratorModule.h"

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include "calibratorModuleView.h"

using namespace cv;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
//	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
//	ON_WM_SIZE()
ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillFileView();
	AdjustLayout();
	CRect rect;
	GetClientRect(&rect);
	CDockablePane::SetMinSize(CSize(rect.Width()/4,rect.Height()));
	return 0;
}




void CFileView::setRootItem()
{
	hRoot = m_wndFileView.InsertItem(_T("����ͼ��"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}


void CFileView::FillFileView()
{

	setRootItem();

	m_wndFileView.Expand(hRoot, TVE_EXPAND);

}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{

}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	imoprtChessBoardImages();

}


void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


BOOL CFileView::CanBeClosed() const {
	return FALSE;
}

BOOL CFileView::CanFloat() const
{
	return FALSE; 
}

void CFileView::imoprtChessBoardImages()
{
	LPCTSTR szFileFilter = _T("ͼƬ(*.jpg,*.tif)|*.jpg;*.tif|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, szFileFilter);

	const int nMaxFiles = 100;  //����ļ�����
	const int nMaxPathBuffer = (nMaxFiles * (MAX_PATH + 1)) + 1;
	LPWSTR pc = (LPWSTR)malloc(nMaxPathBuffer * sizeof(WCHAR));

	if (pc)
	{
		dlg.GetOFN().lpstrFile = pc;
		dlg.GetOFN().lpstrFile[0] = NULL;
		dlg.m_ofn.nMaxFile = nMaxPathBuffer;

		if (dlg.DoModal() == IDOK)
		{

			clearLastImport();
			setRootItem();

			POSITION pos = dlg.GetStartPosition();

			while (pos != NULL)
			{
				CString fullName = dlg.GetNextPathName(pos);

				m_imageListName.push_back(CGeneralUtility::CStringToString(fullName));

			}

			if (!m_imgManage.isVaild(m_imageListName))
			{
				m_imageListName.clear();
				AfxMessageBox(_T("ͼƬ��������"));
				return;
			}

			for (int i = 0; i < m_imageListName.size(); i++)
			{
				CString strName = m_imgManage.getFileName(CGeneralUtility::StringToCString(m_imageListName[i]));
				m_wndFileView.InsertItem(strName, 1, 1, hRoot);
			}
			m_calib.imageNameList.swap(m_imageListName);

			m_wndFileView.Expand(hRoot, TVE_EXPAND);

		}
	}
}

BOOL CFileView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->hwnd == m_wndFileView && pMsg->message == WM_LBUTTONDBLCLK)
		PostMessage(WM_LBUTTONDBLCLK);

	return CDockablePane::PreTranslateMessage(pMsg);
}


void CFileView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndFileView;
	CPoint ptCurSel(0, 0);

	// ͨ������ڿͻ����е�λ��
	GetCursorPos(&ptCurSel);
	pWndTree->ScreenToClient(&ptCurSel);

	// ��ȡ��ǰѡ�������һ��
	TVHITTESTINFO HitTestInfo;
	HitTestInfo.pt = ptCurSel;
	HTREEITEM hTreeItem = pWndTree->HitTest(&HitTestInfo);

	if (hTreeItem != NULL)
	{
		CString strItem = pWndTree->GetItemText(hTreeItem);
		CString fullName = m_imgManage.getFilePath() + "\\" + strItem;

		string imageFullName = CGeneralUtility::CStringToString(fullName);
		Mat img = imread(imageFullName);
		if (img.rows == 0 || img.cols == 0)
			return;

		m_calib.drawPointsOnImage(img, imageFullName,this->flag);

		CcalibratorModuleView* active =(CcalibratorModuleView*)GetTopLevelFrame()->GetActiveView();//��ȡ��ǰ����ָ��

		active->setImage(img);
	}


	CDockablePane::OnLButtonDown(nFlags, point);
}



void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}


void CFileView::showVaildImagesTree()
{
	m_wndFileView.DeleteAllItems();

	HTREEITEM hRootVaild = m_wndFileView.InsertItem(_T("��������ͼ��"), 0, 0);
	m_wndFileView.SetItemState(hRootVaild, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hRootInvaild = m_wndFileView.InsertItem(_T("����������ͼ��"), 0, 0);
	m_wndFileView.SetItemState(hRootInvaild, TVIS_BOLD, TVIS_BOLD);


	for (int i = 0; i < m_calib.imageNameList.size(); i++)
	{
		CString fname = m_imgManage.getFileName(CGeneralUtility::StringToCString(m_calib.imageNameList[i]));

		m_wndFileView.InsertItem(fname, 1, 1, m_calib.imageIsVaild[i]?hRootVaild:hRootInvaild);

	}
	m_wndFileView.Expand(hRootVaild, TVE_EXPAND);
	m_wndFileView.Expand(hRootInvaild, TVE_EXPAND);

	this->flag = 1;
}

void CFileView::clearLastImport()
{
	m_wndFileView.DeleteAllItems();
	m_calib.imageIsVaild.clear();
	m_calib.imageNameList.clear();
	m_calib.imagePointsList.clear();
	m_calib.boardSizeList.clear();

	m_imageListName.clear();
}