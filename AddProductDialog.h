/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/




#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <wx/wx.h>

class AddProductDialog : public wxDialog
{
public:
    AddProductDialog(wxWindow* parent);
    virtual ~AddProductDialog();

    wxString GetProductName() { return m_productName; }
    wxString GetCategory() { return m_category; }
    wxString GetDescription() { return m_description; }
    wxString GetPrice() { return m_price; }
    wxString GetStock() { return m_stock; }

private:
    wxTextCtrl* m_nameCtrl;
    wxTextCtrl* m_descriptionCtrl;
    wxChoice* m_categoryChoice;
    wxTextCtrl* m_priceCtrl;
    wxTextCtrl* m_stockCtrl;

    wxString m_productName;
    wxString m_description;
    wxString m_category;
    wxString m_price;
    wxString m_stock;

    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    enum
    {
        ID_OK_BTN = wxID_HIGHEST + 1,
        ID_CANCEL_BTN
    };

    wxDECLARE_EVENT_TABLE();
};

#endif
