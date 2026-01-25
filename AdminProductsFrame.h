#ifndef ADMINPRODUCTSFRAME_H
#define ADMINPRODUCTSFRAME_H

#include <wx/wx.h>
#include <wx/grid.h>

class AdminProductsFrame : public wxFrame
{
public:
    AdminProductsFrame(wxWindow* parent);
    virtual ~AdminProductsFrame();

private:
    wxGrid* m_productsGrid;

    void PopulateProducts();
    void OnAdd(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);

    enum
    {
        ID_ADD = wxID_HIGHEST + 1,
        ID_DELETE
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // ADMINPRODUCTSFRAME_H
