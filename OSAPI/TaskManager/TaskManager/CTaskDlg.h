#pragma once


// CTaskDlg 对话框

class CTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskDlg)

public:
	CTaskDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TASK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_Tlist;
	//void InitTaskDlg();
public:
	//void InitTaskDlg();
	void BrowseWindow();
	double GetWindowCPUsage(CWnd* pWnd);
	SIZE_T GetWindowMemory(HWND hwnd);
	ULONGLONG GetWindowIOSpeed(HWND hwnd);
	afx_msg void OnBnClickedButtonClosewindow();
	virtual BOOL OnInitDialog();
};
