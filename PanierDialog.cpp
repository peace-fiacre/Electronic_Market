/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#include "PanierDialog.h"
#include "DatabaseManager.h"

wxBEGIN_EVENT_TABLE(PanierDialog, wxDialog)
    EVT_BUTTON(ID_ORDER, PanierDialog::OnOrder)
    EVT_BUTTON(ID_CONTINUE, PanierDialog::OnContinue)
    EVT_BUTTON(ID_REMOVE, PanierDialog::OnRemove)
wxEND_EVENT_TABLE()

PanierDialog::PanierDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Mon Panier",
               wxDefaultPosition, wxSize(900, 700))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 245));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "MON PANIER");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    m_cartGrid = new wxGrid(panel, wxID_ANY);
    m_cartGrid->CreateGrid(3, 5);
    m_cartGrid->SetMinSize(wxSize(840, 350));

    m_cartGrid->SetColLabelValue(0, "Produit");
    m_cartGrid->SetColLabelValue(1, "Prix unitaire");
    m_cartGrid->SetColLabelValue(2, "Quantite");
    m_cartGrid->SetColLabelValue(3, "Sous-total");
    m_cartGrid->SetColLabelValue(4, "Selection");

    m_cartGrid->SetColSize(0, 300);
    m_cartGrid->SetColSize(1, 120);
    m_cartGrid->SetColSize(2, 100);
    m_cartGrid->SetColSize(3, 120);
    m_cartGrid->SetColSize(4, 100);

    m_cartGrid->SetCellValue(0, 0, "Smartphone Samsung");
    m_cartGrid->SetCellValue(0, 1, "450000 F");
    m_cartGrid->SetCellValue(0, 2, "1");
    m_cartGrid->SetCellValue(0, 3, "450000 F");

    m_cartGrid->SetCellValue(1, 0, "Ecouteurs Sony");
    m_cartGrid->SetCellValue(1, 1, "35000 F");
    m_cartGrid->SetCellValue(1, 2, "2");
    m_cartGrid->SetCellValue(1, 3, "70000 F");

    m_cartGrid->SetCellValue(2, 0, "T-shirt Nike");
    m_cartGrid->SetCellValue(2, 1, "15000 F");
    m_cartGrid->SetCellValue(2, 2, "3");
    m_cartGrid->SetCellValue(2, 3, "45000 F");

    m_cartGrid->EnableEditing(false);

    mainSizer->Add(m_cartGrid, 1, wxALL | wxEXPAND, 20);

    wxButton* removeBtn = new wxButton(panel, ID_REMOVE,
                                       "Retirer produit selectionne",
                                       wxDefaultPosition, wxSize(220, 40));
    removeBtn->SetBackgroundColour(wxColour(220, 53, 69));
    removeBtn->SetForegroundColour(*wxWHITE);
    mainSizer->Add(removeBtn, 0, wxLEFT | wxBOTTOM, 20);

    wxBoxSizer* totalSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* totalLabel = new wxStaticText(panel, wxID_ANY, "TOTAL :");
    wxFont totalFont = totalLabel->GetFont();
    totalFont.SetPointSize(18);
    totalFont.SetWeight(wxFONTWEIGHT_BOLD);
    totalLabel->SetFont(totalFont);
    totalSizer->Add(totalLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 20);

    m_totalAmount = new wxStaticText(panel, wxID_ANY, "565 000 F");
    m_totalAmount->SetFont(totalFont);
    m_totalAmount->SetForegroundColour(wxColour(220, 53, 69));
    totalSizer->Add(m_totalAmount, 0, wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(totalSizer, 0, wxALL | wxALIGN_RIGHT, 20);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* continueBtn = new wxButton(panel, ID_CONTINUE, "Continuer mes achats",
                                         wxDefaultPosition, wxSize(200, 45));
    btnSizer->Add(continueBtn, 0, wxRIGHT, 10);

    wxButton* orderBtn = new wxButton(panel, ID_ORDER, "Passer commande",
                                      wxDefaultPosition, wxSize(200, 45));
    orderBtn->SetBackgroundColour(wxColour(40, 167, 69));
    orderBtn->SetForegroundColour(*wxWHITE);
    btnSizer->Add(orderBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxCENTER, 20);

    panel->SetSizer(mainSizer);

    Centre();
}

PanierDialog::~PanierDialog()
{
}

void PanierDialog::OnRemove(wxCommandEvent& event)
{
    int row = m_cartGrid->GetGridCursorRow();

    if(row < 0)
    {
        wxMessageBox("Veuillez selectionner une ligne a retirer.",
                     "Aucune selection", wxOK | wxICON_WARNING, this);
        return;
    }

    wxString productName = m_cartGrid->GetCellValue(row, 0);

    if(productName.IsEmpty())
    {
        wxMessageBox("Cette ligne est vide.", "Erreur", wxOK | wxICON_WARNING, this);
        return;
    }

    int response = wxMessageBox("Retirer du panier :\n\n" + productName + " ?",
                                "Confirmation", wxYES_NO | wxICON_QUESTION, this);

    if(response == wxYES)
    {
        for(int col = 0; col < m_cartGrid->GetNumberCols(); col++)
        {
            m_cartGrid->SetCellValue(row, col, "");
        }

        wxMessageBox("Produit retire du panier !", "Succes",
                     wxOK | wxICON_INFORMATION, this);
    }
}

void PanierDialog::OnOrder(wxCommandEvent& event)
{
    wxTextEntryDialog adresseDlg(
        this,
        "Entrez votre adresse de livraison :",
        "Adresse de livraison",
        "Quartier Agla, Rue 123, Cotonou"
    );

    if(adresseDlg.ShowModal() != wxID_OK)
        return;

    wxString adresse = adresseDlg.GetValue();

    if(adresse.IsEmpty())
    {
        wxMessageBox("Adresse requise.", "Erreur", wxOK | wxICON_WARNING, this);
        return;
    }

    int response = wxMessageBox(
        "Confirmer la commande ?\n\n"
        "Montant total : 565 000 F CFA\n"
        "Articles : 3\n"
        "Adresse : " + adresse + "\n\n"
        "Vous allez etre redirige vers le paiement.",
        "Confirmation de commande",
        wxYES_NO | wxICON_QUESTION, this);

    if(response == wxYES)
    {
        // CRÉER LA COMMANDE EN BASE DE DONNÉES
        int id_commande = DatabaseManager::GetInstance().CreateOrder(1, adresse, 565000.0);

        if(id_commande > 0)
        {
            // Ajouter les lignes de commande (exemple avec IDs fictifs)
            DatabaseManager::GetInstance().AddOrderLine(id_commande, 1, 1, 450000.0);
            DatabaseManager::GetInstance().AddOrderLine(id_commande, 3, 2, 35000.0);
            DatabaseManager::GetInstance().AddOrderLine(id_commande, 4, 3, 15000.0);

            wxMessageBox("Commande validee avec succes !\n\n"
                         "N° de commande : CMD-" + wxString::Format("%d", id_commande) + "\n"
                         "Montant : 565 000 F CFA\n\n"
                         "La commande a ete enregistree en base de donnees.\n"
                         "Le livreur sera notifie.\n\n"
                         "Vous recevrez un email de confirmation.\n"
                         "Livraison prevue sous 24-48h.",
                         "Commande confirmee",
                         wxOK | wxICON_INFORMATION, this);

            EndModal(wxID_OK);
        }
        else
        {
            wxMessageBox("Erreur lors de la creation de la commande.",
                         "Erreur", wxOK | wxICON_ERROR, this);
        }
    }
}

void PanierDialog::OnContinue(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
