#include "PanierDialog.h"

wxBEGIN_EVENT_TABLE(PanierDialog, wxDialog)
    EVT_BUTTON(ID_ORDER, PanierDialog::OnOrder)
    EVT_BUTTON(ID_CONTINUE, PanierDialog::OnContinue)
    EVT_BUTTON(ID_REMOVE, PanierDialog::OnRemove)
wxEND_EVENT_TABLE()

PanierDialog::PanierDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("Mon Panier"),
               wxDefaultPosition, wxSize(900, 700))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 245));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Titre
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, wxT("MON PANIER"));
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // Grille
    m_cartGrid = new wxGrid(panel, wxID_ANY);
    m_cartGrid->CreateGrid(3, 5);
    m_cartGrid->SetMinSize(wxSize(840, 350));

    m_cartGrid->SetColLabelValue(0, wxT("Produit"));
    m_cartGrid->SetColLabelValue(1, wxT("Prix unitaire"));
    m_cartGrid->SetColLabelValue(2, wxT("Quantite"));
    m_cartGrid->SetColLabelValue(3, wxT("Sous-total"));
    m_cartGrid->SetColLabelValue(4, wxT("Selection"));

    m_cartGrid->SetColSize(0, 300);
    m_cartGrid->SetColSize(1, 120);
    m_cartGrid->SetColSize(2, 100);
    m_cartGrid->SetColSize(3, 120);
    m_cartGrid->SetColSize(4, 100);

    m_cartGrid->SetCellValue(0, 0, wxT("Smartphone Samsung"));
    m_cartGrid->SetCellValue(0, 1, wxT("450000 F"));
    m_cartGrid->SetCellValue(0, 2, wxT("1"));
    m_cartGrid->SetCellValue(0, 3, wxT("450000 F"));

    m_cartGrid->SetCellValue(1, 0, wxT("Ecouteurs Sony"));
    m_cartGrid->SetCellValue(1, 1, wxT("35000 F"));
    m_cartGrid->SetCellValue(1, 2, wxT("2"));
    m_cartGrid->SetCellValue(1, 3, wxT("70000 F"));

    m_cartGrid->SetCellValue(2, 0, wxT("T-shirt Nike"));
    m_cartGrid->SetCellValue(2, 1, wxT("15000 F"));
    m_cartGrid->SetCellValue(2, 2, wxT("3"));
    m_cartGrid->SetCellValue(2, 3, wxT("45000 F"));

    m_cartGrid->EnableEditing(false);

    mainSizer->Add(m_cartGrid, 1, wxALL | wxEXPAND, 20);

    // Bouton Retirer
    wxButton* removeBtn = new wxButton(panel, ID_REMOVE,
                                       wxT("Retirer produit selectionne"),
                                       wxDefaultPosition, wxSize(220, 40));
    removeBtn->SetBackgroundColour(wxColour(220, 53, 69));
    removeBtn->SetForegroundColour(*wxWHITE);
    mainSizer->Add(removeBtn, 0, wxLEFT | wxBOTTOM, 20);

    // Total
    wxBoxSizer* totalSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* totalLabel = new wxStaticText(panel, wxID_ANY, wxT("TOTAL :"));
    wxFont totalFont = totalLabel->GetFont();
    totalFont.SetPointSize(18);
    totalFont.SetWeight(wxFONTWEIGHT_BOLD);
    totalLabel->SetFont(totalFont);
    totalSizer->Add(totalLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 20);

    m_totalAmount = new wxStaticText(panel, wxID_ANY, wxT("565 000 F"));
    m_totalAmount->SetFont(totalFont);
    m_totalAmount->SetForegroundColour(wxColour(220, 53, 69));
    totalSizer->Add(m_totalAmount, 0, wxALIGN_CENTER_VERTICAL);

    mainSizer->Add(totalSizer, 0, wxALL | wxALIGN_RIGHT, 20);

    // Boutons d'action
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* continueBtn = new wxButton(panel, ID_CONTINUE,
                                         wxT("Continuer mes achats"),
                                         wxDefaultPosition, wxSize(200, 45));
    btnSizer->Add(continueBtn, 0, wxRIGHT, 10);

    wxButton* orderBtn = new wxButton(panel, ID_ORDER,
                                      wxT("Passer commande"),
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
        wxMessageBox(wxT("Veuillez selectionner une ligne a retirer."),
                     wxT("Aucune selection"),
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString productName = m_cartGrid->GetCellValue(row, 0);

    if(productName.IsEmpty())
    {
        wxMessageBox(wxT("Cette ligne est deja vide."),
                     wxT("Erreur"),
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    int response = wxMessageBox(
        wxString::Format(wxT("Retirer du panier :\n\n%s ?"), productName),
        wxT("Confirmation"),
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        for(int col = 0; col < m_cartGrid->GetNumberCols(); col++)
        {
            m_cartGrid->SetCellValue(row, col, wxT(""));
        }

        wxMessageBox(wxT("Produit retire du panier !"),
                     wxT("Succes"),
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void PanierDialog::OnOrder(wxCommandEvent& event)
{
    int response = wxMessageBox(
        wxT("Confirmer la commande ?\n\n"
        "Montant total : 565 000 F CFA\n"
        "Articles : 3\n\n"
        "Vous allez etre redirige vers le paiement."),
        wxT("Confirmation de commande"),
        wxYES_NO | wxICON_QUESTION,
        this
    );

    if(response == wxYES)
    {
        wxString message;
        message << wxT("Commande validee avec succes !\n\n")
                << wxT("N° de commande : CMD-2026-")
                << wxString::Format(wxT("%03d"), wxGetLocalTime() % 1000) << wxT("\n")
                << wxT("Montant : 565 000 F CFA\n\n")
                << wxT("Vous recevrez un email de confirmation.\n")
                << wxT("Livraison prevue sous 24-48h.");

        wxMessageBox(message,
                     wxT("Commande confirmee"),
                     wxOK | wxICON_INFORMATION,
                     this);

        EndModal(wxID_OK);
    }
}

void PanierDialog::OnContinue(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
