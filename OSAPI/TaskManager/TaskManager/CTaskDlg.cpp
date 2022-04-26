// CTaskDlg.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CTaskDlg.h"
#include "afxdialogex.h"
#include "Windows.h"
#include <Psapi.h>
#include <Pdh.h>


#pragma comment(lib,"pdh.lib")
// CTaskDlg 对话框


IMPLEMENT_DYNAMIC(CTaskDlg, CDialogEx)

CTaskDlg::CTaskDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TASK, pParent)
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_LIST_TASK,m_Tlist);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEWINDOW, &CTaskDlg::OnBnClickedButtonClosewindow)
END_MESSAGE_MAP()


// CTaskDlg 消息处理程序

void CTaskDlg::BrowseWindow()
{
	CWnd* pDesktopWnd = GetDesktopWindow();
	CWnd* pWnd = pDesktopWnd->GetWindow(GW_CHILD);
	CString str;
	int i = 0;
	while (pWnd)
	{
		if (pWnd->IsWindowVisible())
		{
			HWND hwnd = pWnd->GetSafeHwnd(); //获取窗口句柄
			LPWSTR szClass = NULL;
			GetClassName(hwnd, szClass, 256);
			CString szclass(szClass);
			
			if (szclass.Compare(L"Shell_TrayWnd") != 0 && szclass.Compare(L"Progman") != 0) // 滤掉桌面
			{
				pWnd->GetWindowTextW(str);
				if (!str.IsEmpty())
				{
					m_Tlist.InsertItem(i, str);
					
					if (SendMessageTimeout(hwnd, WM_SYSCOMMAND, NULL, NULL, SMTO_ABORTIFHUNG, 1000, NULL))
					{
						str.Format(_T("正在运行"));
					}
					else
					{
						str.Format(_T("已挂起"));
					}
					m_Tlist.SetItemText(i, 1, str);

					str.Format(_T("%dMB"),GetWindowMemory(hwnd));
					m_Tlist.SetItemText(i, 2, str);

					str.Format(_T("%ldKB/s"), GetWindowIOSpeed(hwnd));
					m_Tlist.SetItemText(i, 3, str);

					//str.Format(_T("%lf%%"), GetWindowCPUsage(pWnd));
					//m_Tlist.SetItemText(i, 4, str);
					i++;
				}
			}

			
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		
	}
}

SIZE_T CTaskDlg::GetWindowMemory(HWND hwnd)
{
	DWORD Pid;
	GetWindowThreadProcessId(hwnd, &Pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, Pid);

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
	CloseHandle(hProcess);
	return pmc.WorkingSetSize / (1024 * 1024);
}

ULONGLONG CTaskDlg::GetWindowIOSpeed(HWND hwnd)
{
	DWORD Pid;
	GetWindowThreadProcessId(hwnd, &Pid);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, Pid);

	_IO_COUNTERS ioCounter;
	ULONGLONG ioSpeedOld = 0;
	ULONGLONG ioSpeedNew = 0;
	if (GetProcessIoCounters(hProcess, &ioCounter))
	{
		ioSpeedOld = ioCounter.ReadTransferCount + ioCounter.WriteTransferCount;
	}

	Sleep(500);

	if (GetProcessIoCounters(hProcess, &ioCounter))
	{
		ioSpeedNew = ioCounter.ReadTransferCount + ioCounter.WriteTransferCount;
	}

	return (ioSpeedNew - ioSpeedOld) / (1024 * 0.5);
}

double CTaskDlg::GetWindowCPUsage(CWnd* pWnd)
{
	//开启查询
	HQUERY phQuery;
	PDH_STATUS phStatus = PdhOpenQuery(NULL, NULL, &phQuery);
	if (phStatus != ERROR_SUCCESS)
		return 0;
	
	//添加计数器
	HCOUNTER phCounterCPU;
	phCounterCPU = (HCOUNTER*)GlobalAlloc(GPTR, sizeof(HCOUNTER));
	phStatus = PdhAddCounter(phQuery, _T("\\Processor Information(_Total)\\% Processor Time"), NULL, &phCounterCPU);
	if (phStatus != ERROR_SUCCESS)
		return 0;

	phStatus=PdhCollectQueryData(phQuery);
	if (phStatus != ERROR_SUCCESS)
		return 0;
	Sleep(50);
	phStatus=PdhCollectQueryData(phQuery);
	if (phStatus != ERROR_SUCCESS)
		return 0;
	PDH_FMT_COUNTERVALUE pdhValue;
	DWORD dwValue;

	phStatus = PdhGetFormattedCounterValue(phCounterCPU, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwValue, &pdhValue);
	if (phStatus != ERROR_SUCCESS)
		return 0;
	//关闭计数器
	PdhCloseQuery(phQuery);

	return pdhValue.doubleValue;
}

void CTaskDlg::OnBnClickedButtonClosewindow()
{
	// TODO: 在此添加控件通知处理程序代码
	int selCur = m_Tlist.GetSelectionMark();
	if (selCur == -1)
		return;
	CString hWindowName = m_Tlist.GetItemText(selCur, 0);

	CWnd* pWnd = FindWindow(NULL, hWindowName);
	if (!pWnd)
	{
		AfxMessageBox(_T("无法找打该窗口"));
		return;
	}
	DWORD dwPID;
	HWND hWnd = pWnd->GetSafeHwnd();
	GetWindowThreadProcessId(hWnd, &dwPID);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);

	if (!hProcess)
	{
		AfxMessageBox(_T("该窗口不存在"));
		return;
	}

	TerminateProcess(hProcess, 0);

	m_Tlist.DeleteAllItems();
	BrowseWindow();
}


BOOL CTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 添加表头
	m_Tlist.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_Tlist.InsertColumn(1, _T("状态"), LVCFMT_LEFT, 200);
	m_Tlist.InsertColumn(2, _T("内存"), LVCFMT_RIGHT, 200);
	m_Tlist.InsertColumn(3, _T("磁盘"), LVCFMT_RIGHT, 200);
	//m_Tlist.InsertColumn(4, _T("CPU"), LVCFMT_RIGHT, 200);

	m_Tlist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES);
	if (m_Tlist.DeleteAllItems() == false)
		return FALSE;

	BrowseWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
