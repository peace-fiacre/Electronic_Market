/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#include "DatabaseManager.h"
#include "ServiceClientFrame.h"

wxBEGIN_EVENT_TABLE(ServiceClientFrame, wxFrame)
    EVT_BUTTON(ID_RECHERCHER, ServiceClientFrame::OnRechercher)
    EVT_BUTTON(ID_TRAITER_RECLAMATION, ServiceClientFrame::OnTraiterReclamation)
    EVT_BUTTON(ID_INITIER_RETOUR, ServiceClientFrame::OnInitierRetour)
    EVT_BUTTON(ID_VALIDER_RETOUR, ServiceClientFrame::OnValiderRetour)
    EVT_BUTTON(ID_REFUSER_RETOUR, ServiceClientFrame::OnRefuserRetour)
    EVT_BUTTON(ID_REMBOURSER, ServiceClientFrame::OnTraiterRemboursement)
    EVT_BUTTON(ID_CONTACTER, ServiceClientFrame::OnContacterClient)
wxEND_EVENT_TABLE()

ServiceClientFrame::ServiceClientFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Service Client - Gestion Reclamations & Retours",
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
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "SERVICE CLIENT");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(220, 53, 69));
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // Barre de recherche
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* searchLabel = new wxStaticText(panel, wxID_ANY, "Rechercher commande :");
    searchSizer->Add(searchLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    m_searchCommandeCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(350, 30));
    m_searchCommandeCtrl->SetHint("N° commande ou nom client...");
    searchSizer->Add(m_searchCommandeCtrl, 0, wxRIGHT, 10);

    wxButton* searchBtn = new wxButton(panel, ID_RECHERCHER, "Rechercher", wxDefaultPosition, wxSize(130, 30));
    searchSizer->Add(searchBtn, 0);

    mainSizer->Add(searchSizer, 0, wxALL | wxLEFT, 30);

    mainSizer->AddSpacer(10);

    // Notebook
    m_notebook = new wxNotebook(panel, wxID_ANY);

    CreateReclamationsPage();
    CreateRetoursPage();

    m_notebook->AddPage(m_reclamationsPage, "Reclamations");
    m_notebook->AddPage(m_retoursPage, "Retours");

    mainSizer->Add(m_notebook, 1, wxALL | wxEXPAND, 20);

    panel->SetSizer(mainSizer);

    Maximize(true);
}

ServiceClientFrame::~ServiceClientFrame()
{
}

void ServiceClientFrame::CreateReclamationsPage()
{
    m_reclamationsPage = new wxPanel(m_notebook, wxID_ANY);
    m_reclamationsPage->SetBackgroundColour(*wxWHITE);

    m_reclamationsList = new wxListCtrl(m_reclamationsPage, wxID_ANY,
                                        wxPoint(10, 10),
                                        wxSize(1080, 450),
                                        wxLC_REPORT | wxLC_SINGLE_SEL);

    m_reclamationsList->InsertColumn(0, "ID", wxLIST_FORMAT_CENTER, 50);
    m_reclamationsList->InsertColumn(1, "N° Commande", wxLIST_FORMAT_LEFT, 120);
    m_reclamationsList->InsertColumn(2, "Client", wxLIST_FORMAT_LEFT, 180);
    m_reclamationsList->InsertColumn(3, "Type", wxLIST_FORMAT_LEFT, 150);
    m_reclamationsList->InsertColumn(4, "Description", wxLIST_FORMAT_LEFT, 350);
    m_reclamationsList->InsertColumn(5, "Date", wxLIST_FORMAT_CENTER, 100);
    m_reclamationsList->InsertColumn(6, "Statut", wxLIST_FORMAT_CENTER, 130);

    PopulateReclamations();

    wxButton* traiterBtn = new wxButton(m_reclamationsPage, ID_TRAITER_RECLAMATION,
                                        "Traiter",
                                        wxPoint(10, 480),
                                        wxSize(120, 40));
    traiterBtn->SetBackgroundColour(wxColour(40, 167, 69));
    traiterBtn->SetForegroundColour(*wxWHITE);

    wxButton* contacterBtn = new wxButton(m_reclamationsPage, ID_CONTACTER,
                                          "Contacter client",
                                          wxPoint(140, 480),
                                          wxSize(150, 40));
    contacterBtn->SetBackgroundColour(wxColour(0, 123, 255));
    contacterBtn->SetForegroundColour(*wxWHITE);

    wxButton* initierRetourBtn = new wxButton(m_reclamationsPage, ID_INITIER_RETOUR,
                                              "Initier retour",
                                              wxPoint(300, 480),
                                              wxSize(140, 40));
}

