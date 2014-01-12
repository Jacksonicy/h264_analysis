
/* 
 *H.264 ��Ƶר�÷���
 *H.264 Stream Analysis
 *
 *������ Lei Xiaohua
 *leixiaohua1020@126.com
 *�й���ý��ѧ/���ֵ��Ӽ���
 *Communication University of China / Digital TV Technology
 *http://blog.csdn.net/leixiaohua1020
 */
#include "stdafx.h"
#include "SpecialVH264.h"
#include "SpecialVH264Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpecialVH264Dlg �Ի���




CSpecialVH264Dlg::CSpecialVH264Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpecialVH264Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpecialVH264Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_V_H264_INPUTURL, m_vh264inputurl);
	DDX_Control(pDX, IDC_V_H264_NALLIST, m_vh264nallist);
	DDX_Control(pDX, IDC_V_H264_NALLIST_MAXNUM, m_vh264nallistmaxnum);
	DDX_Control(pDX, IDC_V_H264_NALINFO, m_vh264nalinfo);
	//  DDX_Control(pDX, IDC_V_H264_OUTPUTINFO, m_vh264outputinfo);
	//  DDX_Control(pDX, IDC_V_H264_METHOD, m_vh264method);
}

BEGIN_MESSAGE_MAP(CSpecialVH264Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_V_H264_INPUTURL_OPEN, &CSpecialVH264Dlg::OnBnClickedVH264InputurlOpen)
	ON_NOTIFY ( NM_CUSTOMDRAW,IDC_V_H264_NALLIST, OnCustomdrawMyList )
	ON_BN_CLICKED(IDC_V_H264_ABOUT, &CSpecialVH264Dlg::OnBnClickedVH264About)
ON_NOTIFY(LVN_ITEMACTIVATE, IDC_V_H264_NALLIST, &CSpecialVH264Dlg::OnItemactivateVH264Nallist)
ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CSpecialVH264Dlg ��Ϣ�������

BOOL CSpecialVH264Dlg::OnInitDialog()
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
	//����ѡ���б���ߣ���ͷ����������
	DWORD dwExStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE;
	//�����񣻵���ѡ�񣻸�����ʾѡ����
	//��Ƶ
	m_vh264nallist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_vh264nallist.SetExtendedStyle(dwExStyle);

	m_vh264nallist.InsertColumn(0,"Number",LVCFMT_CENTER,50,0);
	m_vh264nallist.InsertColumn(1,"nal_reference_idc",LVCFMT_CENTER,100,0);
	m_vh264nallist.InsertColumn(2,"NAL Type(nal_unit_type)",LVCFMT_CENTER,100,0);
	m_vh264nallist.InsertColumn(3,"NAL Size(len)",LVCFMT_CENTER,100,0);
	//---------------------
	m_vh264nallistmaxnum.SetCheck(1);
	nl_index=0;
	//------------
	m_vh264inputurl.EnableFileBrowseButton(
		NULL,
		"H.264 Files (*.264,*.h264)|*.264;*.h264|All Files (*.*)|*.*||"
		);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSpecialVH264Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSpecialVH264Dlg::OnPaint()
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
HCURSOR CSpecialVH264Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSpecialVH264Dlg::OnBnClickedVH264InputurlOpen()
{
	SystemClear();
	CString strFilePath;
	m_vh264inputurl.GetWindowText(strFilePath);
	if(strFilePath.IsEmpty()==TRUE){
		text.LoadString(IDS_INPUTURL_ERROR);
		AfxMessageBox(text);
		return;
	}
	strcpy(fileurl,strFilePath);
	h264_nal_parse(this,fileurl);
}

