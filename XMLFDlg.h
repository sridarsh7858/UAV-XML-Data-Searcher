
// XMLFDlg.h : header file
//

#pragma once


// CXMLFDlg dialog
class CXMLFDlg : public CDialogEx
{
// Construction
public:
	CXMLFDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XMLF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_P;
	CComboBox m_TT;
	CComboBox m_TSD;
	CComboBox m_E;
	CComboBox m_A;
	CComboBox m_C;
	CString m_TN;
	afx_msg void OnBnClickedButton1();
};
