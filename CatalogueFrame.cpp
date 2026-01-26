/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#include "CatalogueFrame.h"
#include "PanierDialog.h"
#include "DatabaseManager.h"

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
                                              "Categorie :",
                                              wxPoint(680, 25));
    wxArrayString categories;
    categories.Add("Toutes");
    categories.Add("Electronique");
    categories.Add("Vetements");
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
    m_productList->InsertColumn(2, "Categorie", wxLIST_FORMAT_LEFT, 150);
    m_productList->InsertColumn(3, "Prix", wxLIST_FORMAT_RIGHT, 120);
    m_productList->InsertColumn(4, "Stock", wxLIST_FORMAT_CENTER, 100);
    m_productList->InsertColumn(5, "Disponibilite", wxLIST_FORMAT_CENTER, 150);

    PopulateProducts();

    m_statusBar = CreateStatusBar();
    m_statusBar->SetStatusText("Chargement des produits...");

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
        wxMessageBox("Gestion du profil utilisateur a implementer",
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
        wxTextEntryDialog commandeDlg(
            this,
            "Entrez le numero de votre commande :",
            "Numero de commande",
            ""
        );

        if(commandeDlg.ShowModal() == wxID_OK)
        {
            wxString numCmd = commandeDlg.GetValue();
            long id_commande;

            if(!numCmd.ToLong(&id_commande))
            {
                wxMessageBox("Numero de commande invalide.",
                             "Erreur",
                             wxOK | wxICON_ERROR,
                             this);
                return;
            }

            wxTextEntryDialog reclamDialog(
                this,
                "Decrivez votre reclamation :",
                "Faire une reclamation - CMD-" + numCmd,
                "",
                wxOK | wxCANCEL | wxTE_MULTILINE
            );

            if(reclamDialog.ShowModal() == wxID_OK)
            {
                wxString reclamation = reclamDialog.GetValue();

                if(!reclamation.IsEmpty())
                {
                    // ENREGISTRER EN BASE DE DONNÉES
                    int id_reclam = DatabaseManager::GetInstance().CreateReclamation(
                        (int)id_commande,
                        "Client Connecte",
                        "Reclamation generale",
                        reclamation
                    );

                    if(id_reclam > 0)
                    {
                        wxMessageBox("Reclamation enregistree avec succes !\n\n"
                                   "Numero de reclamation : REC-" + wxString::Format("%d", id_reclam) + "\n\n"
                                   "Votre reclamation a ete enregistree en base de donnees.\n"
                                   "Notre service client vous contactera sous 24-48h.",
                                   "Reclamation envoyee",
                                   wxOK | wxICON_INFORMATION,
                                   this);
                    }
                    else
                    {
                        wxMessageBox("Erreur lors de l'enregistrement de la reclamation.",
                                     "Erreur",
                                     wxOK | wxICON_ERROR,
                                     this);
                    }
                }
            }
        }
    }, itemReclamation->GetId());

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        wxTextEntryDialog commandeDialog(
            this,
            "Entrez votre numero de commande :",
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
                           "Livraison prevue : 20/01/2026\n"
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
    m_productList->DeleteAllItems();

    try
    {
        // CHARGER DEPUIS LA BASE DE DONNÉES
        m_products = DatabaseManager::GetInstance().GetAllProducts();

        wxLogMessage("Chargement de %zu produits", m_products.size());

        for(size_t i = 0; i < m_products.size(); i++)
        {
            const Product& p = m_products[i];

            long index = m_productList->InsertItem(i, wxString::Format("%d", p.id));
            m_productList->SetItem(index, 1, p.nom);
            m_productList->SetItem(index, 2, p.categorie);
            m_productList->SetItem(index, 3, wxString::Format("%.0f F", p.prix));
            m_productList->SetItem(index, 4, wxString::Format("%d", p.stock));
            m_productList->SetItem(index, 5, p.stock > 0 ? "En stock" : "Rupture");

            if(p.stock > 0)
                m_productList->SetItemTextColour(index, wxColour(0, 128, 0));
            else
                m_productList->SetItemTextColour(index, wxColour(200, 0, 0));
        }

        m_statusBar->SetStatusText(wxString::Format("%zu produits affiches", m_products.size()));
    }
    catch(const std::exception& e)
    {
        wxLogError("Erreur lors du chargement des produits: %s", e.what());
        m_statusBar->SetStatusText("Erreur de chargement");

        wxMessageBox("Erreur lors du chargement des produits depuis la base de donnees.",
                     "Erreur",
                     wxOK | wxICON_ERROR,
                     this);
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
            << "Fonctionnalite de recherche active !\n"
            << "Les resultats s'afficheraient ici.";

    m_statusBar->SetStatusText("Recherche : " + searchText);

    wxMessageBox(message,
                 "Resultats de recherche",
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

    if(index < 0 || index >= (long)m_products.size())
        return;

    const Product& product = m_products[index];

    wxString details;
    details << "================================\n"
            << "DETAILS DU PRODUIT\n"
            << "================================\n\n"
            << "Nom : " << product.nom << "\n"
            << "Categorie : " << product.categorie << "\n"
            << "Prix : " << wxString::Format("%.0f F", product.prix) << "\n"
            << "Stock disponible : " << product.stock << " unites\n\n"
            << "Description :\n" << product.description << "\n\n"
            << "================================";

    if(product.stock <= 0)
    {
        wxMessageBox(details + "\n\nProduit en rupture de stock.",
                     "Details - " + product.nom,
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxTextEntryDialog quantityDlg(
        this,
        details + "\n\nQuantite a ajouter au panier :",
        "Ajouter au panier - " + product.nom,
        "1"
    );

    if(quantityDlg.ShowModal() == wxID_OK)
    {
        wxString qtyStr = quantityDlg.GetValue();
        long quantity;

        if(!qtyStr.ToLong(&quantity) || quantity <= 0)
        {
            wxMessageBox("Quantite invalide.",
                         "Erreur",
                         wxOK | wxICON_ERROR,
                         this);
            return;
        }

        if(quantity > product.stock)
        {
            wxMessageBox(wxString::Format("Stock insuffisant !\n\nStock disponible : %d", product.stock),
                         "Stock insuffisant",
                         wxOK | wxICON_WARNING,
                         this);
            return;
        }

        m_cartItemCount += quantity;
        m_viewCartBtn->SetLabel(wxString::Format("Panier (%d)", m_cartItemCount));

        wxMessageBox(wxString::Format("Produit ajoute au panier !\n\n"
                                      "Produit : %s\n"
                                      "Quantite : %ld\n"
                                      "Prix unitaire : %.0f F\n"
                                      "Sous-total : %.0f F",
                                      product.nom, quantity, product.prix, product.prix * quantity),
                     "Ajout au panier",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}
