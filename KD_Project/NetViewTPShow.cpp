// NetViewTPShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KD_Project.h"
#include "NetViewTPShow.h"
#include "KD_ProjectDoc.h"
#include "SimJzq.h"
#include "MainFrm.h"
#include "DlgCCOTPchange.h"
#include "RightTabView.h"
#include "NetViewPToP.h"
#include "DlgCKQUpdataTime.h"
extern CSimJzq  gSimJzq;

// CNetViewTPShow

int g_ConnectCCO = 0;

#define TPSTART_X_SHIFT 100
#define TPSTART_Y_SHIFT 100


IMPLEMENT_DYNCREATE(CNetViewTPShow, CFormView)

CNetViewTPShow::CNetViewTPShow()
	: CFormView(CNetViewTPShow::IDD)
{
	m_fFont = 15;
	m_Scale = 1.0f;
	m_cR = 20;
	m_cRW = 200;
	m_cTBWeith = 50;
	m_cLRWeith = 10;
	m_nAlldataNumb = 0;
	m_nLineType = 0;
	m_nMsgType = 1;
	m_bSave = false;
	m_bAutoUpdata = false;
	m_bShow = false;
	m_nUpdataTime = 0;
	for(int m = 0 ; m < 10 ; m++)
	{
		m_nOnlineNumb[m] = 0;
	}
	m_bBreak = true;
	
	m_topo_jzq_port =! g_ConnectCCO;

	//m_topo_jzq_port = 0;
}

CNetViewTPShow::~CNetViewTPShow()
{
	m_bBreak = false;
	Sleep(100);
}

void CNetViewTPShow::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetViewTPShow, CFormView)
	ON_WM_MOUSEWHEEL()
ON_WM_RBUTTONUP()
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_RBTP_UPDATA, &CNetViewTPShow::OnRbtpUpdata)
//ON_WM_CREATE()
ON_COMMAND(ID_COO_CHANGE, &CNetViewTPShow::OnCooChange)
ON_COMMAND(ID_LINE_ZHI, &CNetViewTPShow::OnLineZhi)
ON_COMMAND(ID_LINE_ZHIJIAO, &CNetViewTPShow::OnLineZhijiao)
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_RBTPJDMSG_FINDLIST, &CNetViewTPShow::OnRbtpjdmsgFindlist)
ON_COMMAND(IDR_MENU_RBTP_SHOW, &CNetViewTPShow::OnIdrMenuRbtpShow)
ON_COMMAND(IDR_MENU_RBTP_HEID, &CNetViewTPShow::OnIdrMenuRbtpHeid)
ON_COMMAND(ID_RBTP_TEST, &CNetViewTPShow::OnRbtpTest)
ON_COMMAND(ID_RBTPJDMSG_PTP, &CNetViewTPShow::OnRbtpjdmsgPtp)
//ON_WM_CHAR()
//ON_WM_KEYDOWN()
ON_COMMAND(ID_RBTP_AUTOUPDATA, &CNetViewTPShow::OnRbtpAutoupdata)
ON_COMMAND(ID_RBTP_SAVE, &CNetViewTPShow::OnRbtpSave)
ON_COMMAND(PORT_JZQ, &CNetViewTPShow::Select_JZQ_Port)
ON_COMMAND(PORT_CKQ, &CNetViewTPShow::OnCkq)
END_MESSAGE_MAP()


// CNetViewTPShow ���

