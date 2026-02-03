/***************************************************************
 * Name:      ElectronicMarketMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Peace Fiacre (fiacreegoudjobi@gmail.com)
 * Created:   2026-01-16
 * Copyright: Peace Fiacre ()
 * License:
 **************************************************************/

#include "CatalogueFrame.h"
#include "PanierDialog.h"
#include "AdminProductsFrame.h"
#include "ElectronicMarketMain.h"
#include "LivreurFrame.h"
#include "ServiceClientFrame.h"
#include <wx/msgdlg.h>
#include <iostream>
#include "ClientSupportFrame.h"
#include "LoginDialog.h"
#include "Headers/Theme.h"


//(*InternalHeaders(ElectronicMarketFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(ElectronicMarketFrame)
const long ElectronicMarketFrame::idMenuQuit = wxNewId();
const long ElectronicMarketFrame::idMenuAbout = wxNewId();
const long ElectronicMarketFrame::ID_STATUSBAR1 = wxNewId();
const long ElectronicMarketFrame::idLogout = wxNewId();
const long ElectronicMarketFrame::idOpenCatalogue = wxNewId();
const long ElectronicMarketFrame::idOpenPanier = wxNewId();
const long ElectronicMarketFrame::idOpenSupport = wxNewId();
const long ElectronicMarketFrame::idOpenAdmin = wxNewId();
const long ElectronicMarketFrame::idOpenLivreur = wxNewId();
const long ElectronicMarketFrame::idOpenServiceClient = wxNewId();
//*)

BEGIN_EVENT_TABLE(ElectronicMarketFrame,wxFrame)
    //(*EventTable(ElectronicMarketFrame)
    //*)
END_EVENT_TABLE()