void ServiceClientFrame::CreateRetoursPage()
{
    m_retoursPage = new wxPanel(m_notebook, wxID_ANY);
    m_retoursPage->SetBackgroundColour(*wxWHITE);

    m_retoursList = new wxListCtrl(m_retoursPage, wxID_ANY,
                                   wxPoint(10, 10),
                                   wxSize(1080, 450),
                                   wxLC_REPORT | wxLC_SINGLE_SEL);

    m_retoursList->InsertColumn(0, "N° Retour", wxLIST_FORMAT_LEFT, 120);
    m_retoursList->InsertColumn(1, "N° Commande", wxLIST_FORMAT_LEFT, 120);
    m_retoursList->InsertColumn(2, "Client", wxLIST_FORMAT_LEFT, 180);
    m_retoursList->InsertColumn(3, "Produit", wxLIST_FORMAT_LEFT, 250);
    m_retoursList->InsertColumn(4, "Motif", wxLIST_FORMAT_LEFT, 200);
    m_retoursList->InsertColumn(5, "Montant", wxLIST_FORMAT_RIGHT, 100);
    m_retoursList->InsertColumn(6, "Statut", wxLIST_FORMAT_CENTER, 110);

    PopulateRetours();

    wxButton* validerBtn = new wxButton(m_retoursPage, ID_VALIDER_RETOUR,
                                        "Valider retour",
                                        wxPoint(10, 480),
                                        wxSize(130, 40));
    validerBtn->SetBackgroundColour(wxColour(40, 167, 69));
    validerBtn->SetForegroundColour(*wxWHITE);

    wxButton* refuserBtn = new wxButton(m_retoursPage, ID_REFUSER_RETOUR,
                                        "Refuser",
                                        wxPoint(150, 480),
                                        wxSize(100, 40));
    refuserBtn->SetBackgroundColour(wxColour(220, 53, 69));
    refuserBtn->SetForegroundColour(*wxWHITE);

    wxButton* rembourserBtn = new wxButton(m_retoursPage, ID_REMBOURSER,
                                           "Rembourser",
                                           wxPoint(260, 480),
                                           wxSize(130, 40));
    rembourserBtn->SetBackgroundColour(wxColour(255, 193, 7));
}



void ServiceClientFrame::PopulateReclamations()
{
    m_reclamationsList->DeleteAllItems();

    // CHARGER DEPUIS LA BASE DE DONNÉES
    std::vector<Reclamation> reclamations = DatabaseManager::GetInstance().GetAllReclamations();

    for(size_t i = 0; i < reclamations.size(); i++)
    {
        const Reclamation& r = reclamations[i];

        long index = m_reclamationsList->InsertItem(i, wxString::Format("%d", r.id));
        m_reclamationsList->SetItem(index, 1, "CMD-" + wxString::Format("%d", r.id_commande));
        m_reclamationsList->SetItem(index, 2, r.client);
        m_reclamationsList->SetItem(index, 3, r.type);
        m_reclamationsList->SetItem(index, 4, r.description);
        m_reclamationsList->SetItem(index, 5, r.date);
        m_reclamationsList->SetItem(index, 6, r.statut);

        if(r.statut == "Nouveau")
            m_reclamationsList->SetItemTextColour(index, wxColour(220, 53, 69));
        else if(r.statut == "En cours")
            m_reclamationsList->SetItemTextColour(index, wxColour(255, 193, 7));
        else
            m_reclamationsList->SetItemTextColour(index, wxColour(40, 167, 69));
    }
}