#ifdef _DEBUG
void CNetViewTPShow::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetViewTPShow::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CNetViewTPShow ��Ϣ��������
//��ʼ������
//ִ���߳�
DWORD WINAPI ThreadSendTP (PVOID pParam) 
{
	CMainFrame * pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CNetViewTPShow * pView = (CNetViewTPShow *)pParam;
	INT16U u16temp = 0 , u16BeginNumb = 0;
	INT8U SendDatabuf[10];
	INT8U ack = 0 , u8SendAFN = 0 ,AddrBuf[7] , u8ReadCount = 0;
	INT16U SendDatalen = 0 ,temp16 = 0;
	CString strAllTPdata = _T("");
   // if (pMain->NW_OR_GW==1){ //������׼
    if(0){
		sPartQCSG376_2CreatFrame ptSendQCSG376_2;
		sPartQCSG376_2DeCodeFrame ptRecvQCSG376_2;

		while(pView->m_bBreak)
		{
			if (pView->m_bAutoUpdata == true)
			{
				if (pView->m_nUpdataTime == 0)
				{
					pView->m_bAutoUpdata = false;
				}
				if (pMain->m_bConnected != TRUE)
				{
					AfxMessageBox(_T("���ȴ򿪴��ڣ�"));
                    #if (0==LOOPTEST)
					continue;
                    #endif
				}
				if (pMain->m_strMAddress == "")
				{
					AfxMessageBox(_T("��������CCO��"));
					#if (0==LOOPTEST)
			 			continue;
					#endif
				}
				u16BeginNumb = 0;
				SendDatalen = 0;
				u8ReadCount = 0;
				//strAllTPdata = pMain->m_strMAddress + _T("0100000021000000");
				pView->m_tools._str16tobuf16( pMain->m_strMAddress , AddrBuf , temp16 , true);
				//ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf , 0xF0 , FN_F0_F10 , SendDatabuf , SendDatalen , pView->m_ptSendQCSG376_2 , ptRecvQCSG376_2 , pView->m_ptRecvQCSG376_2);
				switch(ack)
				{
				case 0x00://�ɹ�
					temp16 = pView->m_ptRecvQCSG376_2.s_RcvDataBuf[1] << 8 | pView->m_ptRecvQCSG376_2.s_RcvDataBuf[0];
					while(temp16)
					{
						u16BeginNumb += u8ReadCount;
						if (temp16 >20)
						{
							u8ReadCount = 20;
						}
						else
						{
							u8ReadCount = temp16;
						}
						temp16 -= u8ReadCount;
						SendDatalen = 3;
						SendDatabuf[0] = (INT8U)u16BeginNumb;
						SendDatabuf[1] = (INT8U)(u16BeginNumb>>8);
						SendDatabuf[2] = u8ReadCount;
						//ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf , 0xF0 , FN_F0_F9 , SendDatabuf , SendDatalen , pView->m_ptSendQCSG376_2, ptRecvQCSG376_2 , pView->m_ptRecvQCSG376_2);
						switch(ack)
						{
						case 0x00://�ɹ�
							strAllTPdata += pView->m_tools._buf16tostr16(&pView->m_ptRecvQCSG376_2.s_RcvDataBuf[5] , pView->m_ptRecvQCSG376_2.s_RcvDataLen -5);
							break;
						}
					}
					pView->SetTPdataToAccess(strAllTPdata);
					pView->GetPointCoordinate();
					break;
				}
				Sleep(pView->m_nUpdataTime * 1000);
			}
			Sleep(200);
		}
	}
	else{
		sPartQGDW376_2CreatFrame ptSendQGDW376_2;
		sPartQGDW376_2DeCodeFrame ptRecvQGDW376_2;
		//AfxMessageBox(_T("read topo"));
		while(pView->m_bBreak)
		{
			if (pView->m_bAutoUpdata == true)
			{
				if (pView->m_nUpdataTime == 0)
				{
					pView->m_bAutoUpdata = false;
				}
				if (pMain->m_bConnected != TRUE)
				{
					//AfxMessageBox(_T("���ȴ򿪴��ڣ�"));
                    #if (0==LOOPTEST)
					continue;
                    #endif
				}
				if (pMain->m_strMAddress == "")
				{
					//AfxMessageBox(_T("��������CCO��"));
					#if (0==LOOPTEST)
			         continue;
			        #endif
					
				}
				strAllTPdata = _T("");
				u16BeginNumb = 1;
				SendDatalen = 0;
				u8ReadCount = 0;
                SendDatabuf[0]=1;
                SendDatabuf[1]=0;
                SendDatabuf[2]=1;
                SendDatalen=3;
				//strAllTPdata = pMain->m_strMAddress + _T("0100000021000000");
				pView->m_tools._str16tobuf16( pMain->m_strMAddress , AddrBuf , temp16 , true);

				if (pView->m_topo_jzq_port)
					ack = gSimJzq.HostSendRcv376_2Buf( 0x10 , F21 , SendDatabuf , SendDatalen , pView->m_ptSendQGDW376_2 , pView->m_ptRecvQGDW376_2);
				else
					ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf, 0x10, F21, SendDatabuf, SendDatalen, pView->m_ptSendQGDW376_2, ptRecvQGDW376_2, pView->m_ptRecvQGDW376_2);\

				switch(ack)
				{
				case 0x00://�ɹ�
					temp16 = pView->m_ptRecvQGDW376_2.s_RcvDataBuf[1] << 8 | pView->m_ptRecvQGDW376_2.s_RcvDataBuf[0];
					int times = 0;
					while(temp16)
					{

						if (temp16 > 1016)
							break;

						u16BeginNumb += u8ReadCount;
						if (temp16 >50)
						{
							u8ReadCount = 50;
						}
						else
						{
							u8ReadCount = temp16;
						}
						temp16 -= u8ReadCount;
						SendDatalen = 3;
						SendDatabuf[0] = (INT8U)u16BeginNumb;
						SendDatabuf[1] = (INT8U)(u16BeginNumb>>8);
						SendDatabuf[2] = u8ReadCount;
						

						if (pView->m_topo_jzq_port)
							ack = gSimJzq.HostSendRcv376_2Buf( 0x10 , F21 , SendDatabuf , SendDatalen , pView->m_ptSendQGDW376_2 , pView->m_ptRecvQGDW376_2);
						else
							ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf, 0x10, F21, SendDatabuf, SendDatalen, pView->m_ptSendQGDW376_2, ptRecvQGDW376_2, pView->m_ptRecvQGDW376_2);

						switch(ack)
						{
						case 0x00://�ɹ�
						    
							strAllTPdata += pView->m_tools._buf16tostr16(&pView->m_ptRecvQGDW376_2.s_RcvDataBuf[5] , pView->m_ptRecvQGDW376_2.s_RcvDataLen -5);

                            
							break;
						}
						TRACE("\n tmpnode %d\n",temp16);
						if(times++ > 21)
							break;
					}
				

					//strAllTPdata = _T("111111111111010001004095021107170003001B001293021107170004001B0022900211071700050004001379021107170006001B0012630811071700070008001596021107170008000A002461081107170009000400136608110717000A000400236208110717000B000C00138902110717000C001B00225808110717000D000C00138802110717000E001200159702110717000F0004001394021107170010000A001467081107170011000A001460081107170012000A002459081107170013000A00246408110717001400170017650811071700150018001630061107170016001700172906110717001700180026330611071700180013002535061107170019001300158002110717001A001700179202110717001B00010021");

					pView->SetTPdataToAccess(strAllTPdata);
					pView->GetPointCoordinate();
					break;
				}
				Sleep(pView->m_nUpdataTime * 1000);
			}
			Sleep(200);
		}
	}	
	return 0;
}
void CNetViewTPShow::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CMainFrame * pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CRect rect;
	GetClientRect(rect); 
	m_cx=rect.right;
	m_cy=rect.bottom;
	SetScrollSizes(IDD_DIALOG_TUOPUSHOW , CSize(m_cx, m_cy));
	m_gw13762.SetMainAdd(pMain->m_HostNodeAddress);
