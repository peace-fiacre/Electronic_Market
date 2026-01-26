/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/




#include "LoginDialog.h"

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_BUTTON(ID_LOGIN, LoginDialog::OnLogin)
    EVT_BUTTON(wxID_CANCEL, LoginDialog::OnCancel)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Connexion - Plateforme E-Commerce",
               wxDefaultPosition, wxSize(600, 700))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->AddSpacer(20);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "CONNEXION");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(0, 102, 204));
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 10);

    mainSizer->AddSpacer(20);

    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Je suis :");
    wxFont labelFont = typeLabel->GetFont();
    labelFont.SetPointSize(11);
    labelFont.SetWeight(wxFONTWEIGHT_BOLD);
    typeLabel->SetFont(labelFont);
    mainSizer->Add(typeLabel, 0, wxLEFT, 50);

    mainSizer->AddSpacer(10);

    wxBoxSizer* radioSizer = new wxBoxSizer(wxHORIZONTAL);

    m_clientRadio = new wxRadioButton(panel, ID_CLIENT_RADIO,
                                       "Client / Acheteur",
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxRB_GROUP);
    m_clientRadio->SetValue(true);
    radioSizer->Add(m_clientRadio, 0, wxRIGHT, 30);

    m_adminRadio = new wxRadioButton(panel, ID_ADMIN_RADIO, "Administrateur");
    radioSizer->Add(m_adminRadio, 0);

    mainSizer->Add(radioSizer, 0, wxLEFT, 50);

    mainSizer->AddSpacer(20);

    wxStaticText* idLabel = new wxStaticText(panel, wxID_ANY, "Identifiant :");
    idLabel->SetFont(labelFont);
    mainSizer->Add(idLabel, 0, wxLEFT, 50);

    mainSizer->AddSpacer(5);

    m_idCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                              wxDefaultPosition, wxSize(450, 40));
    m_idCtrl->SetHint("Votre identifiant...");
    mainSizer->Add(m_idCtrl, 0, wxLEFT | wxRIGHT, 50);

    mainSizer->AddSpacer(20);

    wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Mot de passe :");
    passwordLabel->SetFont(labelFont);
    mainSizer->Add(passwordLabel, 0, wxLEFT, 50);

    mainSizer->AddSpacer(5);

    m_passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                                    wxDefaultPosition, wxSize(450, 40),
                                    wxTE_PASSWORD);
    m_passwordCtrl->SetHint("Votre mot de passe...");
    mainSizer->Add(m_passwordCtrl, 0, wxLEFT | wxRIGHT, 50);

    mainSizer->AddSpacer(30);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* loginBtn = new wxButton(panel, ID_LOGIN, "Se connecter",
                                      wxDefaultPosition, wxSize(200, 45));
    loginBtn->SetBackgroundColour(wxColour(0, 120, 215));
    loginBtn->SetForegroundColour(*wxWHITE);
    wxFont btnFont = loginBtn->GetFont();
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);
    loginBtn->SetFont(btnFont);
    btnSizer->Add(loginBtn, 0, wxRIGHT, 10);

    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, "Annuler",
                                       wxDefaultPosition, wxSize(200, 45));
    btnSizer->Add(cancelBtn, 0);

    mainSizer->Add(btnSizer, 0, wxCENTER);

    mainSizer->AddSpacer(20);

    panel->SetSizer(mainSizer);

    Centre();
    m_idCtrl->SetFocus();
}

void LoginDialog::OnLogin(wxCommandEvent& event)
{
    wxString id = m_idCtrl->GetValue().Trim();
    wxString password = m_passwordCtrl->GetValue();

    if(id.IsEmpty())
    {
        wxMessageBox("Veuillez entrer votre identifiant.",
                     "Champ requis",
                     wxOK | wxICON_WARNING,
                     this);
        m_idCtrl->SetFocus();
        return;
    }

    if(password.IsEmpty())
    {
        wxMessageBox("Veuillez entrer votre mot de passe.",
                     "Champ requis",
                     wxOK | wxICON_WARNING,
                     this);
        m_passwordCtrl->SetFocus();
        return;
    }

    if(m_clientRadio->GetValue())
    {
        m_userType = "CLIENT";
    }
    else if(m_adminRadio->GetValue())
    {
        m_userType = "ADMIN";
    }

    bool loginSuccess = false;
    wxString message;

    if(m_userType == "CLIENT")
    {
        if(id == "client" && password == "client123")
        {
            loginSuccess = true;
            message = "Bienvenue Client : " + id;
        }
        else
        {
            message = "Identifiant ou mot de passe client incorrect.\n\n"
                     "Pour tester :\nID: client\nMDP: client123";
        }
    }
    else if(m_userType == "ADMIN")
    {
        if(id == "admin" && password == "admin123")
        {
            loginSuccess = true;
            message = "Bienvenue Administrateur : " + id;
        }
        else
        {
            message = "Identifiant ou mot de passe admin incorrect.\n\n"
                     "Pour tester :\nID: admin\nMDP: admin123";
        }
    }

    if(loginSuccess)
    {
        m_username = id;
        wxMessageBox(message,
                     "Connexion reussie",
                     wxOK | wxICON_INFORMATION,
                     this);
        EndModal(wxID_OK);
    }
    else
    {
        wxMessageBox(message,
                     "Echec de connexion",
                     wxOK | wxICON_ERROR,
                     this);
        m_passwordCtrl->Clear();
        m_passwordCtrl->SetFocus();
    }
}

void LoginDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