void ServiceClientFrame::PopulateRetours()
{
    struct Retour {
        const char* numRetour;
        const char* commande;
        const char* client;
        const char* produit;
        const char* motif;
        const char* montant;
        const char* statut;
    };

    Retour retours[] = {
        {"RET-001", "CMD-2026-045", "AHOUANSOU Olivier", "Smartphone Samsung",
         "Produit defectueux", "450000 F", "En attente"},
        {"RET-002", "CMD-2026-042", "EGOUDJOBI Peace", "T-shirt Nike",
         "Mauvaise taille", "15000 F", "Valide"},
        {"RET-003", "CMD-2026-039", "HOUNGUEVOU Blandine", "Ecouteurs Sony",
         "Ne correspond pas", "35000 F", "En attente"}
    };

    for(int i = 0; i < 3; i++)
    {
        long index = m_retoursList->InsertItem(i, retours[i].numRetour);
        m_retoursList->SetItem(index, 1, retours[i].commande);
        m_retoursList->SetItem(index, 2, retours[i].client);
        m_retoursList->SetItem(index, 3, retours[i].produit);
        m_retoursList->SetItem(index, 4, retours[i].motif);
        m_retoursList->SetItem(index, 5, retours[i].montant);
        m_retoursList->SetItem(index, 6, retours[i].statut);

        wxString statutStr(retours[i].statut);
        if(statutStr == "En attente")
            m_retoursList->SetItemTextColour(index, wxColour(255, 193, 7));
        else
            m_retoursList->SetItemTextColour(index, wxColour(40, 167, 69));
    }
}