//#ifdef _STD_CSG  //������׼
	m_csg13762.SetMainAdd(pMain->m_HostNodeAddress);
//#endif
	gSimJzq.SetMainAdd(pMain->m_HostNodeAddress);
	m_RrushFont.CreateSolidBrush(RGB(128,0,64));//�����ɫ
	m_BrushAll[2].CreateSolidBrush(RGB(255,0,0));//CC0
	m_BrushAll[3].CreateSolidBrush(RGB(255,128,128));//U_CCO
	m_BrushAll[4].CreateSolidBrush(RGB(0,255,0));//STA
	m_BrushAll[5].CreateSolidBrush(RGB(128,255,128));//U_STA
	m_BrushAll[6].CreateSolidBrush(RGB(255,128,64));//PCO
	m_BrushAll[7].CreateSolidBrush(RGB(192,192,192));//����
	m_cpenXu.CreatePen(PS_SOLID,1,RGB(128,0,64));//�ߵ���ɫ
	int m = 0 ;
	for(m = 0 ; m < MODUMAXNUMB ; m++)
	{
		m_nPointColor[m] = 0;
		m_fPointWeith[m] = 0;
		m_fCoordinate[m][0] = 0;
		m_fCoordinate[m][1] = 0;
		m_strlist[m] = _T("");
		m_nPointLine[m][0] = 0;
		m_nPointLine[m][1] = 0;
		m_nPointNumb[m] = 0;
	}
	for(m = 0 ; m < 10 ; m++)
	{
		m_nOnlineNumb[m] = 0;
	}
	m_TPShowFindList.Create(IDD_DIALOG_FINDLIST,this);
	m_TPDlgJDMsg.Create(IDD_DIALOG_TPSHOW_JDMSG,this);
	m_TPDlgJDMsg.SetWindowPos(NULL,0,0,1,1,SWP_HIDEWINDOW);
	m_TPDlgShowExplian.Create(IDD_DIALOG_TPSHOW_EXPLIAN,this);
	m_TPDlgShowExplian.SetWindowPos(NULL,rect.right - 220,rect.top + 50, 200 ,410,SWP_HIDEWINDOW);


	

	m_hThreadsend=CreateThread (NULL,0,ThreadSendTP,this,0,NULL);
	CloseHandle(m_hThreadsend);
}
//��ͼ
void CNetViewTPShow::OnDraw(CDC* pDC)
{
	
	int n = 0;
	pDC->SelectObject(&m_cpenXu);//����
	if (m_nAlldataNumb != 0)
	{
		CRect rect;
		GetClientRect(rect); 
		m_TPDlgShowExplian.MoveWindow(rect.right - 220,rect.top + 50, 200 ,410);
	}
	for (n = 0; n<m_nAlldataNumb ;n++)
	{
		
		if(m_nLineType == 0)
		{
			if (m_nPointLine[n][1] != 0)
			{
				pDC->MoveTo(m_fCoordinate[m_nPointLine[n][0]][0],m_fCoordinate[m_nPointLine[n][0]][1]);
				pDC->LineTo(m_fCoordinate[m_nPointLine[n][1]][0],m_fCoordinate[m_nPointLine[n][1]][1]);
			}
		}
		else
		{
			if (m_nPointLine[n][1] != 0)
			{
				pDC->MoveTo(m_fCoordinate[m_nPointLine[n][0]][0],m_fCoordinate[m_nPointLine[n][0]][1]);
				pDC->LineTo(m_fCoordinate[m_nPointLine[n][0]][0],(m_fCoordinate[m_nPointLine[n][0]][1] - m_cTBWeith*4/5));
				pDC->LineTo(m_fCoordinate[m_nPointLine[n][1]][0],(m_fCoordinate[m_nPointLine[n][0]][1] - m_cTBWeith*4/5));
				pDC->LineTo(m_fCoordinate[m_nPointLine[n][1]][0],m_fCoordinate[m_nPointLine[n][1]][1]);
			}
		}
		
	}
	pDC->BeginPath();
	for (n = 1; n<m_nAlldataNumb + 1 ;n++)//��Բ�νڵ�
	{
		pDC->SelectObject(&m_BrushAll[m_nPointColor[m_nPointNumb[n-1]]]); 
		pDC->EndPath(); 
		pDC->Ellipse(m_fCoordinate[m_nPointNumb[n-1]][0] - m_cR,m_fCoordinate[m_nPointNumb[n-1]][1] - m_cR,m_fCoordinate[m_nPointNumb[n-1]][0] + m_cR,m_fCoordinate[m_nPointNumb[n-1]][1] + m_cR); 
	}

	pDC->SelectObject(&m_RrushFont);
	pDC->SetBkMode(TRANSPARENT); 
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = -MulDiv(m_fFont, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = FW_NORMAL;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	CFont newFont;
	newFont.CreateFontIndirect(&lf);
	pDC->BeginPath();
	pDC->SelectObject(&newFont);
	CString str;
	float fFont = 0;
	for (n = 1; n<m_nAlldataNumb + 1 ;n++)//�ڵ��� 
	{
		str.Format(_T("%d") , m_nPointLine[n - 1][0]);
		switch(str.GetLength())
		{
		case 1:
			fFont = m_cR/4;
			break;
		case 2:
			fFont = m_cR/2;
			break;
		case 3:
			fFont = m_cR*6/7;
			break;
		}
		pDC->TextOut(m_fCoordinate[m_nPointNumb[n-1]][0]- fFont, m_fCoordinate[m_nPointNumb[n-1]][1]- m_cR/2,str);
	}
	pDC->EndPath();
	pDC->FillPath(); 
}


//��ڵ�����
void CNetViewTPShow::GetPointCoordinate(CString strTableName)
{
	CMainFrame* pMain= (CMainFrame*)AfxGetApp()->GetMainWnd();
	CString strAllDatalist[MODUMAXNUMB][6];
	int nNumb = 0 , nAlldataNumb;
	CString strWhere = _T("");
	CString strAll = _T("");//���нڵ�Ĳ㼶����"$"���
	int nCeng = 15;//������
	CString strCeng = _T("");
	int n = 0 , m = 0 , i = 0;
	int nChildNumb = 0;
	float fChildWeith = 0;
	float fWeith[MODUMAXNUMB];
	//ͬһ���ͽڵ��������
	for(m = 0 ; m < 10 ; m++)
	{
		m_nOnlineNumb[m] = 0;
	}
	//�����ݿ�����ȡ������Ϣ
	m_access.SelectDataFromTable(pMain->m_PeiZhi_db , strTableName , _T("TP_MAC,TP_TEI,TP_PTEI,TP_ROLE,TP_TIER,TP_READSUCCESS") ,_T("") ,m_strlist ,nAlldataNumb);
	//m_access.SelectDataFromTable(_T(".\\�����ļ�\\13762Data.accdb") , strTableName , _T("TP_MAC,TP_TEI,TP_PTEI,TP_ROLE,TP_TIER,TP_READSUCCESS") ,_T("") ,m_strlist ,nAlldataNumb);
	m_nAlldataNumb = nAlldataNumb;
	for(n = 0 ; n < nAlldataNumb ; n++)
	{
		m_tools.Split(m_strlist[n] , _T("$") ,strAllDatalist[n] , nNumb);
	}

	//ͳ��ͬһ���ͽڵ��������ȡTEI��PTEI��Ϣ
	for(n = 0 ; n < nAlldataNumb ; n++)
	{
		switch(_ttoi(strAllDatalist[n][3]))
		{
		case 2:
			m_nOnlineNumb[0] += 1;
			break;
		case 3:
			m_nOnlineNumb[1] += 1;
			break;
		case 4:
			m_nOnlineNumb[2] += 1;
			break;
		case 5:
			m_nOnlineNumb[3] += 1;
			break;
		case 6:
			m_nOnlineNumb[4] += 1;
			break;
		case 7:
			m_nOnlineNumb[5] += 1;
			break;
		}
		m_nPointNumb[n] = _ttoi(strAllDatalist[n][1]);
		m_nPointLine[n][0] = _ttoi(strAllDatalist[n][1]);
		m_nPointLine[n][1] = _ttoi(strAllDatalist[n][2]);
		strAll += strAllDatalist[n][4] + _T("$");
	}
	//��ȡ�������㼶
	while(true)
	{
		strCeng.Format(_T("%d") , nCeng);
		if(strAll.Find(strCeng) != -1)
		{
			break;
		}
		nCeng -= 1;
	}

	//��ȡ�����ڵ㼰�ڵ����ռ����
	for(i = 0 ; i < nCeng; i++)
	{
		strCeng.Format(_T("%d") , nCeng-i);
		for(n = 0 ; n < nAlldataNumb ; n++)
		{
			nChildNumb = 0;
			fChildWeith = 0;
			//����߲㿪ʼ��ȡ
			if (strAllDatalist[n][4] == strCeng)
			{
				//��ȡ�Ӽ��ܿ���
				for(m = 0 ; m < nAlldataNumb ; m++)
				{
					if (strAllDatalist[n][1] == strAllDatalist[m][2])
					{
						nChildNumb++; //��nChildNumb���Ӽ�
						fChildWeith += m_fPointWeith[_ttoi(strAllDatalist[m][1])];
					}
				}
				//��ȡ�Լ��Ŀ���
				if (nChildNumb == 0)
				{
					m_fPointWeith[_ttoi(strAllDatalist[n][1])] = m_cR * 2 + m_cLRWeith;
				}
				else
				{
					m_fPointWeith[_ttoi(strAllDatalist[n][1])] = fChildWeith;
				}
			}
		}
	}

	
	//��ȡÿ���ڵ����꣬�ӵ�һ�㿪ʼ��ȡ
	for (i = 0 ; i < nCeng + 1 ; i++)
	{
		for(m = 0 ; m < MODUMAXNUMB ; m++)
		{
			fWeith[m] = 0;
		}
	
		for(n = 0 ; n < nAlldataNumb ; n++)
		{
			if (_ttoi(strAllDatalist[n][4]) == i)
			{
				m_nPointColor[_ttoi(strAllDatalist[n][1])] =  _ttoi(strAllDatalist[n][3]);//���ݽ�ɫƥ����ɫ
				m_fCoordinate[_ttoi(strAllDatalist[n][1])][1] = (i)* (m_cTBWeith + 2*m_cR)+TPSTART_Y_SHIFT;//��ȡ�ڵ�Y����
				//��ȡ�ڵ�X����
				if (_ttoi(strAllDatalist[n][4]) == 0)//����Ϊ1ʱΪ���ڵ�
				{
					m_fCoordinate[_ttoi(strAllDatalist[n][1])][0] = m_fPointWeith[_ttoi(strAllDatalist[n][1])]/2 + m_cLRWeith+TPSTART_X_SHIFT ;
				}
				else
				{
					m_fCoordinate[_ttoi(strAllDatalist[n][1])][0] = m_fCoordinate[_ttoi(strAllDatalist[n][2])][0] - m_fPointWeith[_ttoi(strAllDatalist[n][2])] / 2 + m_fPointWeith[_ttoi(strAllDatalist[n][1])] / 2 + fWeith[_ttoi(strAllDatalist[n][2])];
					fWeith[_ttoi(strAllDatalist[n][2])] += m_fPointWeith[_ttoi(strAllDatalist[n][1])];
				}
			}
		}
	}
	//���ݲ������ܿ�����ҳ���С
	if (nCeng* (m_cTBWeith + 2*m_cR) > m_cy)
	{
		m_cy = (nCeng + 1)* (m_cTBWeith + 2*m_cR);
	}
	if (m_fPointWeith[1] + m_cRW > m_cx)
	{
		m_cx = m_fPointWeith[1] + m_cRW;
	}

	//SetScrollSizes(IDD_DIALOG_TUOPUSHOW , CSize(m_cx, m_cy));
	m_TPDlgShowExplian.GetNumb(m_nOnlineNumb);//���¸���ģ�����
	Invalidate();//ˢ��
}
//���ֹ���������С����ͼ
BOOL CNetViewTPShow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	static float upLimit = 10.0f;
	static float downLinit = 0.04f;
	static float fUp = 1.1;
	static float fDown = 0.9f;
	bool bChange = true ;
	int n = 0;
	if(zDelta > 0 && m_Scale <= upLimit)
	{
		m_cx *= fUp;
		m_cy *= fUp;
		m_cR *= fUp;
		m_cTBWeith *= fUp;
		m_cLRWeith *= fUp;
		m_cRW *= fUp;
		m_fFont *= fUp;
		for( n = 1 ; n < m_nAlldataNumb + 1 ; n++)
		{
			m_fCoordinate[m_nPointNumb[n-1]][0] *= fUp;
			m_fCoordinate[m_nPointNumb[n-1]][1] *= fUp;
		}
		bChange = true;
	}
	else if(zDelta < 0 && m_Scale >= downLinit)
	{
		m_cx /= fUp;
		m_cy /= fUp;
		m_cR /= fUp;
		m_cTBWeith /= fUp;
		m_cLRWeith /= fUp;
		m_cRW /= fUp;
		if (m_fFont != 1)
		{
			m_fFont /= fUp;
		}
		for( n = 1 ; n < m_nAlldataNumb + 1 ; n++)
		{
			m_fCoordinate[m_nPointNumb[n-1]][0] /= fUp;
			m_fCoordinate[m_nPointNumb[n-1]][1] /= fUp;
		}
		bChange = false;
	}
	this->Invalidate();
	SetScrollSizes(IDD_DIALOG_TUOPUSHOW , CSize(m_cx, m_cy));
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}
//�Ҽ��˵�
void CNetViewTPShow::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	/*
	CString str;
	str.Format(_T("cco %d"),g_ConnectCCO);
	AfxMessageBox(str);
	*/
	if (g_ConnectCCO)
		m_topo_jzq_port = 0;
	else
		m_topo_jzq_port = 1;



	if (m_bShow)
	{
		VERIFY(menu.LoadMenu(IDR_MENU_RBTP_JDMSG));
	}
	else
	{
		VERIFY(menu.LoadMenu(IDR_MENU_RBTP));
	}
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	if (!m_bShow)
	{
		if (m_nLineType == 0)
		{
			pPopup->CheckMenuItem(ID_LINE_ZHI,MF_CHECKED);
			pPopup->CheckMenuItem(ID_LINE_ZHIJIAO,MF_UNCHECKED);
		}
		else
		{
			pPopup->CheckMenuItem(ID_LINE_ZHI,MF_UNCHECKED);
			pPopup->CheckMenuItem(ID_LINE_ZHIJIAO,MF_CHECKED);
		}

		if (m_nMsgType == 0)
		{
			pPopup->CheckMenuItem(IDR_MENU_RBTP_SHOW,MF_CHECKED);
			pPopup->CheckMenuItem(IDR_MENU_RBTP_HEID,MF_UNCHECKED);
		}
		else
		{
			pPopup->CheckMenuItem(IDR_MENU_RBTP_SHOW,MF_UNCHECKED);
			pPopup->CheckMenuItem(IDR_MENU_RBTP_HEID,MF_CHECKED);
		}

		if (m_bAutoUpdata)
		{
			pPopup->CheckMenuItem(ID_RBTP_AUTOUPDATA,MF_CHECKED);
		}
		else
		{
			pPopup->CheckMenuItem(ID_RBTP_AUTOUPDATA,MF_UNCHECKED);
		}
		if (m_bSave)
		{
			pPopup->CheckMenuItem(ID_RBTP_SAVE,MF_CHECKED);
		}
		else
		{
			pPopup->CheckMenuItem(ID_RBTP_SAVE,MF_UNCHECKED);
		}

		if (m_topo_jzq_port)
		{
			pPopup->CheckMenuItem(PORT_JZQ, MF_CHECKED);
			pPopup->CheckMenuItem(PORT_CKQ, MF_UNCHECKED);
		}
		else
		{
			pPopup->CheckMenuItem(PORT_CKQ, MF_CHECKED);
			pPopup->CheckMenuItem(PORT_JZQ, MF_UNCHECKED);
		}

	}
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
	
}

