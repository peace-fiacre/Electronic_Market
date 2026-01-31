/***************************************************************
 * Name:      CatalogueFrame.h
 * Purpose:   Catalogue avec affichage en cartes modernes
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-31
 **************************************************************/

#ifndef CATALOGUEFRAME_H
#define CATALOGUEFRAME_H

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include "DatabaseManager.h"

class CatalogueFrame : public wxFrame
{
public:
    CatalogueFrame(wxWindow* parent);
    virtual ~CatalogueFrame();

private:
    wxScrolledWindow* m_scrolledWindow;
    wxPanel* m_cardsPanel;
    wxStaticText* m_cartBadge;
    wxStaticText* m_statusText;

    void LoadProductCards();
    void UpdateCartBadge();

    wxPanel* CreateProductCard(wxWindow* parent, const Product& product);

    void OnBack(wxCommandEvent& event);
    void OnCart(wxCommandEvent& event);
    void OnAddToCart(wxCommandEvent& event);

    enum
    {
        ID_BACK = wxID_HIGHEST + 1,
        ID_CART,
        ID_ADD_TO_CART_BASE = 10000  // Base pour les IDs dynamiques
    };

    wxDECLARE_EVENT_TABLE();
};

#endif
