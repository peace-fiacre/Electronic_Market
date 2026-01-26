/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/


#ifndef CATALOGUEFRAME_H
#define CATALOGUEFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>              // ← AJOUTER
#include "DatabaseManager.h"   // ← AJOUTER

class CatalogueFrame : public wxFrame
{
public:
    CatalogueFrame(wxWindow* parent);
    virtual ~CatalogueFrame();

private:
    wxPanel* m_panel;
    wxMenuBar* m_menuBar;
    wxTextCtrl* m_searchCtrl;
    wxButton* m_searchBtn;
    wxChoice* m_categoryChoice;
    wxListCtrl* m_productList;
    wxButton* m_viewCartBtn;
    wxStatusBar* m_statusBar;

    int m_cartItemCount;
    std::vector<Product> m_products;  // ← MAINTENANT ÇA VA MARCHER

    void CreateMenuBar();
    void PopulateProducts();

    void OnSearch(wxCommandEvent& event);
    void OnViewCart(wxCommandEvent& event);
    void OnProductDoubleClick(wxListEvent& event);

    enum
    {
        ID_SEARCH = wxID_HIGHEST + 1,
        ID_VIEW_CART
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // CATALOGUEFRAME_H
