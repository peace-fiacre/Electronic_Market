/***************************************************************
 * Name:      ClientSupportFrame.cpp
 * Purpose:   Interface Support & Reclamations pour les clients
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-31
 **************************************************************/

#include "ClientSupportFrame.h"
#include "DatabaseManager.h"

wxBEGIN_EVENT_TABLE(ClientSupportFrame, wxFrame)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, ClientSupportFrame::OnOrderSelected)
    EVT_BUTTON(ID_VIEW_DETAILS, ClientSupportFrame::OnViewOrderDetails)
    EVT_BUTTON(ID_TRACK_ORDER, ClientSupportFrame::OnTrackOrder)
    EVT_BUTTON(ID_REORDER, ClientSupportFrame::OnReorder)
    EVT_BUTTON(ID_NEW_CLAIM, ClientSupportFrame::OnNewClaim)
    EVT_BUTTON(ID_REQUEST_RETURN, ClientSupportFrame::OnRequestReturn)
wxEND_EVENT_TABLE()

ClientSupportFrame::ClientSupportFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Support Client - Mes Commandes & Reclamations",
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
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "SUPPORT CLIENT");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(0, 102, 204));
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // Notebook
    m_notebook = new wxNotebook(panel, wxID_ANY);

    CreateOrdersPage();
    CreateClaimsPage();

    m_notebook->AddPage(m_ordersPage, "Mes Commandes");
    m_notebook->AddPage(m_claimsPage, "Mes Reclamations");

    mainSizer->Add(m_notebook, 1, wxALL | wxEXPAND, 20);

    panel->SetSizer(mainSizer);

    Maximize(true);
}

ClientSupportFrame::~ClientSupportFrame()
{
}

void ClientSupportFrame::CreateOrdersPage()
{
    m_ordersPage = new wxPanel(m_notebook, wxID_ANY);
    m_ordersPage->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* pageSizer = new wxBoxSizer(wxVERTICAL);

    // Info
    wxStaticText* infoText = new wxStaticText(m_ordersPage, wxID_ANY,
        "Consultez l'historique de toutes vos commandes et suivez leur statut");
    infoText->SetForegroundColour(wxColour(100, 100, 100));
    pageSizer->Add(infoText, 0, wxALL, 10);

    // Liste des commandes
    m_ordersList = new wxListCtrl(m_ordersPage, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(1080, 350),
                                   wxLC_REPORT | wxLC_SINGLE_SEL);

    m_ordersList->InsertColumn(0, "N° Commande", wxLIST_FORMAT_LEFT, 120);
    m_ordersList->InsertColumn(1, "Date", wxLIST_FORMAT_LEFT, 150);
    m_ordersList->InsertColumn(2, "Montant", wxLIST_FORMAT_RIGHT, 120);
    m_ordersList->InsertColumn(3, "Statut", wxLIST_FORMAT_CENTER, 150);
    m_ordersList->InsertColumn(4, "Adresse", wxLIST_FORMAT_LEFT, 300);
    m_ordersList->InsertColumn(5, "N° Suivi", wxLIST_FORMAT_LEFT, 140);

    pageSizer->Add(m_ordersList, 0, wxALL | wxEXPAND, 10);

    // Zone détails
    wxStaticText* detailsLabel = new wxStaticText(m_ordersPage, wxID_ANY, "Details de la commande :");
    wxFont detailsFont = detailsLabel->GetFont();
    detailsFont.SetWeight(wxFONTWEIGHT_BOLD);
    detailsLabel->SetFont(detailsFont);
    pageSizer->Add(detailsLabel, 0, wxLEFT | wxTOP, 10);

    m_orderDetailsText = new wxTextCtrl(m_ordersPage, wxID_ANY,
                                        "Selectionnez une commande pour voir les details...",
                                        wxDefaultPosition,
                                        wxSize(1080, 200),
                                        wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);
    pageSizer->Add(m_orderDetailsText, 0, wxALL | wxEXPAND, 10);

    // Boutons d'action
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* viewBtn = new wxButton(m_ordersPage, ID_VIEW_DETAILS, "Voir details",
                                     wxDefaultPosition, wxSize(140, 40));
    viewBtn->SetBackgroundColour(wxColour(0, 123, 255));
    viewBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(viewBtn, 0, wxRIGHT, 10);

    wxButton* trackBtn = new wxButton(m_ordersPage, ID_TRACK_ORDER, "Suivre livraison",
                                      wxDefaultPosition, wxSize(140, 40));
    trackBtn->SetBackgroundColour(wxColour(255, 140, 0));
    trackBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(trackBtn, 0, wxRIGHT, 10);

    wxButton* reorderBtn = new wxButton(m_ordersPage, ID_REORDER, "Commander a nouveau",
                                        wxDefaultPosition, wxSize(180, 40));
    reorderBtn->SetBackgroundColour(wxColour(40, 167, 69));
    reorderBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(reorderBtn, 0);

    pageSizer->Add(btnSizer, 0, wxALL | wxLEFT, 10);

    m_ordersPage->SetSizer(pageSizer);

    LoadOrders();
}

