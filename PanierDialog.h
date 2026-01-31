/***************************************************************
 * Name:      PanierDialog.h
 * Purpose:   Header for Panier Dialog
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/

#ifndef PANIERDIALOG_H
#define PANIERDIALOG_H

#include <wx/wx.h>
#include <wx/listctrl.h>

class PanierDialog : public wxDialog
{
public:
    PanierDialog(wxWindow* parent);
    virtual ~PanierDialog();

private:
    wxListCtrl* m_cartList;
    wxStaticText* m_totalText;

    void LoadCart();
    void UpdateTotal();
    void CreateOrder();

    void OnContinue(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnValidate(wxCommandEvent& event);
    void OnMinus(wxCommandEvent& event);
    void OnPlus(wxCommandEvent& event);

    enum
    {
        ID_CONTINUE = wxID_HIGHEST + 1,
        ID_REMOVE,
        ID_CLEAR,
        ID_VALIDATE,
        ID_MINUS,
        ID_PLUS
    };

    wxDECLARE_EVENT_TABLE();
};

#endif
