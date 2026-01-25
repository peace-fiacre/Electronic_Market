#include "CatalogueFrame.h"
#include "PanierDialog.h"

wxBEGIN_EVENT_TABLE(CatalogueFrame, wxFrame)
    EVT_BUTTON(ID_SEARCH, CatalogueFrame::OnSearch)
    EVT_BUTTON(ID_VIEW_CART, CatalogueFrame::OnViewCart)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, CatalogueFrame::OnProductDoubleClick)
wxEND_EVENT_TABLE()

CatalogueFrame::CatalogueFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Catalogue Produits",
              wxDefaultPosition, wxSize(1200, 800)),
      m_cartItemCount(0)
{
    m_panel = new wxPanel(this, wxID_ANY);
    m_panel->SetBackgroundColour(*wxWHITE);

    CreateMenuBar();

    wxStaticText* searchLabel = new wxStaticText(m_panel, wxID_ANY,
                                                  "Rechercher :",
                                                  wxPoint(30, 25));
    m_searchCtrl = new wxTextCtrl(m_panel, wxID_ANY,
                                  "",
                                  wxPoint(120, 20),
                                  wxSize(400, 35));
    m_searchCtrl->SetHint("Nom du produit...");

    m_searchBtn = new wxButton(m_panel, ID_SEARCH,
                               "Rechercher",
                               wxPoint(530, 20),
                               wxSize(120, 35));

    wxStaticText* catLabel = new wxStaticText(m_panel, wxID_ANY,
                                              "Catégorie :",
                                              wxPoint(680, 25));
    wxArrayString categories;
    categories.Add("Toutes");
    categories.Add("Électronique");
    categories.Add("Vêtements");
    categories.Add("Alimentation");
    categories.Add("Maison");
    categories.Add("Sports");

    m_categoryChoice = new wxChoice(m_panel, wxID_ANY,
                                    wxPoint(760, 20),
                                    wxSize(200, 35),
                                    categories);
    m_categoryChoice->SetSelection(0);

    m_viewCartBtn = new wxButton(m_panel, ID_VIEW_CART,
                                 "Panier (0)",
                                 wxPoint(1000, 20),
                                 wxSize(150, 35));
    m_viewCartBtn->SetBackgroundColour(wxColour(255, 140, 0));
    m_viewCartBtn->SetForegroundColour(*wxWHITE);

    m_productList = new wxListCtrl(m_panel, wxID_ANY,
                                   wxPoint(30, 80),
                                   wxSize(1120, 600),
                                   wxLC_REPORT | wxLC_SINGLE_SEL);

    m_productList->InsertColumn(0, "ID", wxLIST_FORMAT_CENTER, 50);
    m_productList->InsertColumn(1, "Produit", wxLIST_FORMAT_LEFT, 400);
    m_productList->InsertColumn(2, "Catégorie", wxLIST_FORMAT_LEFT, 150);
    m_productList->InsertColumn(3, "Prix", wxLIST_FORMAT_RIGHT, 120);
    m_productList->InsertColumn(4, "Stock", wxLIST_FORMAT_CENTER, 100);
    m_productList->InsertColumn(5, "Disponibilité", wxLIST_FORMAT_CENTER, 150);

    PopulateProducts();

    m_statusBar = CreateStatusBar();
    m_statusBar->SetStatusText("24 produits affichés");

    Maximize(true);
}

CatalogueFrame::~CatalogueFrame()
{
}

void CatalogueFrame::CreateMenuBar()
{
    m_menuBar = new wxMenuBar;

    wxMenu* menuAccueil = new wxMenu;
    wxMenuItem* itemAccueil = menuAccueil->Append(wxID_ANY, "Retour menu principal");
    m_menuBar->Append(menuAccueil, "Accueil");

    wxMenu* menuCompte = new wxMenu;
    wxMenuItem* itemProfil = menuCompte->Append(wxID_ANY, "Mon profil");
    m_menuBar->Append(menuCompte, "Mon compte");

    wxMenu* menuPanier = new wxMenu;
    wxMenuItem* itemVoirPanier = menuPanier->Append(wxID_ANY, "Voir mon panier");
    m_menuBar->Append(menuPanier, "Panier");

    wxMenu* menuReclamation = new wxMenu;
    wxMenuItem* itemReclamation = menuReclamation->Append(wxID_ANY, "Faire une reclamation");
    wxMenuItem* itemSuiviCommande = menuReclamation->Append(wxID_ANY, "Suivre ma commande");
    m_menuBar->Append(menuReclamation, "Support");

    SetMenuBar(m_menuBar);

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        this->Close();
    }, itemAccueil->GetId());

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        wxMessageBox("Gestion du profil utilisateur à implémenter",
                     "Mon profil",
                     wxOK | wxICON_INFORMATION,
                     this);
    }, itemProfil->GetId());

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        PanierDialog* dlg = new PanierDialog(this);
        dlg->ShowModal();
        dlg->Destroy();
    }, itemVoirPanier->GetId());

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        wxTextEntryDialog reclamDialog(
            this,
            "Décrivez votre réclamation :",
            "Faire une réclamation",
            "",
            wxOK | wxCANCEL | wxTE_MULTILINE
        );

        if(reclamDialog.ShowModal() == wxID_OK)
        {
            wxString reclamation = reclamDialog.GetValue();
            if(!reclamation.IsEmpty())
            {
                wxMessageBox("Réclamation enregistrée avec succès !\n\n"
                           "Numéro de réclamation : REC-" + wxString::Format("%03d", wxGetLocalTime() % 1000) + "\n\n"
                           "Notre service client vous contactera sous 24-48h.",
                           "Réclamation envoyée",
                           wxOK | wxICON_INFORMATION,
                           this);
            }
        }
    }, itemReclamation->GetId());

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        wxTextEntryDialog commandeDialog(
            this,
            "Entrez votre numéro de commande :",
            "Suivre ma commande",
            ""
        );

        if(commandeDialog.ShowModal() == wxID_OK)
        {
            wxString numCommande = commandeDialog.GetValue();
            if(!numCommande.IsEmpty())
            {
                wxMessageBox("Suivi de commande : " + numCommande + "\n\n"
                           "Statut : En cours de livraison\n"
                           "N° de suivi : TRK-2026-123\n"
                           "Livraison prévue : 20/01/2026\n"
                           "Transporteur : DHL Express",
                           "Statut de la commande",
                           wxOK | wxICON_INFORMATION,
                           this);
            }
        }
    }, itemSuiviCommande->GetId());
}

