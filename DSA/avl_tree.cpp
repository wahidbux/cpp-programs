/**
 * AVL Tree Implementation
 * 
 * An AVL (Adelson-Velsky and Landis) tree is a self-balancing Binary Search Tree (BST)
 * where the difference between heights of left and right subtrees cannot be more than one
 * for all nodes. This ensures O(log n) time complexity for insertion, deletion, and search.
 * 
 * Features:
 * - Self-balancing through rotations
 * - Insertion with automatic rebalancing
 * - Deletion with automatic rebalancing
 * - Inorder, Preorder, and Postorder traversals
 * - Height and balance factor calculations
 * - Minimum and maximum value finding
 * 
 * Time Complexity:
 * - Search: O(log n)
 * - Insert: O(log n)
 * - Delete: O(log n)
 * - Traversal: O(n)
 * 
 * Space Complexity: O(n) for storing n nodes
 * 
 * Author: Contribution for Hacktoberfest 2025
 * Date: October 1, 2025
 */

#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

// Node structure for AVL Tree
struct Node {
    int data;           // Value stored in the node
    Node* left;         // Pointer to left child
    Node* right;        // Pointer to right child
    int height;         // Height of the node in the tree
    
    // Constructor to initialize a new node
    Node(int value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;  // Root node of the AVL tree
    
    /**
     * Get the height of a node
     * Returns 0 if node is NULL
     */
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }
    
    /**
     * Calculate balance factor of a node
     * Balance Factor = Height(Left Subtree) - Height(Right Subtree)
     * A balanced node has balance factor of -1, 0, or 1
     */
    int getBalanceFactor(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    /**
     * Update the height of a node based on its children
     */
    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }
    
    /**
     * Right Rotation
     * 
     *       y                               x
     *      / \     Right Rotation          / \
     *     x   T3   – – – – – – – >        T1  y
     *    / \                                  / \
     *   T1  T2                               T2  T3
     * 
     * Used when left subtree is heavier
     */
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        // Perform rotation
        x->right = y;
        y->left = T2;
        
        // Update heights
        updateHeight(y);
        updateHeight(x);
        
