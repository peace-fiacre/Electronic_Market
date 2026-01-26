/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/


#include "AddProductDialog.h"

wxBEGIN_EVENT_TABLE(AddProductDialog, wxDialog)
    EVT_BUTTON(ID_OK_BTN, AddProductDialog::OnOK)
    EVT_BUTTON(ID_CANCEL_BTN, AddProductDialog::OnCancel)
wxEND_EVENT_TABLE()

AddProductDialog::AddProductDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Ajouter un produit",
               wxDefaultPosition, wxSize(550, 850))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 250));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "NOUVEAU PRODUIT");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(18);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    titleText->SetForegroundColour(wxColour(40, 167, 69));
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 15);

    mainSizer->AddSpacer(10);

    wxFont labelFont;
    labelFont.SetPointSize(10);
    labelFont.SetWeight(wxFONTWEIGHT_BOLD);

    // Nom
    wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, "Nom du produit :");
    nameLabel->SetFont(labelFont);
    mainSizer->Add(nameLabel, 0, wxLEFT | wxTOP, 40);

    m_nameCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                                wxDefaultPosition, wxSize(420, 35));
    m_nameCtrl->SetHint("Ex: Smartphone Samsung Galaxy S23");
    mainSizer->Add(m_nameCtrl, 0, wxLEFT | wxRIGHT | wxTOP, 40);

    mainSizer->AddSpacer(15);

    // Description
    wxStaticText* descLabel = new wxStaticText(panel, wxID_ANY, "Description :");
    descLabel->SetFont(labelFont);
    mainSizer->Add(descLabel, 0, wxLEFT, 40);

    m_descriptionCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                                       wxDefaultPosition, wxSize(420, 80),
                                       wxTE_MULTILINE);
    m_descriptionCtrl->SetHint("Description detaillee du produit...");
    mainSizer->Add(m_descriptionCtrl, 0, wxLEFT | wxRIGHT | wxTOP, 40);

    mainSizer->AddSpacer(15);

    // Catégorie
    wxStaticText* categoryLabel = new wxStaticText(panel, wxID_ANY, "Categorie :");
    categoryLabel->SetFont(labelFont);
    mainSizer->Add(categoryLabel, 0, wxLEFT, 40);

    wxArrayString categories;
    categories.Add("Electronique");
    categories.Add("Vetements");
    categories.Add("Alimentation");
    categories.Add("Maison");
    categories.Add("Sports");
    categories.Add("Autre");

    m_categoryChoice = new wxChoice(panel, wxID_ANY,
                                    wxDefaultPosition, wxSize(420, 35),
                                    categories);
    m_categoryChoice->SetSelection(0);
    mainSizer->Add(m_categoryChoice, 0, wxLEFT | wxRIGHT | wxTOP, 40);

    mainSizer->AddSpacer(15);

    // Prix
    wxStaticText* priceLabel = new wxStaticText(panel, wxID_ANY, "Prix (F CFA) :");
    priceLabel->SetFont(labelFont);
    mainSizer->Add(priceLabel, 0, wxLEFT, 40);

    m_priceCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                                 wxDefaultPosition, wxSize(420, 35));
    m_priceCtrl->SetHint("Ex: 450000");
    mainSizer->Add(m_priceCtrl, 0, wxLEFT | wxRIGHT | wxTOP, 40);

    mainSizer->AddSpacer(15);

    // Stock
    wxStaticText* stockLabel = new wxStaticText(panel, wxID_ANY, "Stock initial :");
    stockLabel->SetFont(labelFont);
    mainSizer->Add(stockLabel, 0, wxLEFT, 40);

    m_stockCtrl = new wxTextCtrl(panel, wxID_ANY, "",
                                 wxDefaultPosition, wxSize(420, 35));
    m_stockCtrl->SetHint("Ex: 10");
    mainSizer->Add(m_stockCtrl, 0, wxLEFT | wxRIGHT | wxTOP, 40);

    mainSizer->AddSpacer(25);

    // Boutons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* cancelBtn = new wxButton(panel, ID_CANCEL_BTN, "Annuler",
                                       wxDefaultPosition, wxSize(150, 40));
    btnSizer->Add(cancelBtn, 0, wxRIGHT, 10);

    wxButton* okBtn = new wxButton(panel, ID_OK_BTN, "Ajouter",
                                   wxDefaultPosition, wxSize(150, 40));
    okBtn->SetBackgroundColour(wxColour(40, 167, 69));
    okBtn->SetForegroundColour(*wxWHITE);
    wxFont btnFont = okBtn->GetFont();
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);
    okBtn->SetFont(btnFont);
    btnSizer->Add(okBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxCENTER, 20);

    panel->SetSizer(mainSizer);

    Centre();
    m_nameCtrl->SetFocus();
}

AddProductDialog::~AddProductDialog()
{
}

void AddProductDialog::OnOK(wxCommandEvent& event)
{
    m_productName = m_nameCtrl->GetValue().Trim();
    m_description = m_descriptionCtrl->GetValue().Trim();
    m_price = m_priceCtrl->GetValue().Trim();
    m_stock = m_stockCtrl->GetValue().Trim();
    m_category = m_categoryChoice->GetStringSelection();

    if(m_productName.IsEmpty())
    {
        wxMessageBox("Veuillez entrer le nom du produit.",
                     "Champ requis", wxOK | wxICON_WARNING, this);
        m_nameCtrl->SetFocus();
        return;
    }

    if(m_price.IsEmpty())
    {
        wxMessageBox("Veuillez entrer le prix.",
                     "Champ requis", wxOK | wxICON_WARNING, this);
        m_priceCtrl->SetFocus();
        return;
    }

    if(m_stock.IsEmpty())
    {
        wxMessageBox("Veuillez entrer le stock.",
                     "Champ requis", wxOK | wxICON_WARNING, this);
        m_stockCtrl->SetFocus();
        return;
    }

    double priceValue;
    if(!m_price.ToDouble(&priceValue) || priceValue < 0)
    {
        wxMessageBox("Le prix doit etre un nombre positif.",
                     "Prix invalide", wxOK | wxICON_WARNING, this);
        m_priceCtrl->SetFocus();
        return;
    }

    long stockValue;
    if(!m_stock.ToLong(&stockValue) || stockValue < 0)
    {
        wxMessageBox("Le stock doit etre un nombre positif.",
                     "Stock invalide", wxOK | wxICON_WARNING, this);
        m_stockCtrl->SetFocus();
        return;
    }

    EndModal(wxID_OK);
}

void AddProductDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
