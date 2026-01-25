#include "AdminProductsFrame.h"

wxBEGIN_EVENT_TABLE(AdminProductsFrame, wxFrame)
    EVT_BUTTON(ID_ADD, AdminProductsFrame::OnAdd)
    EVT_BUTTON(ID_EDIT, AdminProductsFrame::OnEdit)
    EVT_BUTTON(ID_DELETE, AdminProductsFrame::OnDelete)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, AdminProductsFrame::OnProductDoubleClick)
wxEND_EVENT_TABLE()

AdminProductsFrame::AdminProductsFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, wxT("Administration - Gestion Produits"),
              wxDefaultPosition, wxSize(1200, 800))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(*wxWHITE);

    // Menu Bar
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* menuAccueil = new wxMenu;
    wxMenuItem* itemAccueil = menuAccueil->Append(wxID_ANY, wxT("Retour menu principal"));
    menuBar->Append(menuAccueil, wxT("Accueil"));
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        this->Close();
    }, itemAccueil->GetId());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Titre
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, wxT("GESTION DES PRODUITS"));
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // Info
    wxStaticText* infoText = new wxStaticText(panel, wxID_ANY,
                                              wxT("Double-cliquez sur un produit pour le modifier"));
    infoText->SetForegroundColour(wxColour(100, 100, 100));
    mainSizer->Add(infoText, 0, wxALL | wxCENTER, 5);

    // Liste des produits
    m_productsList = new wxListCtrl(panel, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxLC_REPORT | wxLC_SINGLE_SEL);

    m_productsList->InsertColumn(0, wxT("ID"), wxLIST_FORMAT_CENTER, 50);
    m_productsList->InsertColumn(1, wxT("Nom"), wxLIST_FORMAT_LEFT, 300);
    m_productsList->InsertColumn(2, wxT("Catégorie"), wxLIST_FORMAT_LEFT, 150);
    m_productsList->InsertColumn(3, wxT("Prix"), wxLIST_FORMAT_RIGHT, 120);
    m_productsList->InsertColumn(4, wxT("Stock"), wxLIST_FORMAT_CENTER, 100);
    m_productsList->InsertColumn(5, wxT("Statut"), wxLIST_FORMAT_CENTER, 100);
    m_productsList->InsertColumn(6, wxT("Description"), wxLIST_FORMAT_LEFT, 250);

    mainSizer->Add(m_productsList, 1, wxALL | wxEXPAND, 20);

    // Boutons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* addBtn = new wxButton(panel, ID_ADD,
                                    wxT("Ajouter Produit"),
                                    wxDefaultPosition, wxSize(180, 45));
    addBtn->SetBackgroundColour(wxColour(40, 167, 69));
    addBtn->SetForegroundColour(*wxWHITE);
    wxFont btnFont = addBtn->GetFont();
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);
    addBtn->SetFont(btnFont);
    btnSizer->Add(addBtn, 0, wxRIGHT, 10);

    wxButton* editBtn = new wxButton(panel, ID_EDIT,
                                     wxT("Modifier Produit"),
                                     wxDefaultPosition, wxSize(180, 45));
    editBtn->SetBackgroundColour(wxColour(0, 123, 255));
    editBtn->SetForegroundColour(*wxWHITE);
    editBtn->SetFont(btnFont);
    btnSizer->Add(editBtn, 0, wxRIGHT, 10);

    wxButton* deleteBtn = new wxButton(panel, ID_DELETE,
                                       wxT("Supprimer Produit"),
                                       wxDefaultPosition, wxSize(180, 45));
    deleteBtn->SetBackgroundColour(wxColour(220, 53, 69));
    deleteBtn->SetForegroundColour(*wxWHITE);
    deleteBtn->SetFont(btnFont);
    btnSizer->Add(deleteBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxLEFT, 20);

    panel->SetSizer(mainSizer);

    InitializeProducts();
    RefreshProductsList();

    Maximize(true);
}

AdminProductsFrame::~AdminProductsFrame()
{
}

