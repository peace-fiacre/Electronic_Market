#include "AddProductDialog.h"

wxBEGIN_EVENT_TABLE(AddProductDialog, wxDialog)
    EVT_BUTTON(wxID_OK, AddProductDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, AddProductDialog::OnCancel)
wxEND_EVENT_TABLE()

AddProductDialog::AddProductDialog(wxWindow* parent, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(600, 550))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxFont labelFont;
    labelFont.SetPointSize(10);
    labelFont.SetWeight(wxFONTWEIGHT_BOLD);

    // Nom du produit
    wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, wxT("Nom du produit * :"));
    nameLabel->SetFont(labelFont);
    mainSizer->Add(nameLabel, 0, wxALL, 10);

    m_nameCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(550, 35));
    m_nameCtrl->SetHint(wxT("Ex: Smartphone Samsung Galaxy S23"));
    mainSizer->Add(m_nameCtrl, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Catégorie
    wxStaticText* categoryLabel = new wxStaticText(panel, wxID_ANY, wxT("Catégorie * :"));
    categoryLabel->SetFont(labelFont);
    mainSizer->Add(categoryLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);

    wxArrayString categories;
    categories.Add(wxT("Électronique"));
    categories.Add(wxT("Vêtements"));
    categories.Add(wxT("Alimentation"));
    categories.Add(wxT("Maison"));
    categories.Add(wxT("Sports"));
    categories.Add(wxT("Livres"));
    categories.Add(wxT("Jouets"));
    categories.Add(wxT("Autre"));

    m_categoryChoice = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(550, 35), categories);
    m_categoryChoice->SetSelection(0);
    mainSizer->Add(m_categoryChoice, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Prix et Stock sur la même ligne
    wxBoxSizer* priceStockSizer = new wxBoxSizer(wxHORIZONTAL);

    // Prix
    wxBoxSizer* priceSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* priceLabel = new wxStaticText(panel, wxID_ANY, wxT("Prix (F CFA) * :"));
    priceLabel->SetFont(labelFont);
    priceSizer->Add(priceLabel, 0, wxBOTTOM, 5);

    m_priceCtrl = new wxSpinCtrl(panel, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(260, 35),
                                 wxSP_ARROW_KEYS, 0, 10000000, 0);
    priceSizer->Add(m_priceCtrl, 0);

    priceStockSizer->Add(priceSizer, 0, wxRIGHT, 15);

    // Stock
    wxBoxSizer* stockSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* stockLabel = new wxStaticText(panel, wxID_ANY, wxT("Stock initial * :"));
    stockLabel->SetFont(labelFont);
    stockSizer->Add(stockLabel, 0, wxBOTTOM, 5);

    m_stockCtrl = new wxSpinCtrl(panel, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(260, 35),
                                 wxSP_ARROW_KEYS, 0, 100000, 0);
    stockSizer->Add(m_stockCtrl, 0);

    priceStockSizer->Add(stockSizer, 0);

    mainSizer->Add(priceStockSizer, 0, wxALL, 10);

    // Description
    wxStaticText* descLabel = new wxStaticText(panel, wxID_ANY, wxT("Description :"));
    descLabel->SetFont(labelFont);
    mainSizer->Add(descLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);

    m_descriptionCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""),
                                       wxDefaultPosition, wxSize(550, 100),
                                       wxTE_MULTILINE);
    m_descriptionCtrl->SetHint(wxT("Description détaillée du produit (optionnel)"));
    mainSizer->Add(m_descriptionCtrl, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Actif
    m_activeCheck = new wxCheckBox(panel, wxID_ANY, wxT("Produit actif (visible dans le catalogue)"));
    m_activeCheck->SetValue(true);
    mainSizer->Add(m_activeCheck, 0, wxALL, 10);

    // Ligne de séparation
    wxStaticLine* line = new wxStaticLine(panel);
    mainSizer->Add(line, 0, wxEXPAND | wxALL, 10);

    // Boutons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* okBtn = new wxButton(panel, wxID_OK, wxT("Enregistrer"), wxDefaultPosition, wxSize(150, 40));
    okBtn->SetBackgroundColour(wxColour(40, 167, 69));
    okBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(okBtn, 0, wxRIGHT, 10);

    wxButton* cancelBtn = new wxButton(panel, wxID_CANCEL, wxT("Annuler"), wxDefaultPosition, wxSize(150, 40));
    btnSizer->Add(cancelBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxCENTER, 10);

    panel->SetSizer(mainSizer);

    Centre();
    m_nameCtrl->SetFocus();
}

AddProductDialog::~AddProductDialog()
{
}

void AddProductDialog::SetProductData(const ProductData& data)
{
    m_nameCtrl->SetValue(data.name);

    int categoryIndex = m_categoryChoice->FindString(data.category);
    if(categoryIndex != wxNOT_FOUND)
        m_categoryChoice->SetSelection(categoryIndex);

    m_priceCtrl->SetValue(data.price);
    m_stockCtrl->SetValue(data.stock);
    m_descriptionCtrl->SetValue(data.description);
    m_activeCheck->SetValue(data.active);
}

void AddProductDialog::OnOK(wxCommandEvent& event)
{
    wxString name = m_nameCtrl->GetValue().Trim();

    if(name.IsEmpty())
    {
        wxMessageBox(wxT("Le nom du produit est obligatoire."),
                     wxT("Champ requis"),
                     wxOK | wxICON_WARNING,
                     this);
        m_nameCtrl->SetFocus();
        return;
    }

    int price = m_priceCtrl->GetValue();
    if(price <= 0)
    {
        wxMessageBox(wxT("Le prix doit être supérieur à 0."),
                     wxT("Prix invalide"),
                     wxOK | wxICON_WARNING,
                     this);
        m_priceCtrl->SetFocus();
        return;
    }

    m_productData.name = name;
    m_productData.category = m_categoryChoice->GetStringSelection();
    m_productData.price = price;
    m_productData.stock = m_stockCtrl->GetValue();
    m_productData.description = m_descriptionCtrl->GetValue();
    m_productData.active = m_activeCheck->GetValue();

    EndModal(wxID_OK);
}

void AddProductDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
