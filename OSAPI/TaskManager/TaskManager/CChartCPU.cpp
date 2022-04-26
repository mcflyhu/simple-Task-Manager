// CChartCPU.cpp: 实现文件
//

#include "pch.h"
#include "TaskManager.h"
#include "CChartCPU.h"
#include "afxdialogex.h"
#include <Pdh.h>



// CChartCPU 对话框

IMPLEMENT_DYNAMIC(CChartCPU, CDialogEx)

CChartCPU::CChartCPU(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHART_CPU, pParent)
{

}

CChartCPU::~CChartCPU()
{
}

void CChartCPU::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_CPU, m_Static);
    DDX_Control(pDX, IDC_STATIC_CPU_NAME, m_StaticNameCPU);
    DDX_Control(pDX, IDC_STATIC_CPU_CHART, m_StaticCPUChart);
    DDX_Control(pDX, IDC_STATIC_CPU_Usage, m_StaticCPUsage);
}


BEGIN_MESSAGE_MAP(CChartCPU, CDialogEx)
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CChartCPU 消息处理程序


BOOL CChartCPU::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	//设置显示字体
	cFont_CPU.CreatePointFont(150, _T("黑体"), NULL);
	GetDlgItem(IDC_STATIC_CPU)->SetFont(&cFont_CPU);
    
	cFont_NameCPU.CreatePointFont(100, _T("Times New Roman"), NULL);
	GetDlgItem(IDC_STATIC_CPU_NAME)->SetFont(&cFont_NameCPU);

    cFont_CPUsage.CreatePointFont(130, _T("Times New Roman"), NULL);
    GetDlgItem(IDC_STATIC_CPU_Usage)->SetFont(&cFont_CPUsage);
   
    CString str;
    GetCPUType(str);    //获取CPU型号
    SetDlgItemTextW(IDC_STATIC_CPU_NAME, str);
    
    /*
    CClientDC dc(this);
    CPen Pen;
    Pen.CreatePen(PS_SOLID, 20, RGB(0, 255, 0));
    dc.SelectObject(&Pen);
    CRect rect;
    m_StaticCPUChart.GetWindowRect(&rect);
    m_StaticCPUChart.ScreenToClient(&rect);
    dc.MoveTo(rect.TopLeft());
    dc.LineTo(rect.BottomRight());
    */
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CChartCPU::GetCPUType(CString& sProcessorName)
{
    CString strPath = _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");//注册表子键路径  
    CRegKey regkey;//定义注册表类对象  
    LONG lResult;//LONG型变量－反应结果  
    lResult = regkey.Open(HKEY_LOCAL_MACHINE, LPCTSTR(strPath), KEY_QUERY_VALUE); //打开注册表键  
   //DWORD a = GetLastError();
    if (lResult != ERROR_SUCCESS)
    {
        return;
    }
    
    WCHAR chCPUName[50] = { 0 };
    DWORD dwSize = 50;

    //获取ProcessorNameString字段值  
    if (ERROR_SUCCESS == regkey.QueryStringValue(_T("ProcessorNameString"), chCPUName, &dwSize))
    {
        sProcessorName = chCPUName;
    }

    regkey.Close();//关闭注册表  
}


void CChartCPU::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
    /*
    CDC* pDC = m_StaticCPUChart.GetDC();
    CPen Pen;
    Pen.CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
    pDC->SelectObject(&Pen);
    CRect rect;
    m_StaticCPUChart.GetWindowRect(&rect);
    //m_StaticCPUChart.ScreenToClient(&rect);

    
    int queue_size = qUsage.size();
    CPoint oldPoint;
    CPoint newPoint(0, 0);
    pDC->MoveTo(newPoint);
    for (int i = queue_size; i > 0; i--)
    {
        newPoint.SetPoint(rect.Width() * (10 - i) / 10.0, rect.Height() * (1 - qUsage.front() / 100));
        pDC->LineTo(newPoint);
        pDC->MoveTo(newPoint);
       //oldPoint = newPoint;
        qUsage.push(qUsage.front());
        qUsage.pop();
    }
    */
    
                       // 不为绘图消息调用 CDialogEx::OnPaint()
}

double CChartCPU::GetCPUsage()
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

    phStatus = PdhCollectQueryData(phQuery);
    if (phStatus != ERROR_SUCCESS)
        return 0;
    Sleep(500);
    phStatus = PdhCollectQueryData(phQuery);
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

void CChartCPU::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    double cUsage = GetCPUsage();
    if (qUsage.size() == 10)
        qUsage.pop();
    qUsage.push(cUsage);
   
    m_StaticCPUChart.Invalidate();
    m_StaticCPUChart.UpdateWindow();
    DrawOnMem();
    CDialogEx::OnTimer(nIDEvent);
}

void CChartCPU::BeginTimer()
{
    SetTimer(0, 1000, NULL);
}
void CChartCPU::StopTimer()
{
    KillTimer(0);
}

void CChartCPU::DrawOnMem()
{
    
    CDC* pDC = m_StaticCPUChart.GetDC();
    CPen Pen;
    Pen.CreatePen(PS_SOLID, 1, RGB(51, 153, 255));
    pDC->SelectObject(&Pen);
    CRect rect;
    m_StaticCPUChart.GetWindowRect(&rect);
    //m_StaticCPUChart.ScreenToClient(&rect);


    CString str;
    str.Format(_T("%.0lf%%"), qUsage.back());
    SetDlgItemTextW(IDC_STATIC_CPU_Usage, str);
   
    int queue_size = qUsage.size();
   
    CPoint oldPoint;
    CPoint newPoint;
    newPoint = oldPoint = CPoint(int(rect.Width() * (10 - queue_size) / 10.0), rect.Height() * (1 - qUsage.front() / 100) - 2);
    //pDC->MoveTo(newPoint);
    for (int i = queue_size-1; i >= 0; i--)
    {
        pDC->MoveTo(oldPoint);
        newPoint.SetPoint(int(rect.Width() * (10 - i) / 10.0), rect.Height() * (1 - qUsage.front() / 100) - 2);
        pDC->LineTo(newPoint);
        oldPoint = newPoint;
        qUsage.push(qUsage.front());
        qUsage.pop();
    }
}