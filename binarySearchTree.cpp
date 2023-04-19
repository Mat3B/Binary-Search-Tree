/*
 * Name: Mathew Breland 
 * Date Submitted: 3/25/2023
 * Lab Section: 001
 * Assignment Name: Lab 7 - Binary search tree
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "binarySearchTree.h"

using namespace std;

void fix_size(Node* T)
{
    T->size = 1;
    if (T->left) T->size += T->left->size;
    if (T->right) T->size += T->right->size;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node* insert(Node* T, int k)
{
    if (T == nullptr) return new Node(k);
    if (k < T->key) T->left = insert(T->left, k);
    else T->right = insert(T->right, k);
    fix_size(T);
    return T;
}

// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node* T)
{
    vector<int> inorder;
    vector<int> rhs;
    if (T == nullptr) return inorder;
    inorder = inorder_traversal(T->left);
    inorder.push_back(T->key);
    rhs = inorder_traversal(T->right);
    inorder.insert(inorder.end(), rhs.begin(), rhs.end());
    return inorder;
}

// return a pointer to the node with key k in tree T, or nullptr if it doesn't exist
Node* find(Node* T, int k)
{
    if (T == nullptr || T->key == k) {
        // if T is nullptr (i.e., reached the end of the tree) or T's key matches k, return T
        return T;
    }
    else if (k < T->key) {
        // if k is less than T's key, search the left subtree
        return find(T->left, k);
    }
    else {
        // if k is greater than T's key, search the right subtree
        return find(T->right, k);
    }
}


// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node* select(Node* T, int r)
{
    // if (T == nullptr) {
    //    return nullptr;
    //}

    // given assert function 
    assert(T != nullptr && r >= 0 && r < T->size);

    // if there is a left value of T (to the left is not nullptr) find the size of that left value, otherwise = to 0
    int ranked = (T->left) ? T->left->size : 0;

    if (ranked < r) return select(T->right, r - ranked - 1); 
    
    else if (ranked > r) return select(T->left, r); 
 
    else return T;
 
    
}


// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node* join(Node* L, Node* R)
{
    // base cases: if either tree is empty, return the other tree
    if (L == nullptr) {
        return R;
    }

    else if (R == nullptr) {
        return L;
    }

    // choose whether to make the root of L or R the root of the joined tree
    // with probability proportional to their sizes
    if ((double)rand() / RAND_MAX < (double)L->size / (L->size + R->size)) {
        // make the root of L the root of the joined tree
        L->right = join(L->right, R);
        fix_size(L);
        return L;
    }

    else {
        // make the root of R the root of the joined tree
        R->left = join(L, R->left);
        fix_size(R);
        return R;
    }
}

Node* find_min(Node* T)
{
    assert(T != nullptr);

    while (T->left != nullptr) {    // while loop to find the minimum T
        T = T->left;                // if T->Left is not null then set T equal to  T->left which is the new minimum
    }

    return T;
}

// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T
Node* remove(Node* T, int k)
{
    assert(T != nullptr);

    if (k < T->key) {
        T->left = remove(T->left, k);
    }

    else if (k > T->key) {
        T->right = remove(T->right, k);
    }

    else {
        // k == T->key, so we need to remove this node

        if (T->left == nullptr && T->right == nullptr) {
            // case 1: T is a leaf node, so we can simply delete it
            delete T;

            return nullptr;
        }

        else if (T->left == nullptr) {
            // case 2a: T has no left child, so we replace it with its right child
            Node* temp = T->right;
            delete T;

            return temp;

        }

        else if (T->right == nullptr) {
            // case 2b: T has no right child, so we replace it with its left child
            Node* temp = T->left;
            delete T;

            return temp;

        }

        else {
            // case 3: T has both a left and a right child
            // find the minimum value in the right subtree and replace T's key with it
            // then remove the minimum value from the right subtree
            Node* min_right = find_min(T->right);

            T->key = min_right->key;
            T->right = remove(T->right, min_right->key);

            fix_size(T);
            return T;

        }

    }

    fix_size(T);
    return T;
}


// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node* T, int k, Node** L, Node** R)
{
    if (T == nullptr) {
        *L = nullptr;
        *R = nullptr;
        return;
    }

    if (T->key <= k) {
        *L = T;
        split(T->right, k, &((*L)->right), R);
    }

    else {
        *R = T;
        split(T->left, k, L, &((*R)->left));
    }

    if (*L != nullptr) fix_size(*L);

    if (*R != nullptr) fix_size(*R);
}


// problem with 0 and the fact that it repeats?
Node* insert_random(Node* T, int k) {
    if (T == nullptr) {

        Node* newNode = new Node{ k }; // if T is null, create a new node containing k

        return newNode; // returning the newly created node
    }



    // since this is a random insert, you have to create random properties (took a while to figure this out)
    double randNum = rand() % 101 / 100.00;            // using specific random number gen
    double probability = rand() % T->size;

    if (probability <= randNum) {                      // if statement that runs if the probability is less than the random number

        Node* newNode = new Node(k);                   // creation of a new node

        split(T, k, &newNode->left, &newNode->right); // splitting the node 
        fix_size(newNode);

        return newNode;    // returning the node 
    }

    else if (k < T->key) {
        // if k is less than the key of the root of T,
        // recursively insert k left of the root of T
        T->left = insert_random(T->left, k);
        fix_size(T);
    }

    else {
        // otherwise, recursively insert k right of the root of T
        T->right = insert_random(T->right, k);
        fix_size(T);
    }

    return T;
}


void printVector(vector<int> v) // print function that was given
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << endl;
    }
}
