#pragma once
#include "nvml.h"
#include <queue>



// CChartGPU 对话框

class CChartGPU : public CDialogEx
{
	DECLARE_DYNAMIC(CChartGPU)

public:
	CChartGPU(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChartGPU();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART_GPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:

	CStatic m_StaticGPU;
	
	CFont cFont_GPU;

	
	CStatic m_StaticGPUName;
	CFont cFont_NameGPU;

	CStatic m_StaticGPUPer;
	CStatic m_StaticGPUMemPer;
	CFont cFont_GPUsage;
	
	CStatic m_StaticGPUChart;
	CStatic m_StaticGPUMemChart;
	std::queue<unsigned int> gUsage; //gpu使用率队列
	std::queue<unsigned int> gMemUsage; //显存使用率队列
public:
	//void SetIdentifier(int identifier);
	void BeginTimer();
	void StopTimer();
	void DrawGPUChart();
	void DrawGPUMemChart();
	unsigned int GetGPUsage();
	unsigned int GetGPUMemUsage();
	void GetGPUName(CString& GPU_Name);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	
};
