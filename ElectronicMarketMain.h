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
        //*)

        //(*Declarations(ElectronicMarketFrame)
        wxStatusBar* StatusBar1;
        //*)

        // Informations utilisateur (affichees dans la barre d'etat)
        wxString m_userType;
        wxString m_username;

        DECLARE_EVENT_TABLE()
};

#endif // ELECTRONICMARKETMAIN_H
