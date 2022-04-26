// CChartMemory.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CChartMemory.h"
#include "afxdialogex.h"


// CChartMemory 对话框

IMPLEMENT_DYNAMIC(CChartMemory, CDialogEx)

CChartMemory::CChartMemory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHART_MEMORY, pParent)
{

}

CChartMemory::~CChartMemory()
{
}

void CChartMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MEMORY_SIZE, m_StaticMemorySize);
	DDX_Control(pDX, IDC_STATIC_MEMORY, m_StaticMemory);
	DDX_Control(pDX, IDC_STATIC_USED, m_StaticUsed);
	DDX_Control(pDX, IDC_STATIC_AVLI, m_StaticAvli);
	DDX_Control(pDX, IDC_STATIC_MEMORY_USED, m_StaticMemoryUsed);
	DDX_Control(pDX, IDC_STATIC_MEMORY_AVLI, m_StaticMemoryAvli);
	DDX_Control(pDX, IDC_STATIC_CHART_MEMORY, m_StaticMemChart);
}


BEGIN_MESSAGE_MAP(CChartMemory, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChartMemory 消息处理程序


BOOL CChartMemory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	fTitle.CreatePointFont(150, _T("黑体"), NULL);
	GetDlgItem(IDC_STATIC_MEMORY)->SetFont(&fTitle);

	fSize.CreatePointFont(140, _T("Avenir"), NULL);
	GetDlgItem(IDC_STATIC_MEMORY_SIZE)->SetFont(&fSize);

	fGrey.CreatePointFont(80, _T("Times New Roman"), NULL);
	GetDlgItem(IDC_STATIC_AVLI)->SetFont(&fGrey);
	GetDlgItem(IDC_STATIC_USED)->SetFont(&fGrey);

	fNumber.CreatePointFont(150, _T("Avenir"), NULL);
	GetDlgItem(IDC_STATIC_MEMORY_AVLI)->SetFont(&fNumber);
	GetDlgItem(IDC_STATIC_MEMORY_USED)->SetFont(&fNumber);

	CString str;
	str.Format(_T("%I64d.0 GB"), GetMemorySize() / 1024 / 1024 / 1024);   //获取内存容量大小
	SetDlgItemTextW(IDC_STATIC_MEMORY_SIZE, str);
	str.Format(_T("使用中（已压缩）"));
	SetDlgItemTextW(IDC_STATIC_USED, str);
	str.Format(_T("可用"));
	SetDlgItemTextW(IDC_STATIC_AVLI, str);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

DWORDLONG CChartMemory::GetMemorySize()
{
	_MEMORYSTATUSEX memStatusEx;
	memStatusEx.dwLength = sizeof(memStatusEx);

	GlobalMemoryStatusEx(&memStatusEx);

	return memStatusEx.ullTotalPhys;
}

void CChartMemory::DrawOnMem()
{
	CDC* pDC = m_StaticMemChart.GetDC();
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(128,0,128));
	pDC->SelectObject(&Pen);
	CRect rect;
	m_StaticMemChart.GetWindowRect(&rect);
	//m_StaticCPUChart.ScreenToClient(&rect);


	CString str;
	int queue_size = qUsage.size();

	CPoint oldPoint;
	CPoint newPoint;
	newPoint = oldPoint = CPoint(int(rect.Width() * (10 - queue_size) / 10.0), rect.Height() * (1 - qUsage.front() / 100.0));
	//pDC->MoveTo(newPoint);
	for (int i = queue_size - 1; i >= 0; i--)
	{
		pDC->MoveTo(oldPoint);
		str.Format(_T("%.1Lf GB"), (long double)GetAvailableMemory() / 1024.0 / 1024 / 1024);
		SetDlgItemTextW(IDC_STATIC_MEMORY_AVLI, str);
		str.Format(_T("%.1Lf GB"), (long double)(GetMemorySize() - GetAvailableMemory()) / 1024.0 / 1024 / 1024);
		SetDlgItemTextW(IDC_STATIC_MEMORY_USED,str);
		newPoint.SetPoint(int(rect.Width() * (10 - i) / 10.0), rect.Height() * (1 - qUsage.front() / 100.0));
		pDC->LineTo(newPoint);
		oldPoint = newPoint;
		qUsage.push(qUsage.front());
		qUsage.pop();
	}
}
DWORDLONG CChartMemory::GetAvailableMemory()
{
	_MEMORYSTATUSEX memStatusEx;
	memStatusEx.dwLength = sizeof(memStatusEx);

	GlobalMemoryStatusEx(&memStatusEx);

	return memStatusEx.ullAvailPhys;
}
DWORD CChartMemory::GetMemoryUsage()
{
	_MEMORYSTATUSEX memStatusEx;
	memStatusEx.dwLength = sizeof(memStatusEx);

	GlobalMemoryStatusEx(&memStatusEx);

	return memStatusEx.dwMemoryLoad;
}

void CChartMemory::BeginTimer()
{
	SetTimer(1, 1000, NULL);
}
void CChartMemory::StopTimer()
{
	KillTimer(1);
}

void CChartMemory::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DWORD mUsage = GetMemoryUsage();
	if (qUsage.size() == 10)
		qUsage.pop();
	qUsage.push(mUsage);

	m_StaticMemChart.Invalidate();
	m_StaticMemChart.UpdateWindow();
	DrawOnMem();
	CDialogEx::OnTimer(nIDEvent);
}