void CNetViewTPShow::SetTPdataToAccess(CString strTPdata)
{
	CMainFrame* pMain= (CMainFrame*)AfxGetApp()->GetMainWnd();
	INT8U SucceBuf[3];
	INT16U temp16 = 0;
	CString  strMAC ,strTEI ,strPTEI, strRole , strLayer , strSuccess;
	CTools tools;
	CString strlistName[6] = {_T("TP_MAC") ,_T("TP_TEI") ,_T("TP_PTEI") ,_T("TP_ROLE") ,_T("TP_TIER")  ,_T("TP_READSUCCESS")};
	CString strlistData[6];
	CTime timeNow = CTime::GetCurrentTime();
	CString strTime = timeNow.Format("%Y%m%d%H%M%S") , strName , strData;
	
	char szANSIString[5000]; 
	FILE* file_tpmap;
	CString strCSVTime = timeNow.Format("%Y%m%d%H%M%S");  //CSV ����

	if (m_bSave)
	{
		CString strNewlistName[6] = {_T("TP_MAC TEXT") ,_T("TP_TEI TEXT") ,_T("TP_PTEI TEXT") ,_T("TP_ROLE TEXT") ,_T("TP_TIER TEXT")  ,_T("TP_READSUCCESS TEXT")};
		strName = _T("HISTTPNAME");
		strData = strTime.Mid(2);
		m_access.AddDataToTable(_T(".\\�����ļ�\\HistTPShow.accdb") , _T("HISTTPNAME") , &strName , &strData ,1);
		m_access.CreatTable(_T(".\\�����ļ�\\HistTPShow.accdb") ,strData ,strNewlistName ,6);

//--------------CSV ����----------------------------------------
		strCSVTime = _T(".\\��־\\TOPMap") + strCSVTime + _T(".csv");
		int len =WideCharToMultiByte(CP_ACP,0,strCSVTime,strCSVTime.GetLength(),NULL,0,NULL,NULL);
		char * pFileName = new char[len];
		
		WideCharToMultiByte(CP_ACP,0,strCSVTime,strCSVTime.GetLength(),pFileName,len,NULL,NULL);
		pFileName[len] ='\0';
		
		m_tools.AutoMakedir(pFileName);

		file_tpmap=fopen(pFileName,"r");
		if  ( file_tpmap == NULL )
		{
			file_tpmap=fopen(pFileName,"a+");
		}
		else
		{
			fclose(file_tpmap); 
			file_tpmap=fopen(pFileName,"a+");
		} 
		CString strTopLine = _T("        TP_MAC,TP_TEI,TP_PTEI,TP_ROLE,TP_TIER, TP_READSUCCESS\n");
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strTopLine,-1,szANSIString,sizeof(szANSIString),NULL,NULL);
		fwrite(szANSIString,wcslen(strTopLine),1,file_tpmap);
//--------------------------------------------------------------
	}
	int nNumb = strTPdata.GetLength()/22;
	//m_access.DelectDataFromTable(_T(".\\�����ļ�\\13762Data.accdb"),_T("TPShow"),_T(""));
	m_access.DelectDataFromTable(pMain->m_PeiZhi_db,_T("TPShow"),_T(""));
	for(int n = 0 ; n < nNumb ; n++)
	{
		
		strMAC = strTPdata.Mid(0 , 12);
		strlistData[0] = tools._strDaoxu(strMAC);

		strTEI = strTPdata.Mid(12 , 4);
		strlistData[1] = tools._str16tostr10(tools._strDaoxu(strTEI));

		strPTEI = strTPdata.Mid(16 , 4);
		strlistData[2] = tools._str16tostr10(tools._strDaoxu(strPTEI));

		strRole = strTPdata.Mid(20 , 1);
		strlistData[3] = tools._str16tostr10(strRole);

        strLayer = strTPdata.Mid(21,1);

        
        
		strlistData[4] = tools._str16tostr10(strLayer);

		strlistData[5] = _T("");//m_tools._str16tostr10(strTPdata.Mid(22 , 2));


		if (strlistData[0].Mid(0,10) != "BEBEBEBEBE")
		{
			//m_access.AddDataToTable(pMain->m_PeiZhi_db,_T("TPShow"),strlistName , strlistData ,6);
			m_access.AddDataToTable(pMain->m_PeiZhi_db, _T("TPShow"), strlistName, strlistData, 5);
			//m_access.AddDataToTable(_T(".\\�����ļ�\\13762Data.accdb"),_T("TPShow"),strlistName , strlistData ,6);
		}

		if (m_bSave)
		{
			//m_access.AddDataToTable(_T(".\\�����ļ�\\HistTPShow.accdb") ,strData ,strlistName ,strlistData ,6);
			m_access.AddDataToTable(_T(".\\�����ļ�\\HistTPShow.accdb"), strData, strlistName, strlistData, 5);
		}
//--------------CSV ����----------------------------------------
		if (m_bSave)
		{
			CString strTopLine = strlistData[0] + _T(",") + strlistData[1] + _T(",") + strlistData[2] + _T(",") + strlistData[3] + _T(",") + strlistData[4] + _T(",") + strlistData[5] + _T("\n");
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strTopLine,-1,szANSIString,sizeof(szANSIString),NULL,NULL);
			fwrite(szANSIString,wcslen(strTopLine),1,file_tpmap);
		}
