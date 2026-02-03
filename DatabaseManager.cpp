/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#include "DatabaseManager.h"
#include <wx/stdpaths.h>

DatabaseManager& DatabaseManager::GetInstance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
    : m_db(nullptr)
{
    wxStandardPaths& paths = wxStandardPaths::Get();
    wxString appDir = paths.GetUserDataDir();

    if(!wxDirExists(appDir))
        wxMkdir(appDir);

    m_dbPath = appDir + "/ecommerce.db";
}

DatabaseManager::~DatabaseManager()
{
    Close();
}

bool DatabaseManager::Initialize()
{
    int rc = sqlite3_open(m_dbPath.mb_str(), &m_db);

    if(rc != SQLITE_OK)
    {
        wxLogError("Erreur ouverture base de donnees: %s", sqlite3_errmsg(m_db));
        return false;
    }

    return CreateTables();
}

void DatabaseManager::Close()
{
    if(m_db)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool DatabaseManager::CreateTables()
{
    const char* sql =
        "CREATE TABLE IF NOT EXISTS produits ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nom TEXT NOT NULL,"
        "description TEXT,"
        "categorie TEXT,"
        "prix REAL NOT NULL,"
        "stock INTEGER DEFAULT 0,"
        "actif INTEGER DEFAULT 1"
        ");"

        "CREATE TABLE IF NOT EXISTS commandes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "id_client INTEGER,"
        "date TEXT,"
        "montant_total REAL,"
        "statut TEXT DEFAULT 'En attente',"
        "adresse TEXT,"
        "numero_suivi TEXT"
        ");"

        "CREATE TABLE IF NOT EXISTS lignes_commande ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "id_commande INTEGER,"
        "id_produit INTEGER,"
        "nom_produit TEXT,"
        "quantite INTEGER,"
        "prix_unitaire REAL,"
        "FOREIGN KEY(id_commande) REFERENCES commandes(id),"
        "FOREIGN KEY(id_produit) REFERENCES produits(id)"
        ");"

        "CREATE TABLE IF NOT EXISTS reclamations ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "id_commande INTEGER,"
        "client TEXT,"
        "type TEXT,"
        "description TEXT,"
        "date TEXT,"
        "statut TEXT DEFAULT 'Nouveau',"
        "FOREIGN KEY(id_commande) REFERENCES commandes(id)"
        ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if(rc != SQLITE_OK)
    {
        wxLogError("Erreur creation tables: %s", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DatabaseManager::ExecuteSQL(const wxString& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.mb_str(), nullptr, nullptr, &errMsg);

    if(rc != SQLITE_OK)
    {
        wxLogError("Erreur SQL: %s", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DatabaseManager::AddProduct(const wxString& nom, const wxString& description,
                                 const wxString& categorie, double prix, int stock)
{
    wxString sql = wxString::Format(
        "INSERT INTO produits (nom, description, categorie, prix, stock) "
        "VALUES ('%s', '%s', '%s', %.2f, %d);",
        nom, description, categorie, prix, stock
    );

    return ExecuteSQL(sql);
}

bool DatabaseManager::UpdateProduct(int id, const wxString& nom, const wxString& description,
                                    const wxString& categorie, double prix, int stock)
{
    wxString sql = wxString::Format(
        "UPDATE produits SET nom='%s', description='%s', categorie='%s', "
        "prix=%.2f, stock=%d WHERE id=%d;",
        nom, description, categorie, prix, stock, id
    );

    return ExecuteSQL(sql);
}

bool DatabaseManager::DeleteProduct(int id)
{
    wxString sql = wxString::Format("UPDATE produits SET actif=0 WHERE id=%d;", id);
    return ExecuteSQL(sql);
}


std::vector<Product> DatabaseManager::GetAllProducts()
{
    std::vector<Product> products;

    if(!m_db)
    {
        wxLogError("Base de donnees non initialisee");
        return products;
    }

    const char* sql = "SELECT id, nom, description, categorie, prix, stock, actif "
                     "FROM produits WHERE actif=1;";

    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if(rc != SQLITE_OK)
    {
        wxLogError("Erreur preparation requete: %s", sqlite3_errmsg(m_db));
        return products;
    }

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        Product p;
        p.id = sqlite3_column_int(stmt, 0);
        p.nom = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 1));
        p.description = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
        p.categorie = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 3));
        p.prix = sqlite3_column_double(stmt, 4);
        p.stock = sqlite3_column_int(stmt, 5);
        p.actif = sqlite3_column_int(stmt, 6) == 1;

        products.push_back(p);
    }

    sqlite3_finalize(stmt);

    // Utiliser wxLogDebug pour éviter les dialogues modaux qui interrompent l'execution
    wxLogDebug("Charge %zu produits depuis la base de donnees", products.size());

    return products;
}








bool DatabaseManager::UpdateStock(int id_produit, int nouvelle_quantite)
{
    wxString sql = wxString::Format(
        "UPDATE produits SET stock=%d WHERE id=%d;",
        nouvelle_quantite, id_produit
    );

    return ExecuteSQL(sql);
}

int DatabaseManager::CreateOrder(int id_client, const wxString& adresse, double montant_total)
{
    wxDateTime now = wxDateTime::Now();
    wxString date = now.Format("%Y-%m-%d %H:%M:%S");
    wxString tracking = wxString::Format("TRK-2026-%03d", (int)(wxGetLocalTime() % 1000));

    wxString sql = wxString::Format(
        "INSERT INTO commandes (id_client, date, montant_total, adresse, numero_suivi, statut) "
        "VALUES (%d, '%s', %.2f, '%s', '%s', 'En attente');",
        id_client, date, montant_total, adresse, tracking
    );

    if(ExecuteSQL(sql))
    {
        return (int)sqlite3_last_insert_rowid(m_db);
    }

    return -1;
}

bool DatabaseManager::AddOrderLine(int id_commande, int id_produit, int quantite, double prix_unitaire)
{
    Product p = GetProduct(id_produit);

    wxString sql = wxString::Format(
        "INSERT INTO lignes_commande (id_commande, id_produit, nom_produit, quantite, prix_unitaire) "
        "VALUES (%d, %d, '%s', %d, %.2f);",
        id_commande, id_produit, p.nom, quantite, prix_unitaire
    );

    if(ExecuteSQL(sql))
    {
        int nouveau_stock = p.stock - quantite;
        return UpdateStock(id_produit, nouveau_stock);
    }

    return false;
}

Product DatabaseManager::GetProduct(int id)
{
    Product p;
    p.id = -1;

    wxString sql = wxString::Format(
        "SELECT id, nom, description, categorie, prix, stock, actif "
        "FROM produits WHERE id=%d;", id
    );

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql.mb_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            p.id = sqlite3_column_int(stmt, 0);
            p.nom = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 1));
            p.description = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
            p.categorie = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 3));
            p.prix = sqlite3_column_double(stmt, 4);
            p.stock = sqlite3_column_int(stmt, 5);
            p.actif = sqlite3_column_int(stmt, 6) == 1;
        }
    }

    sqlite3_finalize(stmt);
    return p;
}

