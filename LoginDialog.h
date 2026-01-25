#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <wx/wx.h>

class LoginDialog : public wxDialog
{
public:
    LoginDialog(wxWindow* parent);

    wxString GetUserType() { return m_userType; }
    wxString GetUsername() { return m_username; }

private:
    void OnLogin(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxTextCtrl* m_idCtrl;
    wxTextCtrl* m_passwordCtrl;
    wxRadioButton* m_clientRadio;
    wxRadioButton* m_adminRadio;

    wxString m_userType;
    wxString m_username;

    enum
    {
        ID_LOGIN = wxID_HIGHEST + 1,
        ID_CLIENT_RADIO,
        ID_ADMIN_RADIO
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // LOGINDIALOG_H
