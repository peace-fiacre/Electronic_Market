/***************************************************************
 * Name:      ElectronicMarketMain.h
 * Purpose:   Defines Application Frame
 * Author:    Peace Fiacre (fiacreegoudjobi@gmail.com)
 * Created:   2026-01-16
 * Copyright: Peace Fiacre ()
 * License:
 **************************************************************/

#ifndef ELECTRONICMARKETMAIN_H
#define ELECTRONICMARKETMAIN_H

//(*Headers(ElectronicMarketFrame)
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
//*)

class ElectronicMarketFrame: public wxFrame
{
    public:

        ElectronicMarketFrame(wxWindow* parent,wxWindowID id = -1,
                              const wxString& userType = wxEmptyString,
                              const wxString& username = wxEmptyString);
        virtual ~ElectronicMarketFrame();

    private:

        //(*Handlers(ElectronicMarketFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(ElectronicMarketFrame)
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long idLogout;
        static const long idOpenCatalogue;
        static const long idOpenPanier;
        static const long idOpenSupport;
        static const long idOpenAdmin;
        static const long idOpenLivreur;
        static const long idOpenServiceClient;
        //*)

        //(*Declarations(ElectronicMarketFrame)
        wxStatusBar* StatusBar1;
        //*)

        // Informations utilisateur (affichees dans la barre d'etat)
        wxString m_userType;
        wxString m_username;

        // Label visible du role/utilisateur dans l'entete
        wxStaticText* m_userLabel;

        // Handlers
        void OnLogout(wxCommandEvent& event);
        void OnOpenCatalogue(wxCommandEvent& event);
        void OnOpenPanier(wxCommandEvent& event);
        void OnOpenSupport(wxCommandEvent& event);
        void OnOpenAdmin(wxCommandEvent& event);
        void OnOpenLivreur(wxCommandEvent& event);
        void OnOpenServiceClient(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif // ELECTRONICMARKETMAIN_H
