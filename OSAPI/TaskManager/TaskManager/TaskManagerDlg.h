
// TaskManagerDlg.h: 头文件
#include "CProcessDlg.h"
#include "CTaskDlg.h"
#include "CChartDlg.h"
//

#pragma once


// CTaskManagerDlg 对话框
class CTaskManagerDlg : public CDialogEx
{
// 构造
public:
	CTaskManagerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CTabCtrl m_Tab;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CProcessDlg m_Process;
	CTaskDlg m_Task;
	CChartDlg m_Chart;
	
public:
	afx_msg void OnTcnSelchangeTabMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangingTabMenu(NMHDR* pNMHDR, LRESULT* pResult);
};
