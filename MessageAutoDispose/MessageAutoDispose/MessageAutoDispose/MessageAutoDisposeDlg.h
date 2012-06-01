// MessageAutoDisposeDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CMessageAutoDisposeDlg 对话框
class CMessageAutoDisposeDlg : public CDialog
{
// 构造
public:
	CMessageAutoDisposeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MESSAGEAUTODISPOSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_OK;
	// 消息列表
	CTreeCtrl m_Messagelist;
private:
	TVINSERTSTRUCT tvInsert;
	HTREEITEM  m_CTOS;
	HTREEITEM  m_STOC;
	HTREEITEM  m_CTODP;
	HTREEITEM  m_DPTOC;
public:
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// 消息名称
	CEdit m_EditName;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// ID
	CEdit m_EditID;
	// 参数列表
	CListCtrl m_ParamList;
	// 注释框框...
	CEdit m_EditNote;
};