//--------------------------------------------------------------

		strTPdata = strTPdata.Mid(22);
	}
//--------------CSV ����----------------------------------------
	if (m_bSave)
	{
		if  ( file_tpmap != NULL )
		{
			fclose(file_tpmap);
		}
	}
//--------------------------------------------------------------

}

//ˢ������ͼ
void CNetViewTPShow::OnRbtpUpdata()
{
	if (m_nUpdataTime == 0)
	{
		m_bAutoUpdata = true;
	}
}
//��������ͼ���ݿ�
void CNetViewTPShow::OnCooChange()
{
	CDlgCCOTPchange dlgCCOchange;
	dlgCCOchange.DoModal();
}
//ֱ������
void CNetViewTPShow::OnLineZhi()
{
	m_nLineType = 0;
	this->Invalidate();
}
//ֱ��������
void CNetViewTPShow::OnLineZhijiao()
{
	m_nLineType = 1;
	this->Invalidate();
}
//�ƶ������ʾ�ڵ���Ϣ
void CNetViewTPShow::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	int nX  = point.x , nY = point.y , m = 0;
	CClientDC dc(this); //��ȡ�豸
	OnPrepareDC( &dc ); //׼���豸
	dc.DPtoLP( &point ); //���ת��
	nX = point.x - nX;
	nY = point.y - nY;
	CString str = _T("");
	for(int n = 0 ; n < m_nAlldataNumb ; n++)
	{
		if ((point.x > (m_fCoordinate[m_nPointNumb[n]][0] - m_cR/2))&&(point.x < (m_fCoordinate[m_nPointNumb[n]][0] + m_cR/2))&&
			(point.y > (m_fCoordinate[m_nPointNumb[n]][1] - m_cR/2))&&(point.y < (m_fCoordinate[m_nPointNumb[n]][1] + m_cR/2)))
		{
			if (!m_bShow)
			{
				int nNumb = 0;
				CString strAllDatalist[MODUMAXNUMB][10];
				str.Format(_T("%d"),m_nPointNumb[n]);
				for(m = 0 ; m < m_nAlldataNumb ; m++)
				{
					m_tools.Split(m_strlist[m] , _T("$") ,strAllDatalist[m] , nNumb);
					if (strAllDatalist[m][1] == str) break;
				}
				if (strAllDatalist[m][0].Mid(0,10) == "BEBEBEBEBE")
				{
					strAllDatalist[m][3] += _T("(������)");
				}
				m_strPMessage = _T("MAC��ַ:") + strAllDatalist[m][0] + _T("\r\n") +
					_T("TEI:") + strAllDatalist[m][1] + _T("\r\n") +
					_T("����TEI:") + strAllDatalist[m][2] + _T("\r\n") +
					_T("��ɫ:") + strAllDatalist[m][3] + _T("\r\n") +
					_T("�㼶:") + strAllDatalist[m][4];// +_T("%");//+ _T("\r\n") + 
					 //_T("�����ɹ���:") + strAllDatalist[m][5] + _T("%");// + _T("\r\n") + 
					//  _T("STATUS:") + strAllDatalist[m][6];
				m_TPDlgJDMsg.SetWindowPos(NULL,m_fCoordinate[m_nPointNumb[n]][0] - nX,m_fCoordinate[m_nPointNumb[n]][1] - nY,140,105,SWP_SHOWWINDOW);
				m_TPDlgJDMsg.m_editMsg.SetWindowTextW(m_strPMessage);
				m_bShow = true;
				m_strChooseMAC = strAllDatalist[m][0];
			}
			CFormView::OnMouseMove(nFlags, point);
			return;
		}
	}
	m_bShow = false;
	m_TPDlgJDMsg.ShowWindow(SW_HIDE);
	CFormView::OnMouseMove(nFlags, point);
}


