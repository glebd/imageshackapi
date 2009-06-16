#include "StdAfx.h"
#include "LoginDlg.h"

CLoginDlg::CLoginDlg(ImageShackAPI &rAPI)
	: m_rAPI(rAPI)
{
}

LRESULT CLoginDlg::OnInitDialog(HWND, LPARAM)
{
	DoDataExchange(FALSE);

	SetIcon(AtlLoadIconImage(IDI_LOGIN, LR_DEFAULTCOLOR , ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON)), FALSE);

	// center the dialog on the screen
	CenterWindow();

	CreateToolTipCtrl();
	AddTool(IDC_LOGIN, IDS_TOOLTIP_YOUR_LOGIN);
	AddTool(IDC_PASSWORD, IDS_TOOLTIP_ENTER_PASSWORD);
	ActivateToolTips();

    CString strLoginTo;
    if (strLoginTo.LoadString(IDS_LOGIN_TO))
    {
        strLoginTo.Replace( _T("%domain%"), GetString(IDS_DOMAIN) );
        SetWindowText(strLoginTo);
    }
    
	return TRUE;
}

int CLoginDlg::OnOK()
{
	DoDataExchange(TRUE);

	if (GetLogin().IsEmpty())
		return ShowMessageAndSetFocus(IDS_ENTER_LOGIN, IDC_LOGIN);

	if (GetPassword().IsEmpty())
		return ShowMessageAndSetFocus(IDS_ENTER_PASSWORD, IDC_PASSWORD);

	ErrorResponse erError;
	if (!m_rAPI.Login(GetLogin(), GetPassword(), &erError))
	{
        if (erError.code == _T("authentication_failed"))
			MessageBox(GetString(IDS_WRONG_CREDENTIALS), GetWindowTitle(), MB_ICONSTOP|MB_OK);
		else if (!erError.message.IsEmpty())
			MessageBox(erError.message, GetWindowTitle(), MB_ICONSTOP|MB_OK);
		else
			MessageBox(GetString(IDS_LOGIN_FAILED), GetWindowTitle(), MB_ICONSTOP|MB_OK);
	}
	else
	{
		EndDialog(IDOK);
	}

	return 0;
}

void CLoginDlg::OnFogotPassword()
{
	DoDataExchange(TRUE);

    API::Win32::Shell::Execute(Configuration().urlForgotPassword);
}

void CLoginDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

LRESULT CLoginDlg::OnMouse()
{
	m_tooltips.RelayEvent((LPMSG)GetCurrentMessage());

	SetMsgHandled(FALSE);
	return 0;
}
