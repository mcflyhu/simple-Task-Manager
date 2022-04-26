#pragma once
#include <queue>

// CChartCPU 对话框

class CChartCPU : public CDialogEx
{
	DECLARE_DYNAMIC(CChartCPU)

public:
	CChartCPU(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChartCPU();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART_CPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_Static;
	CFont cFont_CPU;
	
	CStatic m_StaticNameCPU;
	CFont cFont_NameCPU;

	CStatic m_StaticCPUsage;
	CFont cFont_CPUsage;

	CStatic m_StaticCPUChart; //绘制图标区域
	std::queue<double> qUsage; //cpu使用率队列
public:
	virtual BOOL OnInitDialog();
	void GetCPUType(CString& sProcessorName);
	double GetCPUsage();
	void BeginTimer();
	void StopTimer();
	void DrawOnMem();

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
