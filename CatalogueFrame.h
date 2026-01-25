#ifndef CATALOGUEFRAME_H
#define CATALOGUEFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>

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
