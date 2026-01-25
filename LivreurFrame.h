#ifndef LIVREURFRAME_H
#define LIVREURFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>

class LivreurFrame : public wxFrame
{
public:
    LivreurFrame(wxWindow* parent);
    virtual ~LivreurFrame();

private:
    wxPanel* m_panel;
    wxListCtrl* m_livraisonsList;
    wxTextCtrl* m_detailsText;
    wxStaticText* m_statusText;

    void PopulateLivraisons();
    void OnLivraisonSelected(wxListEvent& event);
    void OnAccepter(wxCommandEvent& event);
    void OnConfirmer(wxCommandEvent& event);
    void OnSignalerProbleme(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);

    enum
    {
        ID_ACCEPTER = wxID_HIGHEST + 1,
        ID_CONFIRMER,
        ID_SIGNALER,
        ID_REFRESH
    };

    wxDECLARE_EVENT_TABLE();
};

#endif // LIVREURFRAME_H