std::vector<Order> DatabaseManager::GetPendingDeliveries()
{
    std::vector<Order> orders;

    const char* sql = "SELECT id, id_client, date, montant_total, statut, adresse, numero_suivi "
                     "FROM commandes WHERE statut IN ('En attente', 'En cours');";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Order o;
            o.id = sqlite3_column_int(stmt, 0);
            o.id_client = sqlite3_column_int(stmt, 1);
            o.date = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
            o.montant_total = sqlite3_column_double(stmt, 3);
            o.statut = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 4));
            o.adresse = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 5));
            o.numero_suivi = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 6));

            orders.push_back(o);
        }
    }

    sqlite3_finalize(stmt);
    return orders;
}

std::vector<OrderLine> DatabaseManager::GetOrderLines(int id_commande)
{
    std::vector<OrderLine> lines;

    wxString sql = wxString::Format(
        "SELECT id, id_commande, id_produit, nom_produit, quantite, prix_unitaire "
        "FROM lignes_commande WHERE id_commande=%d;", id_commande
    );

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql.mb_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            OrderLine line;
            line.id = sqlite3_column_int(stmt, 0);
            line.id_commande = sqlite3_column_int(stmt, 1);
            line.id_produit = sqlite3_column_int(stmt, 2);
            line.nom_produit = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 3));
            line.quantite = sqlite3_column_int(stmt, 4);
            line.prix_unitaire = sqlite3_column_double(stmt, 5);

            lines.push_back(line);
        }
    }

    sqlite3_finalize(stmt);
    return lines;
}