void AdminProductsFrame::InitializeProducts()
{
    m_products.clear();

    ProductData p1;
    p1.name = wxT("Smartphone Samsung S23");
    p1.category = wxT("Électronique");
    p1.price = 450000;
    p1.stock = 15;
    p1.description = wxT("Smartphone haut de gamme avec écran AMOLED");
    p1.active = true;
    m_products.push_back(p1);

    ProductData p2;
    p2.name = wxT("MacBook Pro 14");
    p2.category = wxT("Électronique");
    p2.price = 1200000;
    p2.stock = 8;
    p2.description = wxT("Ordinateur portable Apple M2");
    p2.active = true;
    m_products.push_back(p2);

    ProductData p3;
    p3.name = wxT("Écouteurs Sony WH-1000XM5");
    p3.category = wxT("Électronique");
    p3.price = 35000;
    p3.stock = 45;
    p3.description = wxT("Casque à réduction de bruit active");
    p3.active = true;
    m_products.push_back(p3);

    ProductData p4;
    p4.name = wxT("T-shirt Nike");
    p4.category = wxT("Vêtements");
    p4.price = 15000;
    p4.stock = 100;
    p4.description = wxT("T-shirt en coton confortable");
    p4.active = true;
    m_products.push_back(p4);

    ProductData p5;
    p5.name = wxT("Jean Levi's 501");
    p5.category = wxT("Vêtements");
    p5.price = 25000;
    p5.stock = 60;
    p5.description = wxT("Jean classique coupe droite");
    p5.active = true;
    m_products.push_back(p5);
}

void AdminProductsFrame::RefreshProductsList()
{
    m_productsList->DeleteAllItems();

    for(size_t i = 0; i < m_products.size(); i++)
    {
        const ProductData& product = m_products[i];

        long index = m_productsList->InsertItem(i, wxString::Format(wxT("%d"), (int)(i + 1)));
        m_productsList->SetItem(index, 1, product.name);
        m_productsList->SetItem(index, 2, product.category);
        m_productsList->SetItem(index, 3, wxString::Format(wxT("%d F"), product.price));
        m_productsList->SetItem(index, 4, wxString::Format(wxT("%d"), product.stock));
        m_productsList->SetItem(index, 5, product.active ? wxT("Actif") : wxT("Inactif"));
        m_productsList->SetItem(index, 6, product.description);

        if(product.active)
            m_productsList->SetItemTextColour(index, wxColour(0, 128, 0));
        else
            m_productsList->SetItemTextColour(index, wxColour(150, 150, 150));

        if(product.stock < 10)
            m_productsList->SetItemBackgroundColour(index, wxColour(255, 240, 240));
    }
}

void AdminProductsFrame::OnAdd(wxCommandEvent& event)
{
    AddProductDialog dialog(this, wxT("Ajouter un produit"));

    if(dialog.ShowModal() == wxID_OK)
    {
        ProductData newProduct = dialog.GetProductData();
        m_products.push_back(newProduct);
        RefreshProductsList();

        wxMessageBox(wxT("Produit ajouté avec succès !\n\nNom : ") + newProduct.name,
                     wxT("Succès"),
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void AdminProductsFrame::OnEdit(wxCommandEvent& event)
{
    long selected = m_productsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox(wxT("Veuillez sélectionner un produit à modifier.\n\n"
                       "Astuce : Cliquez sur une ligne ou double-cliquez pour modifier."),
                     wxT("Aucune sélection"),
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    if(selected >= 0 && selected < (long)m_products.size())
    {
        AddProductDialog dialog(this, wxT("Modifier le produit"));
        dialog.SetProductData(m_products[selected]);

        if(dialog.ShowModal() == wxID_OK)
        {
            m_products[selected] = dialog.GetProductData();
            RefreshProductsList();

            wxMessageBox(wxT("Produit modifié avec succès !"),
                         wxT("Succès"),
                         wxOK | wxICON_INFORMATION,
                         this);
        }
    }
}

void AdminProductsFrame::OnDelete(wxCommandEvent& event)
{
    long selected = m_productsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox(wxT("Veuillez sélectionner un produit à supprimer."),
                     wxT("Aucune sélection"),
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    if(selected >= 0 && selected < (long)m_products.size())
    {
        wxString productName = m_products[selected].name;

        int response = wxMessageBox(
            wxT("Supprimer définitivement le produit :\n\n") + productName + wxT(" ?\n\n")
            + wxT("Cette action est irréversible !"),
            wxT("Confirmation"),
            wxYES_NO | wxICON_QUESTION,
            this
        );

        if(response == wxYES)
        {
            m_products.erase(m_products.begin() + selected);
            RefreshProductsList();

            wxMessageBox(wxT("Produit supprimé avec succès !"),
                         wxT("Suppression"),
                         wxOK | wxICON_INFORMATION,
                         this);
        }
    }
}

void AdminProductsFrame::OnProductDoubleClick(wxListEvent& event)
{
    long selected = event.GetIndex();

    if(selected >= 0 && selected < (long)m_products.size())
    {
        AddProductDialog dialog(this, wxT("Modifier le produit"));
        dialog.SetProductData(m_products[selected]);

        if(dialog.ShowModal() == wxID_OK)
        {
            m_products[selected] = dialog.GetProductData();
            RefreshProductsList();

            wxMessageBox(wxT("Produit modifié avec succès !"),
                         wxT("Succès"),
                         wxOK | wxICON_INFORMATION,
                         this);
        }
    }
}
