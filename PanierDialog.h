/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/


#ifndef PANIERDIALOG_H
#define PANIERDIALOG_H

#include <wx/wx.h>
#include <wx/grid.h>

class PanierDialog : public wxDialog
{
public:
    PanierDialog(wxWindow* parent);
    virtual ~PanierDialog();

private:
    wxGrid* m_cartGrid;
    wxStaticText* m_totalAmount;

    void OnOrder(wxCommandEvent& event);
    void OnContinue(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);

    enum
    {
        ID_ORDER = wxID_HIGHEST + 1,
        ID_CONTINUE,
        ID_REMOVE
    };

    wxDECLARE_EVENT_TABLE();
};

#endif