//���һ����¼
//ÿ���ֶεĺ��壺���ͣ����ݴ�С��ʱ�����streamid��data�ĵ�һ���ֽ�
//data_lenth�ǰ�����ʼ���NAL����
//len�ǲ�������ʼ���NAL����
int CSpecialVH264Dlg::AppendNLInfo(int nal_reference_idc,int nal_unit_type,int len,int data_lenth,int data_offset){
	//���ѡ���ˡ�������5000�������ж��Ƿ񳬹�5000��
	if(m_vh264nallistmaxnum.GetCheck()==1&&nl_index>5000){
		return 0;
	}
	CString temp_index,temp_nal_reference_idc,temp_nal_unit_type,temp_len;
	int nIndex=0;
	switch(nal_unit_type){
	case 1:temp_nal_unit_type.Format("SLICE");break;
	case 2:temp_nal_unit_type.Format("DPA");break;
	case 3:temp_nal_unit_type.Format("DPB");break;
	case 4:temp_nal_unit_type.Format("DPC");break;
	case 5:temp_nal_unit_type.Format("IDR_SLICE");break;
	case 6:temp_nal_unit_type.Format("SEI");break;
	case 7:temp_nal_unit_type.Format("SPS");break;
	case 8:temp_nal_unit_type.Format("PPS");break;
	case 9:temp_nal_unit_type.Format("AUD");break;
	case 10:temp_nal_unit_type.Format("END_SEQUENCE");break;
	case 11:temp_nal_unit_type.Format("END_STREAM");break;
	case 12:temp_nal_unit_type.Format("FILLER_DATA");break;
	case 13:temp_nal_unit_type.Format("SPS_EXT");break;
	case 19:temp_nal_unit_type.Format("AUXILIARY_SLICE");break;
	default:temp_nal_unit_type.Format("����");break;
	}
	temp_index.Format("%d",nl_index);
	temp_nal_reference_idc.Format("%d",nal_reference_idc);
	temp_len.Format("%d",len);
	//��ȡ��ǰ��¼����
	nIndex=m_vh264nallist.GetItemCount();
	//���С����ݽṹ
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=nIndex;
	lvitem.iSubItem=0;
	//ע��vframe_index������ֱ�Ӹ�ֵ��
	//���ʹ��f_indexִ��Format!�ٸ�ֵ��
	lvitem.pszText=(char *)(LPCTSTR)temp_index;
	//------------------------
	//���vector��¼��nal��λ����Ϣ
	//ʹ�������ǿ��Ի�ȡ��NAL����ϸ��Ϣ
	//����Ҫ�洢������ʼ��ĳ���
	//��ʼ��ԭ������NAL��һ����
	NALInfo nalinfo;
	nalinfo.data_lenth=data_lenth;
	nalinfo.data_offset=data_offset;
	nl_infovector.push_back(nalinfo);
	//------------------------
	m_vh264nallist.InsertItem(&lvitem);
	m_vh264nallist.SetItemText(nIndex,1,temp_nal_reference_idc);
	m_vh264nallist.SetItemText(nIndex,2,temp_nal_unit_type);
	m_vh264nallist.SetItemText(nIndex,3,temp_len);
	nl_index++;
	return TRUE;
}

void CSpecialVH264Dlg::SystemClear(){
	nl_infovector.clear();
	m_vh264nallist.DeleteAllItems();
	nl_index=0;
}
//ListCtrl����ɫ
void CSpecialVH264Dlg::OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{

		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		CString strTemp = m_vh264nallist.GetItemText(nItem,2);
		if(strcmp(strTemp,"SLICE")==0){
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(0,255,255);		//��ɫ
		}
		else if(strcmp(strTemp,"SPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text 
			clrNewBkColor = RGB(255,255,0);		//��ɫ
		}
		else if(strcmp(strTemp,"PPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,153,0);		//����ɫ
		}else if(strcmp(strTemp,"SEI")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,66,255);			//�ۺ�ɫ
		}else if(strcmp(strTemp,"IDR_SLICE")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,0,0);			//��ɫ
		}else{
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,255,255);			//��ɫ
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;


		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;


	}
}

void CSpecialVH264Dlg::OnBnClickedVH264About()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CSpecialVH264Dlg::OnItemactivateVH264Nallist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//----------------------
	POSITION ps;
	int nIndex;

	ps=m_vh264nallist.GetFirstSelectedItemPosition();
	nIndex=m_vh264nallist.GetNextSelectedItem(ps);
	//----------------------
	int data_offset,data_lenth;
	data_offset=nl_infovector[nIndex].data_offset;
	data_lenth=nl_infovector[nIndex].data_lenth;
	probe_nal_unit(fileurl,data_offset,data_lenth,this);
	//----------------------
	//���ѡ���е����
	//CString str;
	//str=m_vh264nallist.GetItemText(nIndex,0);
	//AfxMessageBox(str);
	//----------------------
	*pResult = 0;
}


void CSpecialVH264Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnDropFiles(hDropInfo);
	char* pFilePathName =(char *)malloc(MAX_URL_LENGTH);
	::DragQueryFile(hDropInfo, 0, pFilePathName,MAX_URL_LENGTH);  // ��ȡ�Ϸ��ļ��������ļ�������ؼ���
	m_vh264inputurl.SetWindowTextA(pFilePathName);
	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	free(pFilePathName);
}