//�ڵ��Ҽ������б��˵�����¼�
void CNetViewTPShow::OnRbtpjdmsgFindlist()
{
	CRect rect;
	GetClientRect(rect); 
	m_TPShowFindList.SetWindowPos(NULL,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);
	m_TPShowFindList.ShowWindow(SW_SHOWMAXIMIZED);
	m_TPShowFindList.m_staticPMessage.SetWindowTextW(m_strPMessage);
	m_TPDlgJDMsg.ShowWindow(SW_HIDE);
	CMainFrame * pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (pMain->m_bConnected == TRUE)
	{
		if (pMain->m_strMAddress != "")
		{
			m_hThreadsend=CreateThread (NULL,0,ThreadSendTPFindlist,this,0,NULL);
			CloseHandle(m_hThreadsend);

           // m_hThreadsend=CreateThread (NULL,0,ThreadSendTP,this,0,NULL);
			//CloseHandle(m_hThreadsend);
		}
		else
		{
			AfxMessageBox(_T("��������CCO��"));
		}
	}
	else
	{
		AfxMessageBox(_T("���ȴ򿪴��ڣ�"));
	}
}

void CNetViewTPShow::OnIdrMenuRbtpShow()
{
	m_nMsgType = 0;
	m_TPDlgShowExplian.GetNumb(m_nOnlineNumb);
	m_TPDlgShowExplian.ShowWindow(SW_SHOW);
}

