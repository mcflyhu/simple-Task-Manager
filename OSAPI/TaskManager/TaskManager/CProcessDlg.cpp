// CProcessDlg.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <Psapi.h>


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_LIST_PROCESS,m_List);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE_PROCESS, &CProcessDlg::OnBnClickedButtonTerminateProcess)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序

//遍历进程
void CProcessDlg::BrowseProcess()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	BOOL bMore = Process32First(hProcessSnap, &pe32);
	int i = 0;
	CString str;
	while (bMore)
	{
		//将获取到的进程信息写入列表
		str.Format(_T("%s"), pe32.szExeFile);
		m_List.InsertItem(i, str);
		str.Format(_T("%d"), pe32.th32ProcessID);
		m_List.SetItemText(i, 1, str);
		
		//判断进程状态
		//if(SendMessageTimeout())
		//获取进程的内存资源
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, pe32.th32ProcessID);
	
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
		str.Format(_T("%dK"), pmc.WorkingSetSize / 1024);
		m_List.SetItemText(i, 2, str);

		str.Format(_T("%d"), pe32.cntThreads);
		m_List.SetItemText(i, 3, str);

		str.Format(_T("%ld"), pe32.pcPriClassBase);
		m_List.SetItemText(i, 4, str);

		//获取下一进程信息
		bMore = Process32Next(hProcessSnap, &pe32);
		i++;
		CloseHandle(hProcess);
	}
}

ULONGLONG CProcessDlg::GetProcessCPUsage(HANDLE hProcess)
{
	FILETIME lpIdleTime;
	FILETIME lpKernelTime;
	FILETIME lpUserTime;
	FILETIME lpCreationTime;
	FILETIME lpExitTime;
	FILETIME lpKernelPTime;
	FILETIME lpUserPTime;
	
	ULONGLONG UserTime, KernelTime, KernelPTime, UserPTime;

	if (GetSystemTimes(&lpIdleTime, &lpKernelTime, &lpUserTime))
	{
		//memcpy(&KernelTime, &lpKernelTime, sizeof(UserTime));
		KernelTime = (((ULONGLONG)lpKernelTime.dwHighDateTime) << 32) + lpKernelTime.dwLowDateTime;
		UserTime = (((ULONGLONG)lpUserTime.dwHighDateTime) << 32) + lpUserTime.dwLowDateTime;
		//memcpy(&UserTime, &lpUserTime, sizeof(UserTime));
	}
	else
	{
		KernelTime = UserTime = 1;
	}
	if (GetProcessTimes(hProcess, &lpCreationTime, &lpExitTime, &lpKernelPTime, &lpUserPTime))
	{
		//memcpy(&KernelPTime, &lpKernelPTime, sizeof(KernelPTime));
		KernelPTime = (((ULONGLONG)lpKernelPTime.dwHighDateTime) << 32) + lpKernelPTime.dwLowDateTime;
		UserPTime = (((ULONGLONG)lpUserPTime.dwHighDateTime) << 32) + lpUserPTime.dwLowDateTime;
		//(&UserPTime, &lpUserPTime, sizeof(UserPTime));
	}
	else
	{
		KernelPTime = UserPTime = 0;
	}
	return 100 * (UserPTime + KernelPTime) /(UserTime + KernelTime);
}

void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_List.SetRedraw(false);
	m_List.DeleteAllItems();
	BrowseProcess();
	m_List.SetRedraw(TRUE);
	m_List.Invalidate();
	m_List.UpdateWindow();
		
	CDialogEx::OnTimer(nIDEvent);
}


void CProcessDlg::OnBnClickedButtonTerminateProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	int selCur = m_List.GetSelectionMark();
	if (selCur==-1)
		return;
	CString selPid = m_List.GetItemText(selCur, 1);
	DWORD dwPID = _wtoi(selPid);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);

	if (!hProcess)
	{
		AfxMessageBox(_T("该进程不存在"));
		return;
	}

	TerminateProcess(hProcess, 0);

	//刷新列表
	m_List.DeleteAllItems();
	BrowseProcess();
}


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加表头
	m_List.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_List.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 100);
	m_List.InsertColumn(2, _T("内存"), LVCFMT_RIGHT, 200);
	m_List.InsertColumn(3, _T("执行线程数"), LVCFMT_RIGHT, 200);
	m_List.InsertColumn(4, _T("进程优先级"), LVCFMT_LEFT, 200);

	m_List.SetExtendedStyle(LVS_EX_BORDERSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	if (m_List.DeleteAllItems() == false)
		return FALSE;
	//SetTimer(1, 1000, NULL);
	//获取系统进程
	BrowseProcess();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