        return x;  // New root
    }
    
    /**
     * Left Rotation
     * 
     *     x                               y
     *    / \      Left Rotation          / \
     *   T1  y     – – – – – – – >       x   T3
     *      / \                          / \
     *     T2  T3                       T1  T2
     * 
     * Used when right subtree is heavier
     */
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        // Perform rotation
        y->left = x;
        x->right = T2;
        
        // Update heights
        updateHeight(x);
        updateHeight(y);
        
        return y;  // New root
    }
    
    /**
     * Balance the node after insertion or deletion
     * Checks balance factor and performs appropriate rotations
     */
    Node* balance(Node* node) {
        // Update height of current node
        updateHeight(node);
        
        // Get balance factor
        int balanceFactor = getBalanceFactor(node);
        
        // Left-Left Case (Right Rotation)
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Left-Right Case (Left-Right Rotation)
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right-Right Case (Left Rotation)
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }
        
        // Right-Left Case (Right-Left Rotation)
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;  // Node is already balanced
    }
    
    /**
     * Insert a new value into the AVL tree
     * Recursively inserts and balances the tree
     */
    Node* insertHelper(Node* node, int value) {
        // Perform standard BST insertion
        if (!node) {
            return new Node(value);
        }
        
        if (value < node->data) {
            node->left = insertHelper(node->left, value);
        } else if (value > node->data) {
            node->right = insertHelper(node->right, value);
        } else {
            // Duplicate values not allowed
            return node;
        }
        
        // Balance the node and return
        return balance(node);
    }
    
    /**
     * Find the node with minimum value in a tree
     * Used in deletion operation
     */
    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }
    
    /**
     * Delete a value from the AVL tree
     * Recursively deletes and balances the tree
     */
    Node* deleteHelper(Node* node, int value) {
        if (!node) {
            return nullptr;
        }
        
        // Perform standard BST deletion
        if (value < node->data) {
            node->left = deleteHelper(node->left, value);
        } else if (value > node->data) {
            node->right = deleteHelper(node->right, value);
        } else {
            // Node to be deleted found
            
            // Case 1: Node with only one child or no child
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                
                if (!temp) {  // No child case
                    temp = node;
                    node = nullptr;
                } else {  // One child case
                    *node = *temp;  // Copy the contents
                }
                delete temp;
            } else {
                // Case 2: Node with two children
                // Get inorder successor (smallest in right subtree)
                Node* temp = findMin(node->right);
                
                // Copy successor's data to this node
                node->data = temp->data;
                
                // Delete the successor
                node->right = deleteHelper(node->right, temp->data);
            }
        }
        
        if (!node) {
            return nullptr;
        }
        
        // Balance the node and return
        return balance(node);
    }
    
    /**
     * Search for a value in the tree
     * Returns true if found, false otherwise
     */
    bool searchHelper(Node* node, int value) {
        if (!node) {
            return false;
        }
        
        if (value == node->data) {
            return true;
        } else if (value < node->data) {
            return searchHelper(node->left, value);
        } else {
            return searchHelper(node->right, value);
        }
    }
    
    /**
     * Inorder Traversal (Left-Root-Right)
     * Prints values in sorted order
     */
    void inorderHelper(Node* node) {
        if (node) {
            inorderHelper(node->left);
            cout << node->data << " ";
            inorderHelper(node->right);
        }
    }
    
    /**
     * Preorder Traversal (Root-Left-Right)
     */
    void preorderHelper(Node* node) {
        if (node) {
            cout << node->data << " ";
            preorderHelper(node->left);
            preorderHelper(node->right);
        }
    }
    
    /**
     * Postorder Traversal (Left-Right-Root)
     */
    void postorderHelper(Node* node) {
        if (node) {
            postorderHelper(node->left);
            postorderHelper(node->right);
            cout << node->data << " ";
        }
    }
    
    /**
     * Level Order Traversal (Breadth-First Search)
     * Prints tree level by level
     */
    void levelOrderHelper(Node* node) {
        if (!node) return;
        
        queue<Node*> q;
        q.push(node);
        
        while (!q.empty()) {
            int levelSize = q.size();
            
            for (int i = 0; i < levelSize; i++) {
                Node* current = q.front();
                q.pop();
                cout << current->data << " ";
                
                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            cout << endl;
        }
    }
    
    /**
     * Display tree structure with indentation
     * Shows the hierarchical structure of the tree
     */
    void displayHelper(Node* node, int space, int indent) {
        if (!node) return;
        
        space += indent;
        displayHelper(node->right, space, indent);
        
        cout << endl;
        for (int i = indent; i < space; i++) {
            cout << " ";
        }
        cout << node->data << "(" << getHeight(node) << ")" << endl;
        
        displayHelper(node->left, space, indent);
    }
    
    /**
     * Destroy the tree recursively (cleanup)
     */
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    // Constructor
    AVLTree() : root(nullptr) {}
    
    // Destructor
    ~AVLTree() {
        destroyTree(root);
    }
    
    /**
     * Public interface to insert a value
     */
    void insert(int value) {
        root = insertHelper(root, value);
        cout << "Inserted " << value << " into the AVL tree." << endl;
    }
    
    /**
     * Public interface to delete a value
     */
    void remove(int value) {
        root = deleteHelper(root, value);
        cout << "Deleted " << value << " from the AVL tree." << endl;
    }
    
    /**
     * Public interface to search for a value
     */
    bool search(int value) {
        return searchHelper(root, value);
    }
    
    /**
     * Public interface for inorder traversal
     */
    void inorder() {
        cout << "Inorder Traversal: ";
        inorderHelper(root);
        cout << endl;
    }
    
    /**
     * Public interface for preorder traversal
     */
    void preorder() {
        cout << "Preorder Traversal: ";
        preorderHelper(root);
        cout << endl;
    }
    
    /**
     * Public interface for postorder traversal
     */
    void postorder() {
        cout << "Postorder Traversal: ";
        postorderHelper(root);
        cout << endl;
    }
    
    /**
     * Public interface for level order traversal
     */
    void levelOrder() {
        cout << "Level Order Traversal:" << endl;
        levelOrderHelper(root);
    }
    
    /**
     * Display tree structure
     */
    void display() {
        cout << "\nTree Structure (value(height)):" << endl;
        displayHelper(root, 0, 5);
        cout << endl;
    }
    
    /**
     * Get the height of the tree
     */
    int height() {
        return getHeight(root);
    }
    
    /**
     * Check if tree is empty
     */
    bool isEmpty() {
        return root == nullptr;
    }
};

/**
 * Main function demonstrating AVL Tree operations
 */
int main() {
    cout << "========================================" << endl;
    cout << "   AVL Tree Implementation in C++      " << endl;
    cout << "========================================" << endl << endl;
    
    AVLTree tree;
    
    // Insert elements
    cout << "--- Insertion Operations ---" << endl;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    tree.insert(10);
    tree.insert(25);
    tree.insert(35);
    
    cout << "\nTree height: " << tree.height() << endl;
    
    // Display tree structure
    tree.display();
    
    // Traversals
    cout << "\n--- Tree Traversals ---" << endl;
    tree.inorder();
    tree.preorder();
    tree.postorder();
    cout << endl;
    tree.levelOrder();
    
    // Search operations
    cout << "\n--- Search Operations ---" << endl;
    int searchValues[] = {25, 35, 100};
    for (int val : searchValues) {
        if (tree.search(val)) {
            cout << val << " found in the tree." << endl;
        } else {
            cout << val << " not found in the tree." << endl;
        }
    }
    
    // Delete operations
    cout << "\n--- Deletion Operations ---" << endl;
    tree.remove(20);
    tree.display();
    
    tree.remove(30);
    tree.display();
    
    tree.remove(50);
    tree.display();
    
    cout << "\nFinal tree height: " << tree.height() << endl;
    tree.inorder();
    
    cout << "\n========================================" << endl;
    cout << "   Program completed successfully!     " << endl;
    cout << "========================================" << endl;
    
    return 0;
}