void ServiceClientFrame::OnRechercher(wxCommandEvent& event)
{
    wxString recherche = m_searchCommandeCtrl->GetValue();

    if(recherche.IsEmpty())
    {
        wxMessageBox("Veuillez entrer un numero de commande ou nom de client.",
                     "Recherche",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxMessageBox("Recherche de : " + recherche + "\n\n"
                 "Commandes trouvees :\n"
                 "• CMD-2026-045 - AHOUANSOU Olivier - 520000 F\n"
                 "• CMD-2026-038 - Client associe - 85000 F",
                 "Resultats de recherche",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void ServiceClientFrame::OnTraiterReclamation(wxCommandEvent& event)
{
    long selected = m_reclamationsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une reclamation.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numCommande = m_reclamationsList->GetItemText(selected, 1);
    wxString client = m_reclamationsList->GetItemText(selected, 2);

    wxTextEntryDialog responseDialog(
        this,
        "Reponse au client " + client + " :\n"
        "Commande : " + numCommande,
        "Traiter la reclamation",
        "",
        wxOK | wxCANCEL | wxTE_MULTILINE
    );

    if(responseDialog.ShowModal() == wxID_OK)
    {
        wxString response = responseDialog.GetValue();

        m_reclamationsList->SetItem(selected, 6, "Traite");
        m_reclamationsList->SetItemTextColour(selected, wxColour(40, 167, 69));

        wxMessageBox("Reclamation traitee avec succes !\n\n"
                     "Reponse envoyee au client par email.",
                     "Succes",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void ServiceClientFrame::OnInitierRetour(wxCommandEvent& event)
{
    long selected = m_reclamationsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une reclamation.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numCommande = m_reclamationsList->GetItemText(selected, 1);

    wxArrayString motifs;
    motifs.Add("Produit defectueux");
    motifs.Add("Mauvais article recu");
    motifs.Add("Taille incorrecte");
    motifs.Add("Produit endommage");
    motifs.Add("Non conforme a la description");
    motifs.Add("Autre");

    wxSingleChoiceDialog motifDialog(
        this,
        "Selectionnez le motif du retour :",
        "Initier un retour - " + numCommande,
        motifs
    );

    if(motifDialog.ShowModal() == wxID_OK)
    {
        wxString motif = motifDialog.GetStringSelection();

        wxString message;
        message << "Retour initie avec succes !\n\n"
                << "N° Retour : RET-" << wxString::Format("%03d", (int)(m_retoursList->GetItemCount() + 1)) << "\n"
                << "Commande : " << numCommande << "\n"
                << "Motif : " << motif << "\n\n"
                << "Le client et le livreur ont ete notifies.";

        wxMessageBox(message,
                     "Retour cree",
                     wxOK | wxICON_INFORMATION,
                     this);

        m_notebook->SetSelection(1);
    }
}

void ServiceClientFrame::OnValiderRetour(wxCommandEvent& event)
{
    long selected = m_retoursList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un retour.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numRetour = m_retoursList->GetItemText(selected, 0);
    wxString montant = m_retoursList->GetItemText(selected, 5);

    int response = wxMessageBox(
        "Valider le retour " + numRetour + " ?\n\n"
        "Montant du remboursement : " + montant + "\n\n"
        "Le remboursement sera traite sous 3-5 jours ouvres.",
        "Validation du retour",
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        m_retoursList->SetItem(selected, 6, "Valide");
        m_retoursList->SetItemTextColour(selected, wxColour(40, 167, 69));

        wxMessageBox("Retour valide avec succes !\n\n"
                     "Le client recevra une confirmation par email.\n"
                     "Le remboursement sera effectue automatiquement.",
                     "Validation reussie",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void ServiceClientFrame::OnRefuserRetour(wxCommandEvent& event)
{
    long selected = m_retoursList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un retour.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numRetour = m_retoursList->GetItemText(selected, 0);

    wxTextEntryDialog justifDialog(
        this,
        "Indiquez la raison du refus du retour " + numRetour + " :",
        "Refuser le retour",
        ""
    );

    if(justifDialog.ShowModal() == wxID_OK)
    {
        wxString justification = justifDialog.GetValue();

        if(!justification.IsEmpty())
        {
            m_retoursList->SetItem(selected, 6, "Refuse");
            m_retoursList->SetItemTextColour(selected, wxColour(220, 53, 69));

            wxMessageBox("Retour refuse.\n\n"
                         "Justification : " + justification + "\n\n"
                         "Le client sera notifie par email.",
                         "Refus enregistre",
                         wxOK | wxICON_INFORMATION,
                         this);
        }
    }
}

void ServiceClientFrame::OnTraiterRemboursement(wxCommandEvent& event)
{
    long selected = m_retoursList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un retour.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString numRetour = m_retoursList->GetItemText(selected, 0);
    wxString montant = m_retoursList->GetItemText(selected, 5);
    wxString statut = m_retoursList->GetItemText(selected, 6);

    if(statut != "Valide")
    {
        wxMessageBox("Le retour doit d'abord etre valide avant remboursement.",
                     "Impossible",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    int response = wxMessageBox(
        "Traiter le remboursement de " + montant + " ?\n\n"
        "N° Retour : " + numRetour + "\n\n"
        "Le montant sera credite sur le compte du client.",
        "Confirmation remboursement",
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        m_retoursList->SetItem(selected, 6, "Rembourse");
        m_retoursList->SetItemTextColour(selected, wxColour(0, 128, 0));

        wxMessageBox("Remboursement traite avec succes !\n\n"
                     "Montant : " + montant + "\n"
                     "Date : 19/01/2026\n"
                     "Reference : REMB-" + numRetour + "\n\n"
                     "Le client recevra les fonds sous 3-5 jours.",
                     "Remboursement effectue",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void ServiceClientFrame::OnContacterClient(wxCommandEvent& event)
{
    long selected = m_reclamationsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner une reclamation.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString client = m_reclamationsList->GetItemText(selected, 2);

    wxArrayString methodes;
    methodes.Add("Email");
    methodes.Add("Telephone");
    methodes.Add("SMS");
    methodes.Add("WhatsApp");

    wxSingleChoiceDialog methodeDialog(
        this,
        "Choisissez le mode de contact pour " + client + " :",
        "Contacter le client",
        methodes
    );

    if(methodeDialog.ShowModal() == wxID_OK)
    {
        wxString methode = methodeDialog.GetStringSelection();

        wxString email = client.Lower();
        email.Replace(" ", ".");

        wxString message;
        message << "Contact client via " << methode << "\n\n"
                << "Client : " << client << "\n"
                << "Email : " << email << "@email.com\n"
                << "Telephone : +229 97 XX XX XX";

        wxMessageBox(message,
                     "Coordonnees",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}
