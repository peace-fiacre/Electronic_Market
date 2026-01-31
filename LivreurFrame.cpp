/***************************************************************
 * Name:      LivreurFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#include "LivreurFrame.h"
#include "DatabaseManager.h"

wxBEGIN_EVENT_TABLE(LivreurFrame, wxFrame)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, LivreurFrame::OnLivraisonSelected)
    EVT_BUTTON(ID_ACCEPTER, LivreurFrame::OnAccepter)
    EVT_BUTTON(ID_CONFIRMER, LivreurFrame::OnConfirmer)
    EVT_BUTTON(ID_SIGNALER, LivreurFrame::OnSignalerProbleme)
    EVT_BUTTON(ID_REFRESH, LivreurFrame::OnRefresh)
wxEND_EVENT_TABLE()

LivreurFrame::LivreurFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Livreur - Mes Livraisons",
              wxDefaultPosition, wxSize(1200, 800))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(*wxWHITE);

    // Menu Bar
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* menuAccueil = new wxMenu;
    wxMenuItem* itemAccueil = menuAccueil->Append(wxID_ANY, "Retour menu principal");
    menuBar->Append(menuAccueil, "Accueil");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        this->Close();
    }, itemAccueil->GetId());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Titre
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "MES LIVRAISONS ASSIGNEES");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(0, 102, 204));
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // Statut
    m_statusText = new wxStaticText(panel, wxID_ANY,
                                     "5 livraisons en attente | 12 livrees aujourd'hui");
    wxFont statusFont = m_statusText->GetFont();
    statusFont.SetPointSize(10);
    m_statusText->SetFont(statusFont);
    mainSizer->Add(m_statusText, 0, wxALL | wxCENTER, 5);

    // Container pour liste et détails
    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

    // Liste des livraisons
    m_livraisonsList = new wxListCtrl(panel, wxID_ANY,
                                       wxDefaultPosition,
                                       wxSize(650, 500),
                                       wxLC_REPORT | wxLC_SINGLE_SEL);

    m_livraisonsList->InsertColumn(0, "ID", wxLIST_FORMAT_CENTER, 50);
    m_livraisonsList->InsertColumn(1, "N° Suivi", wxLIST_FORMAT_LEFT, 120);
    m_livraisonsList->InsertColumn(2, "Client", wxLIST_FORMAT_LEFT, 150);
    m_livraisonsList->InsertColumn(3, "Adresse", wxLIST_FORMAT_LEFT, 200);
    m_livraisonsList->InsertColumn(4, "Statut", wxLIST_FORMAT_CENTER, 130);

    contentSizer->Add(m_livraisonsList, 0, wxALL, 10);

    // Zone détails
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* detailsLabel = new wxStaticText(panel, wxID_ANY, "DETAILS DE LA LIVRAISON");
    wxFont detailsFont = detailsLabel->GetFont();
    detailsFont.SetPointSize(12);
    detailsFont.SetWeight(wxFONTWEIGHT_BOLD);
    detailsLabel->SetFont(detailsFont);
    rightSizer->Add(detailsLabel, 0, wxALL, 5);

    m_detailsText = new wxTextCtrl(panel, wxID_ANY,
                                    "Selectionnez une livraison pour voir les details...",
                                    wxDefaultPosition,
                                    wxSize(420, 400),
                                    wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);
    rightSizer->Add(m_detailsText, 1, wxALL | wxEXPAND, 5);

    contentSizer->Add(rightSizer, 1, wxEXPAND);

    mainSizer->Add(contentSizer, 1, wxALL | wxEXPAND, 10);

    // Boutons d'action
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* accepterBtn = new wxButton(panel, ID_ACCEPTER, "Accepter",
                                         wxDefaultPosition, wxSize(130, 40));
    accepterBtn->SetBackgroundColour(wxColour(40, 167, 69));
    accepterBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(accepterBtn, 0, wxRIGHT, 10);

    wxButton* confirmerBtn = new wxButton(panel, ID_CONFIRMER, "Confirmer livraison",
                                          wxDefaultPosition, wxSize(150, 40));
    confirmerBtn->SetBackgroundColour(wxColour(0, 123, 255));
    confirmerBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(confirmerBtn, 0, wxRIGHT, 10);

    wxButton* signalerBtn = new wxButton(panel, ID_SIGNALER, "Signaler probleme",
                                         wxDefaultPosition, wxSize(150, 40));
    signalerBtn->SetBackgroundColour(wxColour(255, 193, 7));
    btnSizer->Add(signalerBtn, 0, wxRIGHT, 10);

    wxButton* refreshBtn = new wxButton(panel, ID_REFRESH, "Actualiser",
                                        wxDefaultPosition, wxSize(130, 40));
    btnSizer->Add(refreshBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxCENTER, 20);

    panel->SetSizer(mainSizer);

    PopulateLivraisons();

    Maximize(true);
}

LivreurFrame::~LivreurFrame()
{
}


void LivreurFrame::PopulateLivraisons()
{
    m_livraisonsList->DeleteAllItems();

    // CHARGER DEPUIS LA BASE DE DONNÉES
    std::vector<Order> orders = DatabaseManager::GetInstance().GetPendingDeliveries();

    for(size_t i = 0; i < orders.size(); i++)
    {
        const Order& o = orders[i];

        long index = m_livraisonsList->InsertItem(i, wxString::Format("%d", o.id));
        m_livraisonsList->SetItem(index, 1, o.numero_suivi);
        m_livraisonsList->SetItem(index, 2, "Client " + wxString::Format("%d", o.id_client));
        m_livraisonsList->SetItem(index, 3, o.adresse);
        m_livraisonsList->SetItem(index, 4, o.statut);

        if(o.statut == "En attente")
            m_livraisonsList->SetItemTextColour(index, wxColour(255, 140, 0));
        else if(o.statut == "En cours")
            m_livraisonsList->SetItemTextColour(index, wxColour(0, 123, 255));
        else
            m_livraisonsList->SetItemTextColour(index, wxColour(40, 167, 69));
    }

    m_statusText->SetLabel(wxString::Format("%zu livraisons en attente", orders.size()));
}






void LivreurFrame::OnLivraisonSelected(wxListEvent& event)
{
    long index = event.GetIndex();

    wxString numeroSuivi = m_livraisonsList->GetItemText(index, 1);
    wxString client = m_livraisonsList->GetItemText(index, 2);
    wxString adresse = m_livraisonsList->GetItemText(index, 3);
    wxString statut = m_livraisonsList->GetItemText(index, 4);

    wxString details;
    details << "===============================\n";
    details << "INFORMATIONS DE LIVRAISON\n";
    details << "===============================\n\n";
    details << "N° de suivi : " << numeroSuivi << "\n\n";
    details << "===============================\n";
    details << "CLIENT\n";
    details << "===============================\n";
    details << "Nom : " << client << "\n";
    details << "Telephone : +229 97 XX XX XX\n\n";
    details << "===============================\n";
    details << "ADRESSE DE LIVRAISON\n";
    details << "===============================\n";
    details << adresse << "\n\n";
    details << "===============================\n";
    details << "PRODUITS\n";
    details << "===============================\n";
    details << "• Smartphone Samsung (x1)\n";
    details << "• Ecouteurs Sony (x2)\n\n";
    details << "===============================\n";
    details << "MONTANT\n";
    details << "===============================\n";
    details << "Total : 520 000 F CFA\n";
    details << "Mode paiement : Carte bancaire\n";
    details << "Statut paiement : Paye\n\n";
    details << "===============================\n";
    details << "STATUT ACTUEL\n";
    details << "===============================\n";
    details << statut << "\n\n";
    details << "Date prevue : 20/01/2026\n";
    details << "Delai estime : 24-48h\n";

    m_detailsText->SetValue(details);
}

void LivreurFrame::OnAccepter(wxCommandEvent& event)
{
    long selected = m_livraisonsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une livraison.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numeroSuivi = m_livraisonsList->GetItemText(selected, 1);

    int response = wxMessageBox(
        "Accepter la livraison " + numeroSuivi + " ?\n\n"
        "Vous vous engagez a effectuer cette livraison.",
        "Confirmation",
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        m_livraisonsList->SetItem(selected, 4, "Acceptee");
        m_livraisonsList->SetItemTextColour(selected, wxColour(40, 167, 69));

        wxMessageBox("Livraison acceptee avec succes !\n\n"
                     "Vous pouvez maintenant proceder a la livraison.",
                     "Succes",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void LivreurFrame::OnConfirmer(wxCommandEvent& event)
{
    long selected = m_livraisonsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une livraison.",
                     "Aucune selection", wxOK | wxICON_WARNING, this);
        return;
    }

    wxString idStr = m_livraisonsList->GetItemText(selected, 0);
    long id;
    idStr.ToLong(&id);

    wxString numeroSuivi = m_livraisonsList->GetItemText(selected, 1);
    wxString client = m_livraisonsList->GetItemText(selected, 2);

    int response = wxMessageBox(
        "Confirmer la livraison au client " + client + " ?\n\n"
        "N° de suivi : " + numeroSuivi + "\n\n"
        "Cette action est irreversible.",
        "Confirmation de livraison",
        wxYES_NO | wxICON_QUESTION, this);

    if(response == wxYES)
    {
        wxTextEntryDialog codeDialog(
            this,
            "Entrez le code de confirmation du client :",
            "Code de livraison",
            ""
        );

        if(codeDialog.ShowModal() == wxID_OK)
        {
            wxString code = codeDialog.GetValue();

            if(!code.IsEmpty())
            {
                // METTRE À JOUR EN BASE DE DONNÉES
                if(DatabaseManager::GetInstance().UpdateOrderStatus((int)id, "Livree"))
                {
                    wxMessageBox("Livraison confirmee avec succes !\n\n"
                                 "Code : " + code + "\n"
                                 "Date : 19/01/2026 14:32\n\n"
                                 "Le client recevra une notification.",
                                 "Livraison reussie",
                                 wxOK | wxICON_INFORMATION, this);

                    // ACTUALISER LA LISTE
                    PopulateLivraisons();
                }
                else
                {
                    wxMessageBox("Erreur lors de la mise a jour.",
                                 "Erreur", wxOK | wxICON_ERROR, this);
                }
            }
        }
    }
}





void LivreurFrame::OnSignalerProbleme(wxCommandEvent& event)
{
    long selected = m_livraisonsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une livraison.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numeroSuivi = m_livraisonsList->GetItemText(selected, 1);

    wxArrayString problemes;
    problemes.Add("Client absent");
    problemes.Add("Adresse introuvable");
    problemes.Add("Client refuse le colis");
    problemes.Add("Colis endommage");
    problemes.Add("Probleme de paiement");
    problemes.Add("Autre (preciser)");

    wxSingleChoiceDialog choiceDialog(
        this,
        "Selectionnez le type de probleme :",
        "Signaler un probleme - " + numeroSuivi,
        problemes
    );

    if(choiceDialog.ShowModal() == wxID_OK)
    {
        wxString probleme = choiceDialog.GetStringSelection();

        wxTextEntryDialog detailsDialog(
            this,
            "Decrivez le probleme en details :",
            "Details du probleme",
            "",
            wxOK | wxCANCEL | wxTE_MULTILINE
        );

        if(detailsDialog.ShowModal() == wxID_OK)
        {
            wxString details = detailsDialog.GetValue();

            m_livraisonsList->SetItem(selected, 4, "Probleme");
            m_livraisonsList->SetItemTextColour(selected, wxColour(220, 53, 69));

            wxMessageBox("Probleme signale avec succes !\n\n"
                         "Type : " + probleme + "\n"
                         "Details : " + details + "\n\n"
                         "Le service client sera notifie.",
                         "Signalement enregistre",
                         wxOK | wxICON_INFORMATION,
                         this);
        }
    }
}

void LivreurFrame::OnRefresh(wxCommandEvent& event)
{
    m_livraisonsList->DeleteAllItems();
    PopulateLivraisons();
    m_detailsText->SetValue("Selectionnez une livraison pour voir les details...");

    wxMessageBox("Liste des livraisons actualisee !",
                 "Actualisation",
                 wxOK | wxICON_INFORMATION,
                 this);
}
