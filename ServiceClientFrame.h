/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#ifndef SERVICECLIENTFRAME_H
#define SERVICECLIENTFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>

class ServiceClientFrame : public wxFrame
{
public:
    ServiceClientFrame(wxWindow* parent);
    virtual ~ServiceClientFrame();

private:
    wxPanel* m_panel;
    wxNotebook* m_notebook;

    // Page Réclamations
    wxPanel* m_reclamationsPage;
    wxListCtrl* m_reclamationsList;
    wxTextCtrl* m_searchCommandeCtrl;

    // Page Retours
    wxPanel* m_retoursPage;
    wxListCtrl* m_retoursList;

    void CreateReclamationsPage();
    void CreateRetoursPage();
    void PopulateReclamations();
    void PopulateRetours();

    void OnRechercher(wxCommandEvent& event);
    void OnTraiterReclamation(wxCommandEvent& event);
    void OnInitierRetour(wxCommandEvent& event);
    void OnValiderRetour(wxCommandEvent& event);
    void OnRefuserRetour(wxCommandEvent& event);
    void OnTraiterRemboursement(wxCommandEvent& event);
    void OnContacterClient(wxCommandEvent& event);

    enum
    {
        ID_RECHERCHER = wxID_HIGHEST + 1,
        ID_TRAITER_RECLAMATION,
        ID_INITIER_RETOUR,
        ID_VALIDER_RETOUR,
        ID_REFUSER_RETOUR,
        ID_REMBOURSER,
        ID_CONTACTER
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // SERVICECLIENTFRAME_H
