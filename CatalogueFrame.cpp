/***************************************************************
 * Name:      CatalogueFrame.cpp
 * Purpose:   Catalogue avec affichage en cartes modernes
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-31
 **************************************************************/

#include "CatalogueFrame.h"
#include "PanierDialog.h"
#include "DatabaseManager.h"
#include "CartManager.h"

wxBEGIN_EVENT_TABLE(CatalogueFrame, wxFrame)
    EVT_BUTTON(ID_BACK, CatalogueFrame::OnBack)
    EVT_BUTTON(ID_CART, CatalogueFrame::OnCart)
wxEND_EVENT_TABLE()

CatalogueFrame::CatalogueFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Catalogue Produits",
              wxDefaultPosition, wxSize(1400, 900))
{
    m_cardsPanel = NULL;  // ← IMPORTANT: Initialiser à NULL

    wxPanel* mainPanel = new wxPanel(this);
    mainPanel->SetBackgroundColour(wxColour(245, 245, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // HEADER
    wxPanel* headerPanel = new wxPanel(mainPanel);
    headerPanel->SetBackgroundColour(*wxWHITE);
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* titleText = new wxStaticText(headerPanel, wxID_ANY, "CATALOGUE PRODUITS");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(0, 102, 204));
    headerSizer->Add(titleText, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 30);

    // Badge panier
    m_cartBadge = new wxStaticText(headerPanel, wxID_ANY, "0",
                                   wxDefaultPosition, wxSize(30, 30));
    wxFont badgeFont = m_cartBadge->GetFont();
    badgeFont.SetPointSize(12);
    badgeFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_cartBadge->SetFont(badgeFont);
    m_cartBadge->SetForegroundColour(*wxWHITE);
    m_cartBadge->SetBackgroundColour(wxColour(220, 53, 69));
    headerSizer->Add(m_cartBadge, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    wxButton* cartBtn = new wxButton(headerPanel, ID_CART, "Mon Panier",
                                     wxDefaultPosition, wxSize(160, 45));
    cartBtn->SetBackgroundColour(wxColour(255, 140, 0));
    cartBtn->SetForegroundColour(*wxWHITE);
    wxFont cartBtnFont = cartBtn->GetFont();
    cartBtnFont.SetPointSize(11);
    cartBtnFont.SetWeight(wxFONTWEIGHT_BOLD);
    cartBtn->SetFont(cartBtnFont);
    headerSizer->Add(cartBtn, 0, wxRIGHT, 30);

    headerPanel->SetSizer(headerSizer);
    mainSizer->Add(headerPanel, 0, wxALL | wxEXPAND, 0);

    // ZONE SCROLLABLE POUR LES CARTES
    m_scrolledWindow = new wxScrolledWindow(mainPanel, wxID_ANY,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxVSCROLL);
    m_scrolledWindow->SetScrollRate(0, 20);
    m_scrolledWindow->SetBackgroundColour(wxColour(245, 245, 250));

    mainSizer->Add(m_scrolledWindow, 1, wxALL | wxEXPAND, 0);

    // STATUS BAR
    m_statusText = new wxStaticText(mainPanel, wxID_ANY, "");
    m_statusText->SetForegroundColour(wxColour(100, 100, 100));
    mainSizer->Add(m_statusText, 0, wxALL | wxCENTER, 10);

    // BOUTON RETOUR
    wxButton* backBtn = new wxButton(mainPanel, ID_BACK, "Retour au menu",
                                     wxDefaultPosition, wxSize(180, 45));
    backBtn->SetBackgroundColour(wxColour(108, 117, 125));
    backBtn->SetForegroundColour(*wxWHITE);
    mainSizer->Add(backBtn, 0, wxALL | wxCENTER, 15);

    mainPanel->SetSizer(mainSizer);

    LoadProductCards();
    UpdateCartBadge();

    Centre();
}

CatalogueFrame::~CatalogueFrame()
{
    // Destruction automatique gérée par wxWidgets
}

wxPanel* CatalogueFrame::CreateProductCard(wxWindow* parent, const Product& product)
{
    // Calculer stock disponible
    int stockDisponible = product.stock;
    if(CartManager::GetInstance().HasProduct(product.id))
    {
        std::vector<CartItem> items = CartManager::GetInstance().GetAllItems();
        for(const auto& item : items)
        {
            if(item.id_produit == product.id)
            {
                stockDisponible -= item.quantite;
                break;
            }
        }
    }

    // Panel de la carte
    wxPanel* card = new wxPanel(parent, wxID_ANY,
                               wxDefaultPosition, wxSize(320, 380));
    card->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* cardSizer = new wxBoxSizer(wxVERTICAL);

    // ZONE IMAGE (placeholder coloré)
    wxPanel* imagePanel = new wxPanel(card, wxID_ANY,
                                     wxDefaultPosition, wxSize(300, 200));

    // Couleur selon catégorie
    wxColour imageColor;
    if(product.categorie == "Electronique")
        imageColor = wxColour(100, 149, 237);
    else if(product.categorie == "Vetements")
        imageColor = wxColour(255, 182, 193);
    else if(product.categorie == "Alimentation")
        imageColor = wxColour(144, 238, 144);
    else if(product.categorie == "Maison")
        imageColor = wxColour(255, 218, 185);
    else if(product.categorie == "Sports")
        imageColor = wxColour(255, 165, 0);
    else
        imageColor = wxColour(220, 220, 220);

    imagePanel->SetBackgroundColour(imageColor);

    wxBoxSizer* imageSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* imageText = new wxStaticText(imagePanel, wxID_ANY, product.categorie);
    wxFont imageFont = imageText->GetFont();
    imageFont.SetPointSize(14);
    imageFont.SetWeight(wxFONTWEIGHT_BOLD);
    imageText->SetFont(imageFont);
    imageText->SetForegroundColour(*wxWHITE);
    imageSizer->AddStretchSpacer();
    imageSizer->Add(imageText, 0, wxALL | wxCENTER, 10);
    imageSizer->AddStretchSpacer();
    imagePanel->SetSizer(imageSizer);

    cardSizer->Add(imagePanel, 0, wxALL | wxEXPAND, 10);

    // NOM DU PRODUIT
    wxStaticText* nameText = new wxStaticText(card, wxID_ANY, product.nom,
                                              wxDefaultPosition, wxSize(300, -1));
    wxFont nameFont = nameText->GetFont();
    nameFont.SetPointSize(11);
    nameFont.SetWeight(wxFONTWEIGHT_BOLD);
    nameText->SetFont(nameFont);
    nameText->Wrap(280);
    cardSizer->Add(nameText, 0, wxLEFT | wxRIGHT, 15);

    cardSizer->AddSpacer(5);

    // PRIX
    wxStaticText* priceText = new wxStaticText(card, wxID_ANY,
                                               wxString::Format("%.0f F CFA", product.prix));
    wxFont priceFont = priceText->GetFont();
    priceFont.SetPointSize(16);
    priceFont.SetWeight(wxFONTWEIGHT_BOLD);
    priceText->SetFont(priceFont);
    priceText->SetForegroundColour(wxColour(220, 53, 69));
    cardSizer->Add(priceText, 0, wxLEFT | wxRIGHT, 15);

    cardSizer->AddSpacer(5);

    // STOCK
    wxString stockText;
    wxColour stockColor;
    if(stockDisponible > 10)
    {
        stockText = wxString::Format("En stock (%d disponibles)", stockDisponible);
        stockColor = wxColour(40, 167, 69);
    }
    else if(stockDisponible > 0)
    {
        stockText = wxString::Format("Stock limite (%d restants)", stockDisponible);
        stockColor = wxColour(255, 140, 0);
    }
    else
    {
        stockText = "Rupture de stock";
        stockColor = wxColour(220, 53, 69);
    }

    wxStaticText* stockLabel = new wxStaticText(card, wxID_ANY, stockText);
    wxFont stockFont = stockLabel->GetFont();
    stockFont.SetPointSize(9);
    stockLabel->SetFont(stockFont);
    stockLabel->SetForegroundColour(stockColor);
    cardSizer->Add(stockLabel, 0, wxLEFT | wxRIGHT, 15);

    cardSizer->AddStretchSpacer();

    // BOUTON AJOUTER AU PANIER
    wxButton* addBtn = new wxButton(card, ID_ADD_TO_CART_BASE + product.id,
                                    "Ajouter au panier",
                                    wxDefaultPosition, wxSize(280, 40));

    if(stockDisponible > 0)
    {
        addBtn->SetBackgroundColour(wxColour(40, 167, 69));
        addBtn->SetForegroundColour(*wxWHITE);
    }
    else
    {
        addBtn->SetBackgroundColour(wxColour(200, 200, 200));
        addBtn->SetForegroundColour(wxColour(120, 120, 120));
        addBtn->Enable(false);
    }

    wxFont btnFont = addBtn->GetFont();
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);
    addBtn->SetFont(btnFont);

    // ← CORRECTION ICI: Capturer product par COPIE
    addBtn->Bind(wxEVT_BUTTON, [this, product](wxCommandEvent&) {
        int stockDisponible = product.stock;
        if(CartManager::GetInstance().HasProduct(product.id))
        {
            std::vector<CartItem> items = CartManager::GetInstance().GetAllItems();
            for(const auto& item : items)
            {
                if(item.id_produit == product.id)
                {
                    stockDisponible -= item.quantite;
                    break;
                }
            }
        }

        if(stockDisponible <= 0)
        {
            wxMessageBox("Ce produit n'est plus en stock.",
                         "Stock epuise",
                         wxOK | wxICON_WARNING,
                         this);
            return;
        }

        CartManager::GetInstance().AddProduct(product.id, product.nom, product.prix, 1);

        wxMessageBox(wxString::Format("%s ajoute au panier !\n\nPrix : %.0f F CFA",
                                       product.nom, product.prix),
                     "Ajoute au panier",
                     wxOK | wxICON_INFORMATION,
                     this);

        UpdateCartBadge();
        LoadProductCards();
    });

    cardSizer->Add(addBtn, 0, wxALL | wxCENTER, 10);

    card->SetSizer(cardSizer);

    return card;
}

void CatalogueFrame::LoadProductCards()
{
    // ← CORRECTION ICI: Vérifier si m_cardsPanel existe avant de détruire
    if(m_cardsPanel && m_scrolledWindow)
    {
        m_scrolledWindow->DestroyChildren();  // Détruire tous les enfants
        m_cardsPanel = NULL;
    }

    // Créer nouveau panel
    m_cardsPanel = new wxPanel(m_scrolledWindow);
    m_cardsPanel->SetBackgroundColour(wxColour(245, 245, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Récupérer les produits
    std::vector<Product> products = DatabaseManager::GetInstance().GetAllProducts();

    if(products.empty())
    {
        m_statusText->SetLabel("Aucun produit trouve");
        return;
    }

    // Créer grille de cartes (4 par ligne)
    wxGridSizer* gridSizer = new wxGridSizer(4, 20, 20);

    for(const auto& product : products)
    {
        wxPanel* card = CreateProductCard(m_cardsPanel, product);
        gridSizer->Add(card, 0, wxALL, 0);
    }

    mainSizer->Add(gridSizer, 0, wxALL, 30);

    m_cardsPanel->SetSizer(mainSizer);
    m_cardsPanel->Layout();

    // Configurer le scrolled window
    wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);
    scrollSizer->Add(m_cardsPanel, 0, wxEXPAND);
    m_scrolledWindow->SetSizer(scrollSizer);
    m_scrolledWindow->FitInside();
    m_scrolledWindow->Layout();

    m_statusText->SetLabel(wxString::Format("%zu produits disponibles", products.size()));
}

void CatalogueFrame::UpdateCartBadge()
{
    int itemCount = CartManager::GetInstance().GetItemCount();
    m_cartBadge->SetLabel(wxString::Format("%d", itemCount));
}

void CatalogueFrame::OnBack(wxCommandEvent& event)
{
    Close(true);  // ← Forcer la fermeture
}

void CatalogueFrame::OnCart(wxCommandEvent& event)
{
    PanierDialog dlg(this);
    dlg.ShowModal();

    UpdateCartBadge();
    LoadProductCards();
}

void CatalogueFrame::OnAddToCart(wxCommandEvent& event)
{
    // Géré dans les lambdas des boutons individuels
}
