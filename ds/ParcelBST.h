#pragma once
// BST of parcels keyed on Parcel ID (Task 5).

#include <string>
#include "../Models.h"

class ParcelBST {
private:
    struct Node {
        Parcel data;
        Node*  left  = nullptr;
        Node*  right = nullptr;
        explicit Node(const Parcel& p) : data(p) {}
    };

    Node* root_ = nullptr;
    int   count_ = 0;

    static Node* insertAt(Node* node, const Parcel& parcel, bool& inserted);
    static Node* removeAt(Node* node, const std::string& id, bool& removed);
    static Node* minNode(Node* node);
    static void  destroy(Node* node);
    static int   heightAt(const Node* node);

    template <typename Visitor>
    static void inOrderAt(const Node* node, Visitor& visit)
    {
        if (node == nullptr) return;
        inOrderAt(node->left, visit);
        visit(node->data);
        inOrderAt(node->right, visit);
    }

    template <typename Visitor>
    static void preOrderAt(const Node* node, int depth, Visitor& visit)
    {
        if (node == nullptr) return;
        visit(node->data, depth);
        preOrderAt(node->left, depth + 1, visit);
        preOrderAt(node->right, depth + 1, visit);
    }

public:
    ParcelBST() = default;
    ~ParcelBST();
    ParcelBST(const ParcelBST&) = delete;
    ParcelBST& operator=(const ParcelBST&) = delete;

    bool isEmpty() const { return root_ == nullptr; }
    int  size() const    { return count_; }
    int  height() const;

    bool insert(const Parcel& parcel);

    // nullptr if not found
    const Parcel* search(const std::string& parcelId) const;

    bool remove(const std::string& parcelId);

    // Replaces all fields except the ID
    bool update(const Parcel& parcel);

    // Ascending Parcel ID order
    template <typename Visitor>
    void inOrder(Visitor visit) const { inOrderAt(root_, visit); }

    // Parent before children, with depth
    template <typename Visitor>
    void preOrder(Visitor visit) const { preOrderAt(root_, 0, visit); }
};
