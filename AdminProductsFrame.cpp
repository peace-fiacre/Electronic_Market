/***************************************************************
 * Name:      AdminProductsFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/


#include "AdminProductsFrame.h"
#include "AddProductDialog.h"
#include "DatabaseManager.h"

wxBEGIN_EVENT_TABLE(AdminProductsFrame, wxFrame)
    EVT_BUTTON(ID_ADD, AdminProductsFrame::OnAdd)
    EVT_BUTTON(ID_DELETE, AdminProductsFrame::OnDelete)
wxEND_EVENT_TABLE()

AdminProductsFrame::AdminProductsFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "Administration - Gestion Produits",
              wxDefaultPosition, wxSize(1200, 800))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(*wxWHITE);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* menuAccueil = new wxMenu;
    wxMenuItem* itemAccueil = menuAccueil->Append(wxID_ANY, "Retour menu principal");
    menuBar->Append(menuAccueil, "Accueil");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        this->Close();
    }, itemAccueil->GetId());

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "GESTION DES PRODUITS");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    m_productsGrid = new wxGrid(panel, wxID_ANY);
    m_productsGrid->CreateGrid(50, 6);

    m_productsGrid->SetColLabelValue(0, "ID");
    m_productsGrid->SetColLabelValue(1, "Nom");
    m_productsGrid->SetColLabelValue(2, "Categorie");
    m_productsGrid->SetColLabelValue(3, "Prix");
    m_productsGrid->SetColLabelValue(4, "Stock");
    m_productsGrid->SetColLabelValue(5, "Actif");

    m_productsGrid->SetColSize(0, 50);
    m_productsGrid->SetColSize(1, 400);
    m_productsGrid->SetColSize(2, 150);
    m_productsGrid->SetColSize(3, 120);
    m_productsGrid->SetColSize(4, 100);
    m_productsGrid->SetColSize(5, 100);

    m_productsGrid->EnableEditing(false);

    PopulateProducts();

    mainSizer->Add(m_productsGrid, 1, wxALL | wxEXPAND, 20);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* addBtn = new wxButton(panel, ID_ADD, "Ajouter Produit",
                                    wxDefaultPosition, wxSize(180, 45));
    addBtn->SetBackgroundColour(wxColour(40, 167, 69));
    addBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(addBtn, 0, wxRIGHT, 10);

    wxButton* deleteBtn = new wxButton(panel, ID_DELETE, "Supprimer Produit",
                                       wxDefaultPosition, wxSize(180, 45));
    deleteBtn->SetBackgroundColour(wxColour(220, 53, 69));
    deleteBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(deleteBtn, 0, wxRIGHT, 10);

    wxButton* refreshBtn = new wxButton(panel, wxID_ANY, "Actualiser",
                                        wxDefaultPosition, wxSize(150, 45));
    refreshBtn->SetBackgroundColour(wxColour(0, 123, 255));
    refreshBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(refreshBtn, 0);

    refreshBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        PopulateProducts();
        wxMessageBox("Liste actualisee !", "Actualisation",
                     wxOK | wxICON_INFORMATION, this);
    });

    mainSizer->Add(btnSizer, 0, wxALL | wxLEFT, 20);

    panel->SetSizer(mainSizer);

    Maximize(true);
}

AdminProductsFrame::~AdminProductsFrame()
{
}

void AdminProductsFrame::PopulateProducts()
{
    // Vider la grille
    for(int i = 0; i < m_productsGrid->GetNumberRows(); i++)
    {
        for(int j = 0; j < m_productsGrid->GetNumberCols(); j++)
        {
            m_productsGrid->SetCellValue(i, j, "");
        }
    }

    // Charger depuis la base de données
    std::vector<Product> products = DatabaseManager::GetInstance().GetAllProducts();

    int row = 0;
    for(const auto& p : products)
    {
        if(row >= m_productsGrid->GetNumberRows())
            break;

        m_productsGrid->SetCellValue(row, 0, wxString::Format("%d", p.id));
        m_productsGrid->SetCellValue(row, 1, p.nom);
        m_productsGrid->SetCellValue(row, 2, p.categorie);
        m_productsGrid->SetCellValue(row, 3, wxString::Format("%.0f F", p.prix));
        m_productsGrid->SetCellValue(row, 4, wxString::Format("%d", p.stock));
        m_productsGrid->SetCellValue(row, 5, p.actif ? "Oui" : "Non");

        row++;
    }
}

void AdminProductsFrame::OnAdd(wxCommandEvent& event)
{
    AddProductDialog* dlg = new AddProductDialog(this);

    if(dlg->ShowModal() == wxID_OK)
    {
        wxString nom = dlg->GetProductName();
        wxString categorie = dlg->GetCategory();
        wxString description = dlg->GetDescription();
        wxString prixStr = dlg->GetPrice();
        wxString stockStr = dlg->GetStock();

        double prix = 0.0;
        long stock = 0;
        prixStr.ToDouble(&prix);
        stockStr.ToLong(&stock);

        if(DatabaseManager::GetInstance().AddProduct(nom, description, categorie, prix, (int)stock))
        {
            wxMessageBox("Produit ajoute avec succes dans la base de donnees !\n\n"
                         "Nom : " + nom + "\n"
                         "Categorie : " + categorie + "\n"
                         "Prix : " + prixStr + " F\n"
                         "Stock : " + stockStr,
                         "Succes", wxOK | wxICON_INFORMATION, this);

            PopulateProducts();
        }
        else
        {
            wxMessageBox("Erreur lors de l'ajout du produit en base de donnees.",
                         "Erreur", wxOK | wxICON_ERROR, this);
        }
    }

    dlg->Destroy();
}

void AdminProductsFrame::OnDelete(wxCommandEvent& event)
{
    int row = m_productsGrid->GetGridCursorRow();

    if(row < 0)
    {
        wxMessageBox("Veuillez selectionner une ligne a supprimer.",
                     "Aucune selection", wxOK | wxICON_WARNING, this);
        return;
    }

    wxString idStr = m_productsGrid->GetCellValue(row, 0);
    wxString productName = m_productsGrid->GetCellValue(row, 1);

    if(idStr.IsEmpty() || productName.IsEmpty())
    {
        wxMessageBox("Cette ligne est vide.", "Erreur", wxOK | wxICON_WARNING, this);
        return;
    }

    long id;
    idStr.ToLong(&id);

    int response = wxMessageBox("Supprimer le produit :\n\n" + productName + " ?",
                                "Confirmation", wxYES_NO | wxICON_QUESTION, this);

    if(response == wxYES)
    {
        if(DatabaseManager::GetInstance().DeleteProduct((int)id))
        {
            wxMessageBox("Produit supprime avec succes de la base de donnees !",
                         "Suppression", wxOK | wxICON_INFORMATION, this);

            PopulateProducts();
        }
        else
        {
            wxMessageBox("Erreur lors de la suppression.",
                         "Erreur", wxOK | wxICON_ERROR, this);
        }
    }
}
