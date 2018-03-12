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
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();
	CRect rect;
	GetClientRect(&rect);
	CDockablePane::SetMinSize(CSize(rect.Width()/4,rect.Height()));
	return 0;
}




void CFileView::setRootItem()
{
	hRoot = m_wndFileView.InsertItem(_T("棋盘图像"), 0, 0);
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
	CPaintDC dc(this); // 用于绘制的设备上下文

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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
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
	LPCTSTR szFileFilter = _T("图片(*.jpg,*.tif)|*.jpg;*.tif|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, szFileFilter);

	const int nMaxFiles = 100;  //最大文件个数
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
				AfxMessageBox(_T("图片数量不足"));
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
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->hwnd == m_wndFileView && pMsg->message == WM_LBUTTONDBLCLK)
		PostMessage(WM_LBUTTONDBLCLK);

	return CDockablePane::PreTranslateMessage(pMsg);
}


void CFileView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndFileView;
	CPoint ptCurSel(0, 0);

	// 通过鼠标在客户区中的位置
	GetCursorPos(&ptCurSel);
	pWndTree->ScreenToClient(&ptCurSel);

	// 获取当前选择的是哪一项
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

		CcalibratorModuleView* active =(CcalibratorModuleView*)GetTopLevelFrame()->GetActiveView();//获取当前视类指针

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

	HTREEITEM hRootVaild = m_wndFileView.InsertItem(_T("可用棋盘图像"), 0, 0);
	m_wndFileView.SetItemState(hRootVaild, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hRootInvaild = m_wndFileView.InsertItem(_T("不可用棋盘图像"), 0, 0);
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