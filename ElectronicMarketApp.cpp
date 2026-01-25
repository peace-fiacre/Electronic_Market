/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/

#include "ElectronicMarketApp.h"
#include "ElectronicMarketMain.h"
#include "LoginDialog.h"

IMPLEMENT_APP(ElectronicMarketApp);

bool ElectronicMarketApp::OnInit()
{
    // Créer et afficher la fenêtre de connexion
    LoginDialog* loginDlg = new LoginDialog(NULL);
    
    // Si l'utilisateur se connecte avec succès
    if(loginDlg->ShowModal() == wxID_OK)
    {
        // Récupérer le type d'utilisateur connecté
        wxString userType = loginDlg->GetUserType();
        wxString username = loginDlg->GetUsername();
        
        // Détruire la fenêtre de connexion
        loginDlg->Destroy();
        
        // Créer et afficher la fenêtre principale
        ElectronicMarketFrame* frame = new ElectronicMarketFrame(0L);
        
        // Personnaliser le titre selon le type d'utilisateur
        if(userType == "ADMIN")
        {
            frame->SetTitle("Plateforme E-Commerce - Mode Administrateur (" + username + ")");
        }
        else
        {
            frame->SetTitle("Plateforme E-Commerce - Bienvenue " + username);
        }
        
        // METTRE EN PLEIN ÉCRAN
        frame->Maximize(true);
        
        frame->Show();
        
        return true;
    }
    
    // Si l'utilisateur annule la connexion
    loginDlg->Destroy();
    return false;
}
