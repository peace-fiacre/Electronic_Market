/***************************************************************
 * Name:      CartManager.h
 * Purpose:   Gestion du panier d'achat (Singleton)
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-28
 **************************************************************/

#ifndef CARTMANAGER_H
#define CARTMANAGER_H

#include <wx/wx.h>
#include <vector>
#include <map>

// Structure pour un article dans le panier
struct CartItem {
    int id_produit;
    wxString nom_produit;
    double prix_unitaire;
    int quantite;

    // Calcule le total pour cet article
    double GetTotal() const {
        return prix_unitaire * quantite;
    }
};

// Gestionnaire singleton du panier
class CartManager
{
public:
    // Récupérer l'instance unique (Singleton)
    static CartManager& GetInstance();

    // === GESTION DU PANIER ===
    // Ajouter un produit (ou augmenter quantité si déjà présent)
    void AddProduct(int id_produit, const wxString& nom, double prix, int quantite = 1);

    // Retirer complètement un produit
    void RemoveProduct(int id_produit);

    // Modifier la quantité d'un produit
    void UpdateQuantity(int id_produit, int nouvelle_quantite);

    // Vider tout le panier
    void ClearCart();

    // === CONSULTATION ===
    // Obtenir tous les articles
    std::vector<CartItem> GetAllItems() const;

    // Nombre total d'articles (somme des quantités)
    int GetItemCount() const;

    // Montant total du panier
    double GetTotal() const;

    // Vérifier si le panier est vide
    bool IsEmpty() const;

    // Vérifier si un produit est dans le panier
    bool HasProduct(int id_produit) const;

private:
    // Constructeur privé (Singleton)
    CartManager();
    ~CartManager();

    // Empêcher la copie
    CartManager(const CartManager&) = delete;
    CartManager& operator=(const CartManager&) = delete;

    // Données : Map qui associe id_produit → CartItem
    std::map<int, CartItem> m_items;
};

#endif // CARTMANAGER_H
