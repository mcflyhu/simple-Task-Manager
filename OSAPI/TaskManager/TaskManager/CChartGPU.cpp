// CChartGPU.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CChartGPU.h"
#include "afxdialogex.h"




// CChartGPU 对话框

IMPLEMENT_DYNAMIC(CChartGPU, CDialogEx)

CChartGPU::CChartGPU(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHART_GPU, pParent)
{

}

CChartGPU::~CChartGPU()
{
}

void CChartGPU::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GPU, m_StaticGPU);
	DDX_Control(pDX, IDC_STATIC_GPU_NAME, m_StaticGPUName);
	DDX_Control(pDX, IDC_STATIC_GPU_USAGE, m_StaticGPUChart);
	DDX_Control(pDX, IDC_STATIC_GPUMEM_USAGE, m_StaticGPUMemChart);
	DDX_Control(pDX, IDC_STATIC_GPU_PER, m_StaticGPUPer);
	DDX_Control(pDX, IDC_STATIC_GPUMEM_PER, m_StaticGPUMemPer);
}


BEGIN_MESSAGE_MAP(CChartGPU, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChartGPU 消息处理程序


BOOL CChartGPU::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置显示字体
	cFont_GPU.CreatePointFont(150, _T("黑体"), NULL);
	GetDlgItem(IDC_STATIC_GPU)->SetFont(&cFont_GPU);

	cFont_NameGPU.CreatePointFont(100, _T("Times New Roman"), NULL);
	GetDlgItem(IDC_STATIC_GPU_NAME)->SetFont(&cFont_NameGPU);

	CString str;
	GetGPUName(str);
	SetDlgItemText(IDC_STATIC_GPU_NAME, str);
	
	cFont_GPUsage.CreatePointFont(100, _T("Avenir"), NULL);
	GetDlgItem(IDC_STATIC_GPU_PER)->SetFont(&cFont_GPUsage);
	GetDlgItem(IDC_STATIC_GPUMEM_PER)->SetFont(&cFont_GPUsage);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChartGPU::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	unsigned int gusage = GetGPUsage();
	if (gUsage.size() == 10)
		gUsage.pop();
	gUsage.push(gusage);

	unsigned int gmusage = GetGPUMemUsage();
	if (gMemUsage.size() == 10)
		gMemUsage.pop();
	gMemUsage.push(gmusage);

	m_StaticGPUChart.Invalidate();
	m_StaticGPUChart.UpdateWindow();
	DrawGPUChart();
	m_StaticGPUMemChart.Invalidate();
	m_StaticGPUMemChart.UpdateWindow();
	DrawGPUMemChart();
	CDialogEx::OnTimer(nIDEvent);
}

void CChartGPU::BeginTimer()
{
	SetTimer(2, 1000, NULL);
}
void CChartGPU::StopTimer()
{
	KillTimer(2);
}

unsigned int CChartGPU::GetGPUsage()
{
	nvmlReturn_t result;
	// First initialize NVML library
	result = nvmlInit();
	nvmlDevice_t device;
	result = nvmlDeviceGetHandleByIndex(0, &device);
	if (NVML_SUCCESS != result)
	{
		return -1;
	}
	//使用率
	nvmlUtilization_t utilization;
	result = nvmlDeviceGetUtilizationRates(device, &utilization);
	if (NVML_SUCCESS == result)
	{
		return utilization.gpu;
	}

}
unsigned int CChartGPU::GetGPUMemUsage()
{
	nvmlReturn_t result;
	// First initialize NVML library
	result = nvmlInit();

	nvmlDevice_t device;

	result = nvmlDeviceGetHandleByIndex(0, &device);
	if (NVML_SUCCESS != result)
	{
		return -1;
	}
	//使用率
	nvmlUtilization_t utilization;
	result = nvmlDeviceGetUtilizationRates(device, &utilization);
	if (NVML_SUCCESS == result)
	{
		return utilization.memory;
	}

}

void CChartGPU::GetGPUName(CString& GPU_Name)
{
	nvmlReturn_t result;
	// First initialize NVML library
	result = nvmlInit();

	nvmlDevice_t device;
	char name[NVML_DEVICE_NAME_BUFFER_SIZE];
	result = nvmlDeviceGetHandleByIndex(0, &device);
	if (NVML_SUCCESS != result)
	{
		return;
	}
	result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
	if (NVML_SUCCESS == result)
	{
		GPU_Name = name;
		return;
	}
	
}

void CChartGPU::DrawGPUChart()
{
	CDC* pDC = m_StaticGPUChart.GetDC();
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&Pen);
	CRect rect;
	m_StaticGPUChart.GetWindowRect(&rect);
	//m_StaticCPUChart.ScreenToClient(&rect);

	CString str;
	str.Format(_T("%d%%"), gUsage.back());
	SetDlgItemTextW(IDC_STATIC_GPU_PER, str);

	int queue_size = gUsage.size();

	CPoint oldPoint;
	CPoint newPoint;
	newPoint = oldPoint = CPoint(int(rect.Width() * (10 - queue_size) / 10.0), rect.Height() * (1 - gUsage.front() / 100.0) );
	//pDC->MoveTo(newPoint);
	for (int i = queue_size - 1; i >= 0; i--)
	{
		pDC->MoveTo(oldPoint);
		newPoint.SetPoint(int(rect.Width() * (10 - i) / 10.0), rect.Height() * (1 - gUsage.front() / 100.0) );
		pDC->LineTo(newPoint);
		oldPoint = newPoint;
		gUsage.push(gUsage.front());
		gUsage.pop();
	}
}

void CChartGPU::DrawGPUMemChart()
{
	CDC* pDC = m_StaticGPUMemChart.GetDC();
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&Pen);
	CRect rect;
	m_StaticGPUMemChart.GetWindowRect(&rect);
	//m_StaticCPUChart.ScreenToClient(&rect);

	CString str;
	str.Format(_T("%d%%"), gMemUsage.back());
	SetDlgItemTextW(IDC_STATIC_GPUMEM_PER, str);
	
	int queue_size = gMemUsage.size();

	CPoint oldPoint;
	CPoint newPoint;
	newPoint = oldPoint = CPoint(int(rect.Width() * (10 - queue_size) / 10.0), rect.Height() * (1 - gMemUsage.front() / 100.0) - 1);
	//pDC->MoveTo(newPoint);
	for (int i = queue_size - 1; i >= 0; i--)
	{
		pDC->MoveTo(oldPoint);
		newPoint.SetPoint(int(rect.Width() * (10 - i) / 10.0), rect.Height() * (1 - gMemUsage.front() / 100.0) - 1);
		pDC->LineTo(newPoint);
		oldPoint = newPoint;
		gMemUsage.push(gMemUsage.front());
		gMemUsage.pop();
	}
}