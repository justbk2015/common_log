
// mfctestlogDlg.h : ͷ�ļ�
//

#pragma once


// CmfctestlogDlg �Ի���
class CmfctestlogDlg : public CDialogEx
{
// ����
public:
	CmfctestlogDlg(CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~CmfctestlogDlg();
// �Ի�������
	enum { IDD = IDD_MFCTESTLOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
public:
    zb::Log m_logger;
};