bool DatabaseManager::UpdateOrderStatus(int id_commande, const wxString& nouveau_statut)
{
    wxString sql = wxString::Format(
        "UPDATE commandes SET statut='%s' WHERE id=%d;",
        nouveau_statut, id_commande
    );

    return ExecuteSQL(sql);
}

int DatabaseManager::CreateReclamation(int id_commande, const wxString& client,
                                       const wxString& type, const wxString& description)
{
    wxDateTime now = wxDateTime::Now();
    wxString date = now.Format("%Y-%m-%d");

    wxString sql = wxString::Format(
        "INSERT INTO reclamations (id_commande, client, type, description, date) "
        "VALUES (%d, '%s', '%s', '%s', '%s');",
        id_commande, client, type, description, date
    );

    if(ExecuteSQL(sql))
    {
        return (int)sqlite3_last_insert_rowid(m_db);
    }

    return -1;
}

std::vector<Reclamation> DatabaseManager::GetAllReclamations()
{
    std::vector<Reclamation> reclamations;

    const char* sql = "SELECT id, id_commande, client, type, description, date, statut "
                     "FROM reclamations ORDER BY id DESC;";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Reclamation r;
            r.id = sqlite3_column_int(stmt, 0);
            r.id_commande = sqlite3_column_int(stmt, 1);
            r.client = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
            r.type = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 3));
            r.description = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 4));
            r.date = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 5));
            r.statut = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 6));

            reclamations.push_back(r);
        }
    }

    sqlite3_finalize(stmt);
    return reclamations;
}

bool DatabaseManager::UpdateReclamationStatus(int id, const wxString& statut)
{
    wxString sql = wxString::Format(
        "UPDATE reclamations SET statut='%s' WHERE id=%d;",
        statut, id
    );

    return ExecuteSQL(sql);
}

std::vector<Order> DatabaseManager::GetAllOrders()
{
    std::vector<Order> orders;

    const char* sql = "SELECT id, id_client, date, montant_total, statut, adresse, numero_suivi "
                     "FROM commandes ORDER BY id DESC;";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Order o;
            o.id = sqlite3_column_int(stmt, 0);
            o.id_client = sqlite3_column_int(stmt, 1);
            o.date = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
            o.montant_total = sqlite3_column_double(stmt, 3);
            o.statut = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 4));
            o.adresse = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 5));
            o.numero_suivi = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 6));

            orders.push_back(o);
        }
    }

    sqlite3_finalize(stmt);
    return orders;
}

bool DatabaseManager::CreateDelivery(int id_commande, const wxString& numero_suivi)
{
    wxString sql = wxString::Format(
        "UPDATE commandes SET numero_suivi='%s', statut='En cours' WHERE id=%d;",
        numero_suivi, id_commande
    );

    return ExecuteSQL(sql);
}

bool DatabaseManager::AssignDelivery(int id_commande, int id_livreur)
{
    return UpdateOrderStatus(id_commande, "Assignee");
}

bool DatabaseManager::UpdateDeliveryStatus(int id_commande, const wxString& statut)
{
    return UpdateOrderStatus(id_commande, statut);
}

std::vector<Order> DatabaseManager::GetOrdersByStatus(const wxString& statut)
{
    std::vector<Order> orders;

    wxString sql = wxString::Format(
        "SELECT id, id_client, date, montant_total, statut, adresse, numero_suivi "
        "FROM commandes WHERE statut='%s' ORDER BY id DESC;", statut
    );

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(m_db, sql.mb_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Order o;
            o.id = sqlite3_column_int(stmt, 0);
            o.id_client = sqlite3_column_int(stmt, 1);
            o.date = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 2));
            o.montant_total = sqlite3_column_double(stmt, 3);
            o.statut = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 4));
            o.adresse = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 5));
            o.numero_suivi = wxString::FromUTF8((const char*)sqlite3_column_text(stmt, 6));

            orders.push_back(o);
        }
    }

    sqlite3_finalize(stmt);
    return orders;
}

