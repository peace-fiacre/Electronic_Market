/***************************************************************
 * Name:      ClientSupportFrame.h
 * Purpose:   Interface Support & Reclamations pour les clients
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-31
 **************************************************************/

#ifndef CLIENTSUPPORTFRAME_H
#define CLIENTSUPPORTFRAME_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

class ClientSupportFrame : public wxFrame
{
public:
    ClientSupportFrame(wxWindow* parent);
    virtual ~ClientSupportFrame();

private:
    wxNotebook* m_notebook;
    
    // Page Mes Commandes
    wxPanel* m_ordersPage;
    wxListCtrl* m_ordersList;
    wxTextCtrl* m_orderDetailsText;
    
    // Page Mes Réclamations
    wxPanel* m_claimsPage;
    wxListCtrl* m_claimsList;
    wxTextCtrl* m_claimDetailsText;
    
    void CreateOrdersPage();
    void CreateClaimsPage();
    
    void LoadOrders();
    void LoadClaims();
    
    void OnOrderSelected(wxListEvent& event);
    void OnViewOrderDetails(wxCommandEvent& event);
    void OnTrackOrder(wxCommandEvent& event);
    void OnReorder(wxCommandEvent& event);
    
    void OnClaimSelected(wxListEvent& event);
    void OnNewClaim(wxCommandEvent& event);
    void OnRequestReturn(wxCommandEvent& event);
    
    enum
    {
        ID_VIEW_DETAILS = wxID_HIGHEST + 1,
        ID_TRACK_ORDER,
        ID_REORDER,
        ID_NEW_CLAIM,
        ID_REQUEST_RETURN
    };
    
    wxDECLARE_EVENT_TABLE();
};

#endif // CLIENTSUPPORTFRAME_H