ElectronicMarketFrame::ElectronicMarketFrame(wxWindow* parent,wxWindowID id,
                                             const wxString& userType,
                                             const wxString& username)
    : m_userType(userType), m_username(username)
{
    //(*Initialize(ElectronicMarketFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;
    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnAbout);
    //*)

    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(Theme::Background());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "PLATEFORME E-COMMERCE");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(22);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);

    // Header : titre + label utilisateur + role badge + bouton deconnexion
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    headerSizer->Add(titleText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);

    m_userLabel = new wxStaticText(panel, wxID_ANY, wxEmptyString,
                                   wxDefaultPosition, wxDefaultSize);
    wxFont userLabelFont = m_userLabel->GetFont();
    userLabelFont.SetPointSize(10);
    m_userLabel->SetFont(userLabelFont);
    m_userLabel->SetForegroundColour(Theme::Muted());
    headerSizer->Add(m_userLabel, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);

    wxButton* logoutBtn = new wxButton(panel, wxID_ANY, "Se deconnecter",
                                        wxDefaultPosition, wxSize(140, 40));
    logoutBtn->SetBackgroundColour(Theme::Danger());
    logoutBtn->SetForegroundColour(*wxWHITE);
    logoutBtn->SetToolTip("Se déconnecter (Ctrl+L)");
    headerSizer->Add(logoutBtn, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 30);

    mainSizer->Add(headerSizer, 0, wxALL | wxEXPAND, 0);

    mainSizer->AddSpacer(10);

    wxFont labelFont;
    labelFont.SetPointSize(11);
    labelFont.SetWeight(wxFONTWEIGHT_BOLD);

    wxFont btnFont;
    btnFont.SetPointSize(12);
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);

    // ========================================
    // MODULE CLIENT
    // ========================================
    wxStaticText* clientLabel = new wxStaticText(panel, wxID_ANY, "MODULE CLIENT :");
    clientLabel->SetFont(labelFont);
    mainSizer->Add(clientLabel, 0, wxLEFT | wxTOP, 30);

    wxButton* catalogueBtn = new wxButton(panel, wxID_ANY, "Catalogue Produits",
                                          wxDefaultPosition, wxSize(350, 60));
    catalogueBtn->SetBackgroundColour(Theme::Primary());
    catalogueBtn->SetForegroundColour(*wxWHITE);
    catalogueBtn->SetFont(btnFont);
    catalogueBtn->SetToolTip("Voir le catalogue des produits (Ctrl+1)");
    mainSizer->Add(catalogueBtn, 0, wxLEFT | wxTOP, 30);

    wxButton* panierBtn = new wxButton(panel, wxID_ANY, "Mon Panier",
                                       wxDefaultPosition, wxSize(350, 60));
    panierBtn->SetBackgroundColour(Theme::Success());
    panierBtn->SetForegroundColour(*wxWHITE);
    panierBtn->SetFont(btnFont);
    panierBtn->SetToolTip("Voir le panier courant (Ctrl+2)");
    mainSizer->Add(panierBtn, 0, wxLEFT | wxTOP, 30);

    wxButton* supportBtn = new wxButton(panel, wxID_ANY, "Support & Reclamations",
                                        wxDefaultPosition, wxSize(350, 60));
    supportBtn->SetBackgroundColour(Theme::Warning());
    supportBtn->SetFont(btnFont);
    supportBtn->SetToolTip("Contacter le support client (Ctrl+3)");
    mainSizer->Add(supportBtn, 0, wxLEFT | wxTOP, 30);

    mainSizer->AddSpacer(20);

    // ========================================
    // MODULE ADMIN
    // ========================================
    wxStaticText* adminLabel = new wxStaticText(panel, wxID_ANY, "MODULE ADMIN :");
    adminLabel->SetFont(labelFont);
    mainSizer->Add(adminLabel, 0, wxLEFT | wxTOP, 30);

    wxButton* adminBtn = new wxButton(panel, wxID_ANY, "Gestion Produits",
                                      wxDefaultPosition, wxSize(350, 60));
    adminBtn->SetBackgroundColour(Theme::Neutral());
    adminBtn->SetForegroundColour(*wxWHITE);
    adminBtn->SetFont(btnFont);
    adminBtn->SetToolTip("Accès administration (Ctrl+4)");
    mainSizer->Add(adminBtn, 0, wxLEFT | wxTOP, 30);

    mainSizer->AddSpacer(20);

    // ========================================
    // MODULE LIVREUR
    // ========================================
    wxStaticText* livreurLabel = new wxStaticText(panel, wxID_ANY, "MODULE LIVREUR :");
    livreurLabel->SetFont(labelFont);
    mainSizer->Add(livreurLabel, 0, wxLEFT | wxTOP, 30);

    wxButton* livreurBtn = new wxButton(panel, wxID_ANY, "Mes Livraisons",
                                        wxDefaultPosition, wxSize(350, 60));
    livreurBtn->SetBackgroundColour(Theme::Accent());
    livreurBtn->SetForegroundColour(*wxWHITE);
    livreurBtn->SetFont(btnFont);
    livreurBtn->SetToolTip("Outils livreur (Ctrl+5)");
    mainSizer->Add(livreurBtn, 0, wxLEFT | wxTOP, 30);

    mainSizer->AddSpacer(20);

    // ========================================
    // MODULE SERVICE CLIENT
    // ========================================
    wxStaticText* serviceLabel = new wxStaticText(panel, wxID_ANY, "MODULE SERVICE CLIENT :");
    serviceLabel->SetFont(labelFont);
    mainSizer->Add(serviceLabel, 0, wxLEFT | wxTOP, 30);

    wxButton* serviceClientBtn = new wxButton(panel, wxID_ANY, "Reclamations et Retours",
                                              wxDefaultPosition, wxSize(350, 60));
    serviceClientBtn->SetBackgroundColour(Theme::Danger());
    serviceClientBtn->SetForegroundColour(*wxWHITE);
    serviceClientBtn->SetFont(btnFont);
    serviceClientBtn->SetToolTip("Interface service client (Ctrl+6)");
    mainSizer->Add(serviceClientBtn, 0, wxLEFT | wxTOP | wxBOTTOM, 30);

    panel->SetSizer(mainSizer);

    // Keyboard shortcuts and menu connects
    Connect(idLogout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnLogout);
    Connect(idOpenCatalogue,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenCatalogue);
    Connect(idOpenPanier,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenPanier);
    Connect(idOpenSupport,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenSupport);
    Connect(idOpenAdmin,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenAdmin);
    Connect(idOpenLivreur,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenLivreur);
    Connect(idOpenServiceClient,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ElectronicMarketFrame::OnOpenServiceClient);

    wxAcceleratorEntry entries[7];
    entries[0].Set(wxACCEL_CTRL, (int)'L', idLogout);
    entries[1].Set(wxACCEL_CTRL, (int)'1', idOpenCatalogue);
    entries[2].Set(wxACCEL_CTRL, (int)'2', idOpenPanier);
    entries[3].Set(wxACCEL_CTRL, (int)'3', idOpenSupport);
    entries[4].Set(wxACCEL_CTRL, (int)'4', idOpenAdmin);
    entries[5].Set(wxACCEL_CTRL, (int)'5', idOpenLivreur);
    entries[6].Set(wxACCEL_CTRL, (int)'6', idOpenServiceClient);
    wxAcceleratorTable accel(7, entries);
    SetAcceleratorTable(accel);

    // ========================================
    // ÉVÉNEMENTS DES BOUTONS
    // ========================================

    catalogueBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenCatalogue, this);

    panierBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenPanier, this);

    // ✅ BOUTON SUPPORT CORRIGÉ
    supportBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenSupport, this);



    adminBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenAdmin, this);

    livreurBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenLivreur, this);

    serviceClientBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnOpenServiceClient, this);

    // Déconnexion complète : appeler le handler centralisé
    logoutBtn->Bind(wxEVT_BUTTON, &ElectronicMarketFrame::OnLogout, this);

    // Afficher le type d'utilisateur dans la barre d'etat si disponible
    if(!m_username.IsEmpty() || !m_userType.IsEmpty())
    {
        wxString info = wxString::Format("Utilisateur : %s (%s)",
                                         m_username.IsEmpty() ? "-" : m_username,
                                         m_userType.IsEmpty() ? "-" : m_userType);
        StatusBar1->SetStatusText(info);
        // Mettre à jour le label utilisateur dans l'entete
        m_userLabel->SetLabel(wxString::Format("%s (%s)",
                                               m_username.IsEmpty() ? "-" : m_username,
                                               m_userType.IsEmpty() ? "-" : m_userType));

    }
    else
    {
        m_userLabel->SetLabel("-");
    }

    SetSize(650, 850);
    Centre();
}