bool DatabaseManager::InitializeSampleData()
{
    // Vérifier si des produits existent déjà
    std::vector<Product> existing = GetAllProducts();
    if(!existing.empty())
    {
        // Déjà initialisé
        return true;
    }


    // AJOUTER DES COMMANDES DE DÉMONSTRATION
    int cmd1 = CreateOrder(1, "Quartier Agla, Rue 123, Cotonou", 535000);
    if(cmd1 > 0)
    {
        AddOrderLine(cmd1, 1, 1, 450000);  // 1 Samsung
        AddOrderLine(cmd1, 3, 2, 35000);   // 2 Ecouteurs Sony
    }

    int cmd2 = CreateOrder(2, "Akpakpa, Avenue des Palmiers, Cotonou", 40000);
    if(cmd2 > 0)
    {
        AddOrderLine(cmd2, 5, 2, 15000);   // 2 T-shirts
    }

    // AJOUTER DES RÉCLAMATIONS DE DÉMONSTRATION
    CreateReclamation(cmd1, "Client Test", "Produit defectueux",
                     "Le smartphone ne s'allume plus apres 2 jours");
    CreateReclamation(cmd2, "AHOUANSOU Olivier", "Livraison retard",
                     "Commande passee il y a 5 jours, toujours pas recue");




    // AJOUTER DES PRODUITS DE DÉMONSTRATION
    AddProduct("Smartphone Samsung Galaxy S23",
               "Ecran 6.1 pouces, 128GB, 5G, Camera 50MP",
               "Electronique", 450000, 15);

    AddProduct("MacBook Pro 14 M3",
               "Puce M3, 16GB RAM, 512GB SSD, Ecran Retina",
               "Electronique", 1200000, 8);

    AddProduct("Ecouteurs Sony WH-1000XM5",
               "Reduction de bruit active, Bluetooth, 30h autonomie",
               "Electronique", 185000, 45);

    AddProduct("iPhone 15 Pro Max",
               "Ecran 6.7 pouces, 256GB, Titane, Camera 48MP",
               "Electronique", 850000, 12);

    AddProduct("T-shirt Nike Dri-FIT",
               "100% coton, respirant, plusieurs tailles disponibles",
               "Vetements", 15000, 100);

    AddProduct("Jean Levi's 501",
               "Coupe classique, denim premium, resistant",
               "Vetements", 25000, 60);

    AddProduct("Chaussures Adidas Ultraboost",
               "Running, amorti Boost, confort maximal",
               "Vetements", 75000, 30);

    AddProduct("Robe elegante",
               "Pour soirees, tissu satin, couleurs variees",
               "Vetements", 35000, 40);

    AddProduct("Riz parfume 25kg",
               "Riz thailandais de qualite superieure",
               "Alimentation", 18000, 200);

    AddProduct("Huile vegetale 5L",
               "Huile de tournesol 100% naturelle",
               "Alimentation", 8000, 150);

    AddProduct("Pack eau minerale 12x1.5L",
               "Eau de source pure",
               "Alimentation", 3500, 300);

    AddProduct("Cafe Nescafe Gold 200g",
               "Cafe soluble premium",
               "Alimentation", 4500, 80);

    AddProduct("Canape 3 places en cuir",
               "Confortable, moderne, garantie 2 ans",
               "Maison", 185000, 12);

    AddProduct("Table a manger 6 personnes",
               "Bois massif, design elegant",
               "Maison", 95000, 20);

    AddProduct("Refrigerateur Samsung 350L",
               "Classe energetique A++, No Frost",
               "Maison", 320000, 8);

    AddProduct("Televiseur LG 55 pouces 4K",
               "Smart TV, HDR, WebOS",
               "Electronique", 380000, 15);

    AddProduct("Ballon de football Adidas",
               "Taille 5, officiel",
               "Sports", 12000, 50);

    AddProduct("Raquette de tennis Wilson",
               "Professionnelle, graphite",
               "Sports", 45000, 25);

    AddProduct("Tapis de yoga premium",
               "Anti-derapant, 6mm epaisseur",
               "Sports", 18000, 40);

    AddProduct("Velo VTT 26 pouces",
               "21 vitesses, suspension avant",
               "Sports", 125000, 10);

    wxLogDebug("Base de donnees initialisee avec 20 produits de demonstration");
    return true;




}