void ClientSupportFrame::CreateClaimsPage()
{
    m_claimsPage = new wxPanel(m_notebook, wxID_ANY);
    m_claimsPage->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* pageSizer = new wxBoxSizer(wxVERTICAL);

    // Info
    wxStaticText* infoText = new wxStaticText(m_claimsPage, wxID_ANY,
        "Deposez une reclamation ou demandez un retour de produit");
    infoText->SetForegroundColour(wxColour(100, 100, 100));
    pageSizer->Add(infoText, 0, wxALL, 10);

    // Liste des réclamations
    m_claimsList = new wxListCtrl(m_claimsPage, wxID_ANY,
                                  wxDefaultPosition,
                                  wxSize(1080, 350),
                                  wxLC_REPORT | wxLC_SINGLE_SEL);

    m_claimsList->InsertColumn(0, "N° Reclamation", wxLIST_FORMAT_LEFT, 130);
    m_claimsList->InsertColumn(1, "N° Commande", wxLIST_FORMAT_LEFT, 130);
    m_claimsList->InsertColumn(2, "Type", wxLIST_FORMAT_LEFT, 180);
    m_claimsList->InsertColumn(3, "Description", wxLIST_FORMAT_LEFT, 350);
    m_claimsList->InsertColumn(4, "Date", wxLIST_FORMAT_LEFT, 120);
    m_claimsList->InsertColumn(5, "Statut", wxLIST_FORMAT_CENTER, 120);

    pageSizer->Add(m_claimsList, 0, wxALL | wxEXPAND, 10);

    // Zone détails
    wxStaticText* detailsLabel = new wxStaticText(m_claimsPage, wxID_ANY, "Details de la reclamation :");
    wxFont detailsFont = detailsLabel->GetFont();
    detailsFont.SetWeight(wxFONTWEIGHT_BOLD);
    detailsLabel->SetFont(detailsFont);
    pageSizer->Add(detailsLabel, 0, wxLEFT | wxTOP, 10);

    m_claimDetailsText = new wxTextCtrl(m_claimsPage, wxID_ANY,
                                        "Selectionnez une reclamation pour voir les details...",
                                        wxDefaultPosition,
                                        wxSize(1080, 200),
                                        wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);
    pageSizer->Add(m_claimDetailsText, 0, wxALL | wxEXPAND, 10);

    // Boutons d'action
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* newClaimBtn = new wxButton(m_claimsPage, ID_NEW_CLAIM, "Nouvelle reclamation",
                                         wxDefaultPosition, wxSize(180, 40));
    newClaimBtn->SetBackgroundColour(wxColour(220, 53, 69));
    newClaimBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(newClaimBtn, 0, wxRIGHT, 10);

    wxButton* returnBtn = new wxButton(m_claimsPage, ID_REQUEST_RETURN, "Demander un retour",
                                       wxDefaultPosition, wxSize(180, 40));
    returnBtn->SetBackgroundColour(wxColour(255, 193, 7));
    btnSizer->Add(returnBtn, 0);

    pageSizer->Add(btnSizer, 0, wxALL | wxLEFT, 10);

    m_claimsPage->SetSizer(pageSizer);

    LoadClaims();
}

