/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/

#include "ElectronicMarketApp.h"
#include "ElectronicMarketMain.h"
#include "LoginDialog.h"
#include "DatabaseManager.h"

IMPLEMENT_APP(ElectronicMarketApp);


bool ElectronicMarketApp::OnInit()
{
    // INITIALISER LA BASE DE DONNÉES
    DatabaseManager& db = DatabaseManager::GetInstance();

    if(!db.Initialize())
    {
        wxMessageBox("Erreur d'initialisation de la base de donnees !\n\n"
                     "L'application ne peut pas demarrer.",
                     "Erreur critique",
                     wxOK | wxICON_ERROR);
        return false;
    }

    wxLogMessage("Base de donnees initialisee avec succes");

    // AJOUTER DES DONNÉES DE DÉMONSTRATION SI LA DB EST VIDE
    if(!db.InitializeSampleData())
    {
        wxLogWarning("Impossible d'initialiser les donnees de demonstration");
    }
    else
    {
        wxLogMessage("Donnees de demonstration chargees");
    }

    // Créer et afficher la fenêtre de connexion
    LoginDialog* loginDlg = new LoginDialog(NULL);

    if(loginDlg->ShowModal() == wxID_OK)
    {
        wxString userType = loginDlg->GetUserType();
        wxString username = loginDlg->GetUsername();

        loginDlg->Destroy();

        ElectronicMarketFrame* frame = new ElectronicMarketFrame(0L);

        if(userType == "ADMIN")
        {
            frame->SetTitle("Plateforme E-Commerce - Mode Administrateur (" + username + ")");
        }
        else
        {
            frame->SetTitle("Plateforme E-Commerce - Bienvenue " + username);
        }

        frame->Maximize(true);
        frame->Show();

        return true;
    }

    loginDlg->Destroy();
    db.Close();

    return false;
}
