#include "ParcelBST.h"

ParcelBST::~ParcelBST()
{
    destroy(root_);
}

void ParcelBST::destroy(Node* node)
{
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

int ParcelBST::heightAt(const Node* node)
{
    if (node == nullptr) return 0;
    int l = heightAt(node->left);
    int r = heightAt(node->right);
    return 1 + (l > r ? l : r);
}

int ParcelBST::height() const
{
    return heightAt(root_);
}

ParcelBST::Node* ParcelBST::insertAt(Node* node, const Parcel& parcel, bool& inserted)
{
    if (node == nullptr) {
        inserted = true;
        return new Node(parcel);
    }
    if (parcel.parcelId < node->data.parcelId)
        node->left = insertAt(node->left, parcel, inserted);
    else if (parcel.parcelId > node->data.parcelId)
        node->right = insertAt(node->right, parcel, inserted);
    // equal key: leave the tree unchanged, inserted stays false
    return node;
}

bool ParcelBST::insert(const Parcel& parcel)
{
    bool inserted = false;
    root_ = insertAt(root_, parcel, inserted);
    if (inserted) ++count_;
    return inserted;
}

const Parcel* ParcelBST::search(const std::string& parcelId) const
{
    const Node* cur = root_;
    while (cur != nullptr) {
        if (parcelId == cur->data.parcelId) return &cur->data;
        cur = (parcelId < cur->data.parcelId) ? cur->left : cur->right;
    }
    return nullptr;
}

ParcelBST::Node* ParcelBST::minNode(Node* node)
{
    while (node->left != nullptr) node = node->left;
    return node;
}

ParcelBST::Node* ParcelBST::removeAt(Node* node, const std::string& id, bool& removed)
{
    if (node == nullptr) return nullptr;

    if (id < node->data.parcelId) {
        node->left = removeAt(node->left, id, removed);
    } else if (id > node->data.parcelId) {
        node->right = removeAt(node->right, id, removed);
    } else {
        removed = true;
        if (node->left == nullptr) {                  // leaf or only a right child
            Node* child = node->right;
            delete node;
            return child;
        }
        if (node->right == nullptr) {                 // only a left child
            Node* child = node->left;
            delete node;
            return child;
        }
        // Two children: copy in the in-order successor, then delete the successor from the right subtree.
        Node* successor = minNode(node->right);
        node->data = successor->data;
        bool ignored = false;
        node->right = removeAt(node->right, successor->data.parcelId, ignored);
    }
    return node;
}

bool ParcelBST::remove(const std::string& parcelId)
{
    bool removed = false;
    root_ = removeAt(root_, parcelId, removed);
    if (removed) --count_;
    return removed;
}

bool ParcelBST::update(const Parcel& parcel)
{
    Node* cur = root_;
    while (cur != nullptr) {
        if (parcel.parcelId == cur->data.parcelId) {
            cur->data = parcel;                       // key is identical, so BST order is untouched
            return true;
        }
        cur = (parcel.parcelId < cur->data.parcelId) ? cur->left : cur->right;
    }
    return false;
}