ElectronicMarketFrame::~ElectronicMarketFrame()
{
    //(*Destroy(ElectronicMarketFrame)
    //*)
}

void ElectronicMarketFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void ElectronicMarketFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void ElectronicMarketFrame::OnOpenCatalogue(wxCommandEvent& event)
{
    CatalogueFrame* frame = new CatalogueFrame(NULL);
    frame->Show();
}

void ElectronicMarketFrame::OnOpenPanier(wxCommandEvent& event)
{
    PanierDialog dlg(this);
    dlg.ShowModal();
}

void ElectronicMarketFrame::OnOpenSupport(wxCommandEvent& event)
{
    ClientSupportFrame* frame = new ClientSupportFrame(NULL);
    frame->Show();
}

void ElectronicMarketFrame::OnOpenAdmin(wxCommandEvent& event)
{
    AdminProductsFrame* frame = new AdminProductsFrame(NULL);
    frame->Show();
}

void ElectronicMarketFrame::OnOpenLivreur(wxCommandEvent& event)
{
    LivreurFrame* frame = new LivreurFrame(NULL);
    frame->Show();
}

void ElectronicMarketFrame::OnOpenServiceClient(wxCommandEvent& event)
{
    ServiceClientFrame* frame = new ServiceClientFrame(NULL);
    frame->Show();
}

void ElectronicMarketFrame::OnLogout(wxCommandEvent& event)
{
    LoginDialog loginDlg(this);
    if(loginDlg.ShowModal() == wxID_OK)
    {
        wxString newType = loginDlg.GetUserType();
        wxString newUser = loginDlg.GetUsername();

        // Créer une nouvelle fenêtre principale pour la nouvelle session
        ElectronicMarketFrame* newFrame = new ElectronicMarketFrame(NULL, -1, newType, newUser);
        newFrame->Show();

        // Définir la nouvelle fenêtre comme top window de l'application
        if(wxTheApp)
            wxTheApp->SetTopWindow(newFrame);

        // Détruire l'ancienne fenêtre après que la nouvelle soit visible
        wxWindow* old = this;
        // Utiliser CallAfter sur l'ancienne fenêtre pour la détruire après l'événement courant
        old->CallAfter([old]() {
            if(old && old->IsShown())
                old->Destroy();
        });
    }
}
