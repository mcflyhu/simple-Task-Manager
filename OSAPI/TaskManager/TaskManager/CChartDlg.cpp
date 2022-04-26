// CChartDlg.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CChartDlg.h"
#include "afxdialogex.h"


// CChartDlg 对话框

IMPLEMENT_DYNAMIC(CChartDlg, CDialogEx)

CChartDlg::CChartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHART, pParent)
{

}

CChartDlg::~CChartDlg()
{
}

void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_TAB_CHART_MENU, mChartTab);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChartDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHART_MENU, &CChartDlg::OnTcnSelchangeTabChartMenu)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CHART_MENU, &CChartDlg::OnTcnSelchangingTabChartMenu)
END_MESSAGE_MAP()


// CChartDlg 消息处理程序


BOOL CChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mChartTab.InsertItem(0, _T("CPU"));
	mChartTab.InsertItem(1, _T("内存"));
	mChartTab.InsertItem(2, _T("显卡"));

	mChartCPU.Create(IDD_DIALOG_CHART_CPU, &mChartTab);
	mChartMemory.Create(IDD_DIALOG_CHART_MEMORY, &mChartTab);
	mChartGPU.Create(IDD_DIALOG_CHART_GPU, &mChartTab);

	//调整子窗口大小
	CRect rc_Wnd, rc_Tab;
	mChartTab.GetClientRect(&rc_Wnd);
	mChartTab.GetItemRect(0, &rc_Tab);
	rc_Wnd.left += rc_Tab.Width();

	mChartCPU.MoveWindow(rc_Wnd);
	mChartMemory.MoveWindow(rc_Wnd);
	mChartGPU.MoveWindow(rc_Wnd);

	mChartCPU.ShowWindow(SW_SHOW);
	mChartCPU.BeginTimer();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChartDlg::OnTcnSelchangeTabChartMenu(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (mChartTab.GetCurSel())
	{
	case 0:
		mChartCPU.ShowWindow(SW_SHOW);
		mChartCPU.BeginTimer();
		break;
	case 1:
		mChartMemory.ShowWindow(SW_SHOW);
		mChartMemory.BeginTimer();
		break;
	case 2:
		mChartGPU.ShowWindow(SW_SHOW);
		mChartGPU.BeginTimer();
		break;
	default:
		break;
	}
	*pResult = 0;
}


void CChartDlg::OnTcnSelchangingTabChartMenu(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (mChartTab.GetCurSel())
	{
	case 0:
		mChartCPU.ShowWindow(SW_HIDE);
		mChartCPU.StopTimer();
		break;
	case 1:
		mChartMemory.ShowWindow(SW_HIDE);
		mChartMemory.StopTimer();
		break;
	case 2:
		mChartGPU.ShowWindow(SW_HIDE);
		mChartGPU.StopTimer();
	default:
		break;
	}
	*pResult = 0;
}
