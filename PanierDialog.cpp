/***************************************************************
 * Name:      PanierDialog.cpp
 * Purpose:   Dialog pour afficher et gérer le panier
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/

#include "PanierDialog.h"
#include "CartManager.h"
#include "DatabaseManager.h"

wxBEGIN_EVENT_TABLE(PanierDialog, wxDialog)
    EVT_BUTTON(ID_CONTINUE, PanierDialog::OnContinue)
    EVT_BUTTON(ID_REMOVE, PanierDialog::OnRemove)
    EVT_BUTTON(ID_CLEAR, PanierDialog::OnClear)
    EVT_BUTTON(ID_VALIDATE, PanierDialog::OnValidate)
    EVT_BUTTON(ID_MINUS, PanierDialog::OnMinus)
    EVT_BUTTON(ID_PLUS, PanierDialog::OnPlus)
wxEND_EVENT_TABLE()

PanierDialog::PanierDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Mon Panier",
               wxDefaultPosition, wxSize(950, 700))
{
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(wxColour(245, 245, 245));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // TITRE
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "MON PANIER");
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 20);

    // LISTE DES ARTICLES
    m_cartList = new wxListCtrl(panel, wxID_ANY,
                                wxDefaultPosition,
                                wxSize(800, 300),
                                wxLC_REPORT | wxLC_SINGLE_SEL);

    m_cartList->InsertColumn(0, "Produit", wxLIST_FORMAT_LEFT, 350);
    m_cartList->InsertColumn(1, "Prix unitaire", wxLIST_FORMAT_RIGHT, 150);
    m_cartList->InsertColumn(2, "Quantite", wxLIST_FORMAT_CENTER, 100);
    m_cartList->InsertColumn(3, "Total", wxLIST_FORMAT_RIGHT, 150);

    mainSizer->Add(m_cartList, 0, wxALL | wxCENTER, 10);

    // BOUTONS QUANTITE
    wxBoxSizer* qtyBtnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* minusBtn = new wxButton(panel, ID_MINUS, "-",
                                      wxDefaultPosition, wxSize(50, 35));
    qtyBtnSizer->Add(minusBtn, 0, wxRIGHT, 10);

    wxButton* plusBtn = new wxButton(panel, ID_PLUS, "+",
                                     wxDefaultPosition, wxSize(50, 35));
    qtyBtnSizer->Add(plusBtn, 0, wxRIGHT, 30);

    wxButton* removeBtn = new wxButton(panel, ID_REMOVE, "Retirer article",
                                       wxDefaultPosition, wxSize(140, 35));
    removeBtn->SetBackgroundColour(wxColour(220, 53, 69));
    removeBtn->SetForegroundColour(*wxWHITE);
    qtyBtnSizer->Add(removeBtn, 0, wxRIGHT, 10);

    wxButton* clearBtn = new wxButton(panel, ID_CLEAR, "Vider panier",
                                      wxDefaultPosition, wxSize(130, 35));
    qtyBtnSizer->Add(clearBtn, 0);

    mainSizer->Add(qtyBtnSizer, 0, wxALL | wxCENTER, 10);

    // TOTAL
    m_totalText = new wxStaticText(panel, wxID_ANY, "TOTAL : 0 F CFA");
    wxFont totalFont = m_totalText->GetFont();
    totalFont.SetPointSize(16);
    totalFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_totalText->SetFont(totalFont);
    m_totalText->SetForegroundColour(wxColour(0, 102, 204));
    mainSizer->Add(m_totalText, 0, wxALL | wxCENTER, 15);

    // BOUTONS PRINCIPAUX
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* continueBtn = new wxButton(panel, ID_CONTINUE, "Continuer mes achats",
                                         wxDefaultPosition, wxSize(220, 50));
    wxFont contBtnFont = continueBtn->GetFont();
    contBtnFont.SetPointSize(11);
    continueBtn->SetFont(contBtnFont);
    btnSizer->Add(continueBtn, 0, wxRIGHT, 15);

    wxButton* validateBtn = new wxButton(panel, ID_VALIDATE, "VALIDER LA COMMANDE",
                                         wxDefaultPosition, wxSize(250, 50));
    validateBtn->SetBackgroundColour(wxColour(40, 167, 69));
    validateBtn->SetForegroundColour(*wxWHITE);
    wxFont btnFont = validateBtn->GetFont();
    btnFont.SetWeight(wxFONTWEIGHT_BOLD);
    btnFont.SetPointSize(12);
    validateBtn->SetFont(btnFont);
    btnSizer->Add(validateBtn, 0);

    mainSizer->Add(btnSizer, 0, wxALL | wxCENTER, 25);

    panel->SetSizer(mainSizer);

    // CHARGER LE PANIER
    LoadCart();

    Centre();
}

PanierDialog::~PanierDialog()
{
}

void PanierDialog::LoadCart()
{
    m_cartList->DeleteAllItems();

    CartManager& cart = CartManager::GetInstance();
    std::vector<CartItem> items = cart.GetAllItems();

    if(items.empty())
    {
        m_totalText->SetLabel("TOTAL : 0 F CFA");
        return;
    }

    for(size_t i = 0; i < items.size(); i++)
    {
        const CartItem& item = items[i];

        long idx = m_cartList->InsertItem(i, item.nom_produit);
        m_cartList->SetItem(idx, 1, wxString::Format("%.0f F", item.prix_unitaire));
        m_cartList->SetItem(idx, 2, wxString::Format("%d", item.quantite));
        m_cartList->SetItem(idx, 3, wxString::Format("%.0f F", item.GetTotal()));

        // Stocker l'ID dans les données de l'item
        m_cartList->SetItemData(idx, item.id_produit);
    }

    UpdateTotal();
}

void PanierDialog::UpdateTotal()
{
    double total = CartManager::GetInstance().GetTotal();
    m_totalText->SetLabel(wxString::Format("TOTAL : %.0f F CFA", total));
}

void PanierDialog::OnContinue(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void PanierDialog::OnRemove(wxCommandEvent& event)
{
    long selected = m_cartList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un article.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    wxString productName = m_cartList->GetItemText(selected, 0);
    int id_produit = (int)m_cartList->GetItemData(selected);

    int response = wxMessageBox("Retirer " + productName + " du panier ?",
                                "Confirmation",
                                wxYES_NO | wxICON_QUESTION,
                                this);

    if(response == wxYES)
    {
        CartManager::GetInstance().RemoveProduct(id_produit);
        LoadCart();
    }
}

void PanierDialog::OnClear(wxCommandEvent& event)
{
    if(CartManager::GetInstance().IsEmpty())
    {
        wxMessageBox("Le panier est deja vide.",
                     "Panier vide",
                     wxOK | wxICON_INFORMATION,
                     this);
        return;
    }

    int response = wxMessageBox("Vider completement le panier ?",
                                "Confirmation",
                                wxYES_NO | wxICON_QUESTION,
                                this);

    if(response == wxYES)
    {
        CartManager::GetInstance().ClearCart();
        LoadCart();
        wxMessageBox("Panier vide !",
                     "Succes",
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void PanierDialog::OnMinus(wxCommandEvent& event)
{
    long selected = m_cartList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un article.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    int id_produit = (int)m_cartList->GetItemData(selected);
    wxString qtyStr = m_cartList->GetItemText(selected, 2);
    long qty;
    qtyStr.ToLong(&qty);

    if(qty <= 1)
    {
        wxMessageBox("La quantite est deja a 1.\nUtilisez 'Retirer article' pour supprimer du panier.",
                     "Quantite minimale",
                     wxOK | wxICON_INFORMATION,
                     this);
        return;
    }

    CartManager::GetInstance().UpdateQuantity(id_produit, (int)qty - 1);
    LoadCart();
}

void PanierDialog::OnPlus(wxCommandEvent& event)
{
    long selected = m_cartList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if(selected == -1)
    {
        wxMessageBox("Veuillez selectionner un article.",
                     "Aucune selection",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    int id_produit = (int)m_cartList->GetItemData(selected);
    wxString qtyStr = m_cartList->GetItemText(selected, 2);
    long qty;
    qtyStr.ToLong(&qty);

    // Verifier stock
    Product p = DatabaseManager::GetInstance().GetProduct(id_produit);

    if(qty + 1 > p.stock)
    {
        wxMessageBox(wxString::Format("Stock insuffisant.\nDisponible : %d", p.stock),
                     "Stock insuffisant",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    CartManager::GetInstance().UpdateQuantity(id_produit, (int)qty + 1);
    LoadCart();
}

void PanierDialog::OnValidate(wxCommandEvent& event)
{
    if(CartManager::GetInstance().IsEmpty())
    {
        wxMessageBox("Le panier est vide.\nAjoutez des produits avant de valider.",
                     "Panier vide",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    double total = CartManager::GetInstance().GetTotal();
    int nbItems = CartManager::GetInstance().GetItemCount();

    wxString message;
    message << "Valider la commande ?\n\n";
    message << "Nombre d'articles : " << nbItems << "\n";
    message << "Montant total : " << wxString::Format("%.0f F CFA", total) << "\n\n";
    message << "Une adresse de livraison vous sera demandee.";

    int response = wxMessageBox(message,
                                "Confirmation de commande",
                                wxYES_NO | wxICON_QUESTION,
                                this);

    if(response == wxYES)
    {
        // Créer la commande
        CreateOrder();
    }
}

void PanierDialog::CreateOrder()
{
    // Demander l'adresse
    wxTextEntryDialog addrDialog(
        this,
        "Entrez votre adresse de livraison :",
        "Adresse de livraison",
        "",
        wxOK | wxCANCEL | wxTE_MULTILINE
    );

    if(addrDialog.ShowModal() != wxID_OK)
    {
        return;
    }

    wxString adresse = addrDialog.GetValue().Trim();

    if(adresse.IsEmpty())
    {
        wxMessageBox("L'adresse ne peut pas etre vide.",
                     "Adresse requise",
                     wxOK | wxICON_WARNING,
                     this);
        return;
    }

    // Créer la commande en BD
    double total = CartManager::GetInstance().GetTotal();
    int id_commande = DatabaseManager::GetInstance().CreateOrder(1, adresse, total);

    if(id_commande <= 0)
    {
        wxMessageBox("Erreur lors de la creation de la commande.",
                     "Erreur",
                     wxOK | wxICON_ERROR,
                     this);
        return;
    }

    // Ajouter les lignes de commande
    std::vector<CartItem> items = CartManager::GetInstance().GetAllItems();

    for(const auto& item : items)
    {
        DatabaseManager::GetInstance().AddOrderLine(
            id_commande,
            item.id_produit,
            item.quantite,
            item.prix_unitaire
        );
    }

    // Vider le panier
    CartManager::GetInstance().ClearCart();

    // Message de succès
    wxString successMsg;
    successMsg << "Commande validee avec succes !\n\n";
    successMsg << "Numero de commande : CMD-" << id_commande << "\n";
    successMsg << "Montant total : " << wxString::Format("%.0f F CFA", total) << "\n";
    successMsg << "Adresse : " << adresse << "\n\n";
    successMsg << "Vous recevrez une confirmation par email.";

    wxMessageBox(successMsg,
                 "Commande enregistree",
                 wxOK | wxICON_INFORMATION,
                 this);

    EndModal(wxID_OK);
}