void CNetViewTPShow::OnIdrMenuRbtpHeid()
{
	m_nMsgType = 1;
	m_TPDlgShowExplian.ShowWindow(SW_HIDE);
}

void CNetViewTPShow::OnRbtpTest()
{
	GetPointCoordinate(_T("CCOTPShow"));
}

void CNetViewTPShow::OnRbtpjdmsgPtp()
{
	CMainFrame * pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CFrameSetNet * pChildFrm =  ( CFrameSetNet *) pMain->GetActiveFrame();
	CRightTabView *  pTabView =( CRightTabView *) pChildFrm->m_wndSplitter.GetPane(0 , 1);
	pTabView->SetActiveView(2);
	CNetViewPToP * pViewPTP = (CNetViewPToP *)pTabView->GetActiveView();
	pViewPTP->m_editMAC1.SetWindowText(m_strChooseMAC);
}

BOOL CNetViewTPShow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			break;
		case VK_RETURN:
			break;
		}
		break;
	case WM_CHAR:
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			break;
		case VK_RETURN:
			break;
		}
		break;
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CNetViewTPShow::OnRbtpAutoupdata()
{
	if (m_bAutoUpdata)
	{
		m_bAutoUpdata = false;
		m_nUpdataTime = 0;
	}
	else
	{
		CDlgCKQUpdataTime dlgUpdataTime;
		if (dlgUpdataTime.DoModal() == IDOK)
		{
			m_nUpdataTime = _ttoi(dlgUpdataTime.m_strTime);
			m_bAutoUpdata = true;
		}
	}
	
}

