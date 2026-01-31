/***************************************************************
 * Name:      CartManager.cpp
 * Purpose:   Implémentation du gestionnaire de panier
 * Author:    EGOUDJOBI Peace, HOUNGUEVOU Blandine, AHOUANSOU Olivier
 * Created:   2026-01-28
 **************************************************************/

#include "CartManager.h"

// Retourne l'instance unique (création à la première utilisation)
CartManager& CartManager::GetInstance()
{
    static CartManager instance;
    return instance;
}

CartManager::CartManager()
{
    // Constructeur vide - le panier démarre vide
}

CartManager::~CartManager()
{
    // Destructeur - nettoyage automatique
}

void CartManager::AddProduct(int id_produit, const wxString& nom, double prix, int quantite)
{
    // Vérifier si le produit est déjà dans le panier
    if(m_items.find(id_produit) != m_items.end())
    {
        // Produit déjà présent : augmenter la quantité
        m_items[id_produit].quantite += quantite;
    }
    else
    {
        // Nouveau produit : créer l'entrée
        CartItem item;
        item.id_produit = id_produit;
        item.nom_produit = nom;
        item.prix_unitaire = prix;
        item.quantite = quantite;
        m_items[id_produit] = item;
    }
}

void CartManager::RemoveProduct(int id_produit)
{
    // Supprimer le produit du panier
    m_items.erase(id_produit);
}

void CartManager::UpdateQuantity(int id_produit, int nouvelle_quantite)
{
    // Vérifier que le produit existe
    if(m_items.find(id_produit) != m_items.end())
    {
        if(nouvelle_quantite <= 0)
        {
            // Quantité nulle ou négative : supprimer l'article
            RemoveProduct(id_produit);
        }
        else
        {
            // Mettre à jour la quantité
            m_items[id_produit].quantite = nouvelle_quantite;
        }
    }
}

void CartManager::ClearCart()
{
    // Vider complètement le panier
    m_items.clear();
}

std::vector<CartItem> CartManager::GetAllItems() const
{
    std::vector<CartItem> items;
    // Convertir la map en vector
    for(const auto& pair : m_items)
    {
        items.push_back(pair.second);
    }
    return items;
}

int CartManager::GetItemCount() const
{
    int count = 0;
    // Compter toutes les quantités
    for(const auto& pair : m_items)
    {
        count += pair.second.quantite;
    }
    return count;
}

double CartManager::GetTotal() const
{
    double total = 0.0;
    // Additionner tous les sous-totaux
    for(const auto& pair : m_items)
    {
        total += pair.second.GetTotal();
    }
    return total;
}

bool CartManager::IsEmpty() const
{
    return m_items.empty();
}

bool CartManager::HasProduct(int id_produit) const
{
    return m_items.find(id_produit) != m_items.end();
}
