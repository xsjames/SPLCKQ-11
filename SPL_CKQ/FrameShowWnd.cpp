// FrameShowWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SPL_CKQ.h"
#include "FrameShowWnd.h"


// CFrameShowWnd

IMPLEMENT_DYNAMIC(CFrameShowWnd, CDockablePane)

CFrameShowWnd::CFrameShowWnd()
{

}

CFrameShowWnd::~CFrameShowWnd()
{
}


BEGIN_MESSAGE_MAP(CFrameShowWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_COMMAND(ID_COPYFRAME, &CFrameShowWnd::OnCopyframe)
END_MESSAGE_MAP()



// CFrameShowWnd 消息处理程序




int CFrameShowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	m_Font.CreatePointFont(94,_T("宋体"));
	CRect rectDummy;
	rectDummy.SetRectEmpty();  
	
	m_myTabCtrl.Create(TCS_TABS|WS_CHILD|WS_VISIBLE,rectDummy,this,IDC_TABLE_MYSHOWFRAME);
	m_myTabCtrl.SetFont(&m_Font); 
	m_myTabCtrl.InsertItem(0,_T("     拓扑信息     "));  
	m_myTabCtrl.InsertItem(1,_T("     报文记录     ")); 
	return 0;
}


void CFrameShowWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (GetSafeHwnd() == NULL)  
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient); 
	m_myTabCtrl.SetWindowPos(NULL, rectClient.left + 1, rectClient.top  + 1, rectClient.Width() - 1, rectClient.Height()  - 1, SWP_NOACTIVATE | SWP_NOZORDER);

}