void CNetViewTPShow::OnRbtpSave()
{
	if (m_bSave)
	{
		m_bSave = false;
	}
	else
	{
		m_bSave = true;
	}
}


void CNetViewTPShow::Select_JZQ_Port()
{
	// TODO: Add your command handler code here
	if (!m_topo_jzq_port)
		m_topo_jzq_port = 1;


	
}


void CNetViewTPShow::OnCkq()
{
	// TODO: Add your command handler code here
	if (m_topo_jzq_port)
		m_topo_jzq_port = 0;

}


DWORD WINAPI ThreadSendTPFindlist (PVOID pParam) 
{
	//����Ԫ���
	//�˴�Ϊ�Ҽ��������б�ִ�к�������δ�޸�
	CMainFrame * pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CNetViewTPShow * pView = (CNetViewTPShow *)pParam;
	INT16U u16temp = 0 , u16BeginNumb = 0;
	INT8U SendDatabuf[10];
	INT8U ack = 0 , u8SendAFN = 0 ,AddrBuf[7] , u8ReadCount = 0;
	INT16U SendDatalen = 0 ,temp16 = 0;
    INT16U times=0;
	CString strAllTPdata =_T("");
    if (pMain->NW_OR_GW==1){ //������׼
		sPartQCSG376_2CreatFrame ptSendQCSG376_2;
		sPartQCSG376_2DeCodeFrame ptRecvQCSG376_2;
		bool bOK = true;
		pView->m_tools._str16tobuf16( pMain->m_strMAddress , AddrBuf , temp16 , true);//�ַ���תbuf
		SendDatabuf[0] = 1;
		SendDatabuf[1] = 0;
		SendDatabuf[2] = 50;
		SendDatalen = 3;
		while(bOK)
		{
			//ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf , 0xF0 , F11 , SendDatabuf , SendDatalen , pView->m_ptSendQCSG376_2,ptRecvQCSG376_2 , pView->m_ptRecvQCSG376_2);


            
            if (pView->m_topo_jzq_port)
                ack = gSimJzq.HostSendRcv376_2Buf( 0x10 , F21 , SendDatabuf , SendDatalen , pView->m_ptSendQGDW376_2 , pView->m_ptRecvQGDW376_2);
            else
				ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf, 0x10, F21, SendDatabuf, SendDatalen, pView->m_ptSendQCSG376_2, ptRecvQCSG376_2, pView->m_ptRecvQCSG376_2);
            
			switch(ack)
			{
			case 0x00://�ɹ�
				strAllTPdata += pView->m_tools._buf16tostr16(&pView->m_ptRecvQCSG376_2.s_RcvDataBuf[5] , pView->m_ptRecvQCSG376_2.s_RcvDataLen -5);//bufת�ַ���
				temp16 = pView->m_ptRecvQCSG376_2.s_RcvDataBuf[1] << 8 | pView->m_ptRecvQCSG376_2.s_RcvDataBuf[0];
				if (temp16 <= SendDatabuf[2])
				{
					bOK = false;
				}
				else
				{
					u16BeginNumb += 50;
					SendDatabuf[0] = u16BeginNumb;
					SendDatabuf[1] = u16BeginNumb >> 8;
				}
				break;
			}
		}
	}
	else{
		sPartQGDW376_2CreatFrame ptSendQGDW376_2;
		sPartQGDW376_2DeCodeFrame ptRecvQGDW376_2;
		bool bOK = true;
		pView->m_tools._str16tobuf16( pMain->m_strMAddress , AddrBuf , temp16 , true);//�ַ���תbuf
		SendDatabuf[0] = 1;
		SendDatabuf[1] = 0;
		SendDatabuf[2] = 50;
		SendDatalen = 3;
		AfxMessageBox(_T(" thread "));
		while(bOK)
		{
			//ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf , 0xF0 , F11 , SendDatabuf , SendDatalen , pView->m_ptSendQGDW376_2,ptRecvQGDW376_2 , pView->m_ptRecvQGDW376_2);

            if (pView->m_topo_jzq_port)
                ack = gSimJzq.HostSendRcv376_2Buf( 0x10 , F21 , SendDatabuf , SendDatalen , pView->m_ptSendQGDW376_2 , pView->m_ptRecvQGDW376_2);
            else
                ack = gSimJzq.HostSendRcv376_2F0F8(AddrBuf, 0x10, F21, SendDatabuf, SendDatalen, pView->m_ptSendQGDW376_2, ptRecvQGDW376_2, pView->m_ptRecvQGDW376_2);

            
            
            if(times++>22)
                break;
            
			switch(ack)
			{
			case 0x00://�ɹ�
				strAllTPdata += pView->m_tools._buf16tostr16(&pView->m_ptRecvQGDW376_2.s_RcvDataBuf[5] , pView->m_ptRecvQGDW376_2.s_RcvDataLen -5);//bufת�ַ���
				
				temp16 = pView->m_ptRecvQGDW376_2.s_RcvDataBuf[1] << 8 | pView->m_ptRecvQGDW376_2.s_RcvDataBuf[0];

                if(temp16>1016)
                    break;
                
				temp16=pView->m_ptRecvQGDW376_2.s_RcvDataBuf[4];
				if (temp16 < 50)
				{
					bOK = false;
				}
				else
				{
					u16BeginNumb += 50;
					SendDatabuf[0] = u16BeginNumb;
					SendDatabuf[1] = u16BeginNumb >> 8;
				}
				break;
             default :
                TRACE("\n ACK ERR\n");
                
			}
		}
	}
	pView->m_TPShowFindList.SetTPdataToAccess(strAllTPdata);
	return 0;
}
