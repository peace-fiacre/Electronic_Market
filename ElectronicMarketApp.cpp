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
    bool wxsOK = true;
    wxInitAllImageHandlers();

    // Initialiser DatabaseManager
    DatabaseManager& db = DatabaseManager::GetInstance();

    if (!db.Initialize()) {
        wxMessageBox("Erreur lors de l'ouverture de la base de données.\n\n"
                    "Vérifiez que SQLite3 est correctement configuré.",
                    "Erreur Critique", wxICON_ERROR);
        return false;
    }

    // Charger les données de démo
    db.InitializeSampleData();

    // Afficher le login
    LoginDialog loginDlg(NULL);
    if(loginDlg.ShowModal() == wxID_OK)
    {
        if(wxsOK)
        {
            ElectronicMarketFrame* Frame = new ElectronicMarketFrame(0);
            Frame->Show();
            SetTopWindow(Frame);
        }
    }
    else
    {
        return false;
    }

    return wxsOK;
}

int ElectronicMarketApp::OnExit()
{
    DatabaseManager::GetInstance().Close();
    return 0;
}