void ClientSupportFrame::LoadOrders()
{
    m_ordersList->DeleteAllItems();

    // Charger depuis la BD
    std::vector<Order> orders = DatabaseManager::GetInstance().GetAllOrders();

    for(size_t i = 0; i < orders.size(); i++)
    {
        const Order& o = orders[i];

        long idx = m_ordersList->InsertItem(i, "CMD-" + wxString::Format("%d", o.id));
        m_ordersList->SetItem(idx, 1, o.date);
        m_ordersList->SetItem(idx, 2, wxString::Format("%.0f F", o.montant_total));
        m_ordersList->SetItem(idx, 3, o.statut);
        m_ordersList->SetItem(idx, 4, o.adresse);
        m_ordersList->SetItem(idx, 5, o.numero_suivi);

        // Couleur selon statut
        if(o.statut == "Livree")
            m_ordersList->SetItemTextColour(idx, wxColour(40, 167, 69));
        else if(o.statut == "En cours")
            m_ordersList->SetItemTextColour(idx, wxColour(255, 140, 0));
        else
            m_ordersList->SetItemTextColour(idx, wxColour(0, 123, 255));

        // Stocker l'ID
        m_ordersList->SetItemData(idx, o.id);
    }
}

void ClientSupportFrame::LoadClaims()
{
    m_claimsList->DeleteAllItems();

    // Charger depuis la BD
    std::vector<Reclamation> claims = DatabaseManager::GetInstance().GetAllReclamations();

    for(size_t i = 0; i < claims.size(); i++)
    {
        const Reclamation& r = claims[i];

        long idx = m_claimsList->InsertItem(i, "REC-" + wxString::Format("%d", r.id));
        m_claimsList->SetItem(idx, 1, "CMD-" + wxString::Format("%d", r.id_commande));
        m_claimsList->SetItem(idx, 2, r.type);
        m_claimsList->SetItem(idx, 3, r.description);
        m_claimsList->SetItem(idx, 4, r.date);
        m_claimsList->SetItem(idx, 5, r.statut);

        // Couleur selon statut
        if(r.statut == "Nouveau")
            m_claimsList->SetItemTextColour(idx, wxColour(220, 53, 69));
        else if(r.statut == "En cours")
            m_claimsList->SetItemTextColour(idx, wxColour(255, 193, 7));
        else
            m_claimsList->SetItemTextColour(idx, wxColour(40, 167, 69));

        m_claimsList->SetItemData(idx, r.id);
    }
}

void ClientSupportFrame::OnOrderSelected(wxListEvent& event)
{
    if(m_notebook->GetSelection() != 0) return; // Pas dans l'onglet commandes

    long index = event.GetIndex();
    int id_commande = (int)m_ordersList->GetItemData(index);

    wxString numCommande = m_ordersList->GetItemText(index, 0);
    wxString date = m_ordersList->GetItemText(index, 1);
    wxString montant = m_ordersList->GetItemText(index, 2);
    wxString statut = m_ordersList->GetItemText(index, 3);
    wxString adresse = m_ordersList->GetItemText(index, 4);
    wxString numSuivi = m_ordersList->GetItemText(index, 5);

    // Charger les lignes de commande
    std::vector<OrderLine> lines = DatabaseManager::GetInstance().GetOrderLines(id_commande);

    wxString details;
    details << "===============================\n";
    details << "DETAILS DE LA COMMANDE\n";
    details << "===============================\n\n";
    details << "Numero : " << numCommande << "\n";
    details << "Date : " << date << "\n";
    details << "Statut : " << statut << "\n";
    details << "Numero de suivi : " << numSuivi << "\n\n";
    details << "===============================\n";
    details << "ADRESSE DE LIVRAISON\n";
    details << "===============================\n";
    details << adresse << "\n\n";
    details << "===============================\n";
    details << "PRODUITS COMMANDES\n";
    details << "===============================\n";

    for(const auto& line : lines)
    {
        details << "- " << line.nom_produit << "\n";
        details << "  Quantite : " << line.quantite << "\n";
        details << "  Prix unitaire : " << wxString::Format("%.0f F", line.prix_unitaire) << "\n";
        details << "  Sous-total : " << wxString::Format("%.0f F", line.prix_unitaire * line.quantite) << "\n\n";
    }

    details << "===============================\n";
    details << "MONTANT TOTAL : " << montant << "\n";
    details << "===============================\n";

    m_orderDetailsText->SetValue(details);
}

