#pragma once
#include <queue>

// CChartMemory 对话框

class CChartMemory : public CDialogEx
{
	DECLARE_DYNAMIC(CChartMemory)

public:
	CChartMemory(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChartMemory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART_MEMORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CFont fSize;
	CStatic m_StaticMemorySize;

	CFont fTitle;
	CStatic m_StaticMemory;

	//
	CFont fGrey;
	CStatic m_StaticUsed;
	CStatic m_StaticAvli;

	//内存使用情况
	CFont fNumber;
	CStatic m_StaticMemoryUsed;
	CStatic m_StaticMemoryAvli;

	CStatic m_StaticMemChart;

	std::queue<DWORD> qUsage; //cpu使用率队列
public:
	DWORDLONG GetMemorySize();
	void DrawOnMem();
	DWORDLONG GetAvailableMemory();
	DWORD GetMemoryUsage();
	void BeginTimer();
	void StopTimer();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
