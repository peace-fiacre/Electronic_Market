/***************************************************************
 * Name:      ElectronicMarketApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-16
 **************************************************************/



#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <wx/wx.h>
#include <sqlite3.h>
#include <vector>

struct Product {
    int id;
    wxString nom;
    wxString description;
    wxString categorie;
    double prix;
    int stock;
    bool actif;
};

struct Order {
    int id;
    int id_client;
    wxString date;
    double montant_total;
    wxString statut;
    wxString adresse;
    wxString numero_suivi;
};

struct OrderLine {
    int id;
    int id_commande;
    int id_produit;
    wxString nom_produit;
    int quantite;
    double prix_unitaire;
};

struct Reclamation {
    int id;
    int id_commande;
    wxString client;
    wxString type;
    wxString description;
    wxString date;
    wxString statut;
};

class DatabaseManager
{
public:
    static DatabaseManager& GetInstance();

    bool Initialize();
    bool InitializeSampleData();
    void Close();

    // PRODUITS
    bool AddProduct(const wxString& nom, const wxString& description,
                   const wxString& categorie, double prix, int stock);
    bool UpdateProduct(int id, const wxString& nom, const wxString& description,
                      const wxString& categorie, double prix, int stock);
    bool DeleteProduct(int id);
    std::vector<Product> GetAllProducts();
    Product GetProduct(int id);
    bool UpdateStock(int id_produit, int nouvelle_quantite);

    // COMMANDES
    int CreateOrder(int id_client, const wxString& adresse, double montant_total);
    bool AddOrderLine(int id_commande, int id_produit, int quantite, double prix_unitaire);
    std::vector<Order> GetAllOrders();
    std::vector<Order> GetOrdersByStatus(const wxString& statut);
    std::vector<OrderLine> GetOrderLines(int id_commande);
    bool UpdateOrderStatus(int id_commande, const wxString& nouveau_statut);

    // LIVRAISONS
    bool CreateDelivery(int id_commande, const wxString& numero_suivi);
    bool AssignDelivery(int id_commande, int id_livreur);
    bool UpdateDeliveryStatus(int id_commande, const wxString& statut);
    std::vector<Order> GetPendingDeliveries();

    // RÉCLAMATIONS
    int CreateReclamation(int id_commande, const wxString& client,
                         const wxString& type, const wxString& description);
    std::vector<Reclamation> GetAllReclamations();
    bool UpdateReclamationStatus(int id, const wxString& statut);

private:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    sqlite3* m_db;
    wxString m_dbPath;

    bool CreateTables();
    bool ExecuteSQL(const wxString& sql);
};

#endif
