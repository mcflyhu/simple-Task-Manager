#pragma once
#include "CChartCPU.h"
#include "CChartMemory.h"
#include "CChartGPU.h"

// CChartDlg 对话框

class CChartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChartDlg)

public:
	CChartDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChartDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl mChartTab;
	CChartCPU mChartCPU;
	CChartMemory mChartMemory;
	CChartGPU mChartGPU;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabChartMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangingTabChartMenu(NMHDR* pNMHDR, LRESULT* pResult);
};