void CatalogueFrame::PopulateProducts()
{
    struct Product {
        const char* name;
        const char* category;
        const char* price;
        const char* stock;
        const char* availability;
    };

    Product products[] = {
        {"Smartphone Samsung S23", "Électronique", "450000", "15", "En stock"},
        {"MacBook Pro 14", "Électronique", "1200000", "8", "En stock"},
        {"Écouteurs Sony", "Électronique", "35000", "45", "En stock"},
        {"T-shirt Nike", "Vêtements", "15000", "100", "En stock"},
        {"Jean Levi's", "Vêtements", "25000", "60", "En stock"},
        {"Chaussures Adidas", "Vêtements", "45000", "30", "En stock"},
        {"Riz 25kg", "Alimentation", "18000", "200", "En stock"},
        {"Huile 5L", "Alimentation", "8000", "150", "En stock"},
        {"Pack eau 12x1.5L", "Alimentation", "3500", "300", "En stock"},
        {"Canapé 3 places", "Maison", "185000", "12", "En stock"}
    };

    for(int i = 0; i < 10; i++)
    {
        long index = m_productList->InsertItem(i, wxString::Format("%d", i + 1));
        m_productList->SetItem(index, 1, products[i].name);
        m_productList->SetItem(index, 2, products[i].category);
        wxString priceStr;
        priceStr << products[i].price << " F";
        m_productList->SetItem(index, 3, priceStr);
        m_productList->SetItem(index, 4, products[i].stock);
        m_productList->SetItem(index, 5, products[i].availability);

        m_productList->SetItemTextColour(index, wxColour(0, 128, 0));
    }
}

void CatalogueFrame::OnSearch(wxCommandEvent& event)
{
    wxString searchText = m_searchCtrl->GetValue();

    if(searchText.IsEmpty())
    {
        wxMessageBox("Veuillez entrer un terme de recherche.",
                     "Recherche",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString message;
    message << "Recherche de : \"" << searchText << "\"\n\n"
            << "Fonctionnalité de recherche active !\n"
            << "Les résultats s'afficheraient ici.";

    m_statusBar->SetStatusText("Recherche : " + searchText);

    wxMessageBox(message,
                 "Résultats de recherche",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void CatalogueFrame::OnViewCart(wxCommandEvent& event)
{
    PanierDialog* dlg = new PanierDialog(this);
    dlg->ShowModal();
    dlg->Destroy();
}

void CatalogueFrame::OnProductDoubleClick(wxListEvent& event)
{
    long index = event.GetIndex();
    wxString productName = m_productList->GetItemText(index, 1);
    wxString productPrice = m_productList->GetItemText(index, 3);
    wxString productStock = m_productList->GetItemText(index, 4);

    wxString details;
    details << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
            << "DÉTAILS DU PRODUIT\n"
            << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
            << "Nom : " << productName << "\n"
            << "Prix : " << productPrice << "\n"
            << "Stock disponible : " << productStock << " unités\n\n"
            << "Description : Produit de haute qualité\n"
            << "Garantie : 1 an\n"
            << "Livraison : 24-48h\n\n"
            << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";

    int response = wxMessageBox(details + "\n\nAjouter au panier ?",
                                "Détails - " + productName,
                                wxYES_NO | wxICON_QUESTION,
                                this);

    if(response == wxYES)
    {
        m_cartItemCount++;
        m_viewCartBtn->SetLabel(wxString::Format("Panier (%d)", m_cartItemCount));

        wxMessageBox("Produit ajouté au panier avec succès !",
                     "Ajout au panier",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}
