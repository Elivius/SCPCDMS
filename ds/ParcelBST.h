#pragma once
// BST of parcels keyed on Parcel ID. Owner: Member E
// Used by: Task 5 ParcelCategory - the system's one parcel registry; every module looks parcels up here.
// Agreed interface: insert, search, remove, update, inOrder, isEmpty
// Extras: preOrder (shows tree shape), size, height. Post-order is used by the destructor.
//
// Records are stored by value inside nodes, so callers must refer to a parcel by its ID and
// look it up again; a pointer from search() is only valid until the next insert/remove.
// No STL containers: the tree is built from raw nodes. Copying is disabled (the tree owns raw memory).

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

    // Recursive helpers (each works on one subtree)
    static Node* insertAt(Node* node, const Parcel& parcel, bool& inserted);
    static Node* removeAt(Node* node, const std::string& id, bool& removed);
    static Node* minNode(Node* node);
    static void  destroy(Node* node);                 // post-order: children before parent
    static int   heightAt(const Node* node);

    template <typename Visitor>
    static void inOrderAt(const Node* node, Visitor& visit)    // left, node, right -> sorted by ID
    {
        if (node == nullptr) return;
        inOrderAt(node->left, visit);
        visit(node->data);
        inOrderAt(node->right, visit);
    }

    template <typename Visitor>
    static void preOrderAt(const Node* node, int depth, Visitor& visit)   // node, left, right
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
    int  height() const;                              // 0 for an empty tree

    // false when a parcel with the same ID already exists
    bool insert(const Parcel& parcel);

    // nullptr when not found
    const Parcel* search(const std::string& parcelId) const;

    // false when not found. Handles leaf, one-child, two-child and root nodes.
    bool remove(const std::string& parcelId);

    // Replaces every field except the key. false when the ID does not exist.
    bool update(const Parcel& parcel);

    // visit(const Parcel&) for each parcel in ascending Parcel ID order
    template <typename Visitor>
    void inOrder(Visitor visit) const { inOrderAt(root_, visit); }

    // visit(const Parcel&, int depth) parent-first; depth 0 is the root
    template <typename Visitor>
    void preOrder(Visitor visit) const { preOrderAt(root_, 0, visit); }
};
