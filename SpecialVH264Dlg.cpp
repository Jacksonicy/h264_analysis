
/* 
 *H.264 视频专用分析
 *H.264 Stream Analysis
 *
 *雷霄骅 Lei Xiaohua
 *leixiaohua1020@126.com
 *中国传媒大学/数字电视技术
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSpecialVH264Dlg 对话框




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


// CSpecialVH264Dlg 消息处理程序

BOOL CSpecialVH264Dlg::OnInitDialog()
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
	//整行选择；有表格线；表头；单击激活
	DWORD dwExStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE;
	//报表风格；单行选择；高亮显示选择行
	//视频
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
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSpecialVH264Dlg::OnPaint()
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

//添加一条记录
//每个字段的含义：类型，数据大小，时间戳，streamid，data的第一个字节
//data_lenth是包含起始码的NAL长度
//len是不包含起始码的NAL长度
int CSpecialVH264Dlg::AppendNLInfo(int nal_reference_idc,int nal_unit_type,int len,int data_lenth,int data_offset){
	//如果选择了“最多输出5000条”，判断是否超过5000条
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
	default:temp_nal_unit_type.Format("其他");break;
	}
	temp_index.Format("%d",nl_index);
	temp_nal_reference_idc.Format("%d",nal_reference_idc);
	temp_len.Format("%d",len);
	//获取当前记录条数
	nIndex=m_vh264nallist.GetItemCount();
	//“行”数据结构
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=nIndex;
	lvitem.iSubItem=0;
	//注：vframe_index不可以直接赋值！
	//务必使用f_index执行Format!再赋值！
	lvitem.pszText=(char *)(LPCTSTR)temp_index;
	//------------------------
	//这个vector记录了nal的位置信息
	//使用它我们可以获取到NAL的详细信息
	//我们要存储包含起始码的长度
	//起始码原本不是NAL的一部分
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
//ListCtrl加颜色
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
			clrNewBkColor = RGB(0,255,255);		//青色
		}
		else if(strcmp(strTemp,"SPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text 
			clrNewBkColor = RGB(255,255,0);		//黄色
		}
		else if(strcmp(strTemp,"PPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,153,0);		//咖啡色
		}else if(strcmp(strTemp,"SEI")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,66,255);			//粉红色
		}else if(strcmp(strTemp,"IDR_SLICE")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,0,0);			//红色
		}else{
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,255,255);			//白色
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
	//获得选中行的序号
	//CString str;
	//str=m_vh264nallist.GetItemText(nIndex,0);
	//AfxMessageBox(str);
	//----------------------
	*pResult = 0;
}


void CSpecialVH264Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnDropFiles(hDropInfo);
	char* pFilePathName =(char *)malloc(MAX_URL_LENGTH);
	::DragQueryFile(hDropInfo, 0, pFilePathName,MAX_URL_LENGTH);  // 获取拖放文件的完整文件名，最关键！
	m_vh264inputurl.SetWindowTextA(pFilePathName);
	::DragFinish(hDropInfo);   // 注意这个不能少，它用于释放Windows 为处理文件拖放而分配的内存
	free(pFilePathName);
}
