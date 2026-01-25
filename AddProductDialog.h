#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

struct ProductData {
    wxString name;
    wxString category;
    int price;
    int stock;
    wxString description;
    bool active;
};

class AddProductDialog : public wxDialog
{
public:
    AddProductDialog(wxWindow* parent, const wxString& title = wxT("Ajouter un produit"));
    virtual ~AddProductDialog();

    ProductData GetProductData() const { return m_productData; }
    void SetProductData(const ProductData& data);

private:
    wxTextCtrl* m_nameCtrl;
    wxChoice* m_categoryChoice;
    wxSpinCtrl* m_priceCtrl;
    wxSpinCtrl* m_stockCtrl;
    wxTextCtrl* m_descriptionCtrl;
    wxCheckBox* m_activeCheck;

    ProductData m_productData;

    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // ADDPRODUCTDIALOG_H