void ClientSupportFrame::OnViewOrderDetails(wxCommandEvent& event)
{
    long selected = m_ordersList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une commande.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    // Les détails sont déjà affichés dans m_orderDetailsText
    wxMessageBox("Les details de la commande sont affiches ci-dessous.",
                 "Details",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void ClientSupportFrame::OnTrackOrder(wxCommandEvent& event)
{
    long selected = m_ordersList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une commande.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numSuivi = m_ordersList->GetItemText(selected, 5);
    wxString statut = m_ordersList->GetItemText(selected, 3);

    wxString tracking;
    tracking << "=== SUIVI DE LIVRAISON ===\n\n";
    tracking << "Numero de suivi : " << numSuivi << "\n";
    tracking << "Statut actuel : " << statut << "\n\n";
    tracking << "--- HISTORIQUE ---\n";
    tracking << "[" << wxDateTime::Now().Format("%d/%m/%Y %H:%M") << "] Commande recue\n";
    tracking << "[" << wxDateTime::Now().Format("%d/%m/%Y %H:%M") << "] Paiement confirme\n";

    if(statut != "En attente")
    {
        tracking << "[" << wxDateTime::Now().Format("%d/%m/%Y %H:%M") << "] En cours de preparation\n";
    }

    if(statut == "En cours")
    {
        tracking << "[" << wxDateTime::Now().Format("%d/%m/%Y %H:%M") << "] Expedie\n";
    }

    if(statut == "Livree")
    {
        tracking << "[" << wxDateTime::Now().Format("%d/%m/%Y %H:%M") << "] Livre avec succes\n";
    }

    wxMessageBox(tracking, "Suivi de livraison", wxOK | wxICON_INFORMATION, this);
}

void ClientSupportFrame::OnReorder(wxCommandEvent& event)
{
    long selected = m_ordersList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une commande.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numCommande = m_ordersList->GetItemText(selected, 0);

    int response = wxMessageBox(
        "Recommander les memes produits que la commande " + numCommande + " ?",
        "Commander a nouveau",
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        wxMessageBox("Les produits ont ete ajoutes a votre panier.\n\n"
                     "Rendez-vous dans le catalogue pour finaliser votre commande.",
                     "Produits ajoutes",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void ClientSupportFrame::OnClaimSelected(wxListEvent& event)
{
    // Similaire à OnOrderSelected mais pour réclamations
}

void ClientSupportFrame::OnNewClaim(wxCommandEvent& event)
{
    // Dialogue pour nouvelle réclamation
    wxArrayString types;
    types.Add("Produit defectueux");
    types.Add("Livraison en retard");
    types.Add("Produit manquant");
    types.Add("Erreur de commande");
    types.Add("Autre");

    wxSingleChoiceDialog typeDialog(
        this,
        "Selectionnez le type de reclamation :",
        "Nouvelle reclamation",
        types
    );

    if(typeDialog.ShowModal() != wxID_OK)
        return;

    wxString type = typeDialog.GetStringSelection();

    wxTextEntryDialog descDialog(
        this,
        "Decrivez votre probleme en details :",
        "Description de la reclamation",
        "",
        wxOK | wxCANCEL | wxTE_MULTILINE
    );

    if(descDialog.ShowModal() != wxID_OK)
        return;

    wxString description = descDialog.GetValue();

    if(description.IsEmpty())
    {
        wxMessageBox("Veuillez decrire votre probleme.",
                     "Description requise",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    // Créer la réclamation (ID commande = 1 pour démo)
    int id_reclamation = DatabaseManager::GetInstance().CreateReclamation(
        1, "Client Test", type, description
    );

    if(id_reclamation > 0)
    {
        wxMessageBox("Reclamation enregistree avec succes !\n\n"
                     "Numero : REC-" + wxString::Format("%d", id_reclamation) + "\n\n"
                     "Notre service client vous contactera sous 24-48h.",
                     "Reclamation enregistree",
                     wxOK | wxICON_INFORMATION,
                     this);

        LoadClaims();
    }
    else
    {
        wxMessageBox("Erreur lors de l'enregistrement.",
                     "Erreur",
                     wxOK | wxICON_ERROR,
                     this);
    }
}

void ClientSupportFrame::OnRequestReturn(wxCommandEvent& event)
{
    wxMessageBox("Fonctionnalite de demande de retour.\n\n"
                 "Le client pourra demander le retour d'un produit apres livraison.",
                 "Demande de retour",
                 wxOK | wxICON_INFORMATION,
                 this);
}
