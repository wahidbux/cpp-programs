// implicit_treap.cpp
// Advanced Implicit Treap: supports sequence operations with lazy propagation.

#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
std::mt19937_64 rng(std::random_device{}());

struct Node {
    int64 val;           // value at this position
    int64 sum;           // subtree sum
    int sz;              // subtree size
    uint64_t pri;        // random priority
    Node *l, *r;

    // lazy flags
    bool rev;            // pending reverse
    bool assign_flag;    // pending assign
    int64 assign_val;    // value to assign
    int64 add;           // pending add

    Node(int64 v)
        : val(v), sum(v), sz(1), pri(rng()), l(nullptr), r(nullptr),
          rev(false), assign_flag(false), assign_val(0), add(0) {}
};

int size(Node* t) { return t ? t->sz : 0; }
int64 subsum(Node* t) { return t ? t->sum : 0; }

void apply_assign(Node* t, int64 x) {
    if (!t) return;
    t->assign_flag = true;
    t->assign_val = x;
    t->add = 0; // add and assign conflict -> assign wins
    t->val = x;
    t->sum = (int64)t->sz * x;
}

void apply_add(Node* t, int64 x) {
    if (!t) return;
    if (t->assign_flag) {
        // if assign pending, just change its assign_val
        t->assign_val += x;
        t->val += x;
        t->sum += (int64)t->sz * x;
    } else {
        t->add += x;
        t->val += x;
        t->sum += (int64)t->sz * x;
    }
}

void apply_rev(Node* t) {
    if (!t) return;
    t->rev ^= 1;
    std::swap(t->l, t->r);
}

void push(Node* t) {
    if (!t) return;
    // Assign has highest precedence
    if (t->assign_flag) {
        apply_assign(t->l, t->assign_val);
        apply_assign(t->r, t->assign_val);
        t->assign_flag = false;
    }
    if (t->add != 0) {
        apply_add(t->l, t->add);
        apply_add(t->r, t->add);
        t->add = 0;
    }
    if (t->rev) {
        apply_rev(t->l);
        apply_rev(t->r);
        t->rev = false;
    }
}

void pull(Node* t) {
    if (!t) return;
    t->sz = 1 + size(t->l) + size(t->r);
    t->sum = t->val + subsum(t->l) + subsum(t->r);
}

// split t into [0..k-1] and [k..end], where k is number of nodes in left part
void split(Node* t, int k, Node*& a, Node*& b) {
    if (!t) { a = b = nullptr; return; }
    push(t);
    if (size(t->l) >= k) {
        // whole split in left subtree
        split(t->l, k, a, t->l);
        b = t;
        pull(b);
    } else {
        split(t->r, k - size(t->l) - 1, t->r, b);
        a = t;
        pull(a);
    }
}

Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->pri > b->pri) {
        push(a);
        a->r = merge(a->r, b);
        pull(a);
        return a;
    } else {
        push(b);
        b->l = merge(a, b->l);
        pull(b);
        return b;
    }
}

// build from vector in O(n) expected using stack-based method
Node* build_from_vector(const vector<int64>& v) {
    if (v.empty()) return nullptr;
    vector<Node*> st;
    for (auto x : v) {
        Node* cur = new Node(x);
        Node* last = nullptr;
        while (!st.empty() && st.back()->pri < cur->pri) {
            last = st.back();
            st.pop_back();
            pull(last);
        }
        cur->l = last;
        if (!st.empty()) st.back()->r = cur;
        st.push_back(cur);
    }
    // now pull and link root
    while (st.size() > 1) {
        pull(st.back());
        st.pop_back();
    }
    pull(st[0]);
    return st[0];
}

// wrappers for operations
// insert sequence 'v' at position pos (0-indexed, insert before pos)
Node* insert_at(Node* root, int pos, const vector<int64>& v) {
    Node *a, *b;
    split(root, pos, a, b);
    Node* mid = build_from_vector(v);
    return merge(merge(a, mid), b);
}

// erase [l..r) zero-indexed
Node* erase_range(Node* root, int l, int r) {
    Node *a, *mid, *c;
    split(root, l, a, mid);
    split(mid, r - l, mid, c);
    // delete mid subtree nodes (optional, avoid memory leaks)
    // Use DFS to delete nodes
    if (mid) {
        vector<Node*> stack = {mid};
        while (!stack.empty()) {
            Node* n = stack.back(); stack.pop_back();
            if (n->l) stack.push_back(n->l);
            if (n->r) stack.push_back(n->r);
            delete n;
        }
    }
    return merge(a, c);
}

// range add [l..r)
Node* range_add(Node* root, int l, int r, int64 val) {
    Node *a, *mid, *c;
    split(root, l, a, mid);
    split(mid, r - l, mid, c);
    apply_add(mid, val);
    return merge(merge(a, mid), c);
}

// range assign [l..r)
Node* range_assign(Node* root, int l, int r, int64 val) {
    Node *a, *mid, *c;
    split(root, l, a, mid);
    split(mid, r - l, mid, c);
    apply_assign(mid, val);
    return merge(merge(a, mid), c);
}

// range reverse [l..r)
Node* range_reverse(Node* root, int l, int r) {
    Node *a, *mid, *c;
    split(root, l, a, mid);
    split(mid, r - l, mid, c);
    apply_rev(mid);
    return merge(merge(a, mid), c);
}

// range sum query [l..r)
int64 range_sum(Node* root, int l, int r) {
    Node *a, *mid, *c;
    split(root, l, a, mid);
    split(mid, r - l, mid, c);
    int64 res = subsum(mid);
    root = merge(merge(a, mid), c); // reassemble
    (void)root;
    return res;
}

// get value at index pos
int64 get_at(Node* root, int pos) {
    Node *a, *b;
    split(root, pos, a, b);
    split(b, 1, b, b);
    int64 res = b ? b->val : 0;
    root = merge(merge(a, b), b);
    // proper reassemble:
    Node* single = nullptr;
    split(merge(a,b), pos, a, single);
    Node* cur = merge(a,b); 
    cur = nullptr;
    Node* t = merge(a, b);
    return res;
}

// safer get (non-destructive)
int64 get_at_safe(Node* t, int pos) {
    if (!t || pos < 0 || pos >= size(t)) throw out_of_range("index");
    while (t) {
        push(t);
        int lsz = size(t->l);
        if (pos < lsz) t = t->l;
        else if (pos == lsz) return t->val;
        else {
            pos -= lsz + 1;
            t = t->r;
        }
    }
    throw runtime_error("unexpected");
}

// kth smallest by index (0-indexed)
int64 kth(Node* root, int k) { return get_at_safe(root, k); }

// inorder traversal to print sequence
void inorder(Node* t, vector<int64>& out) {
    if (!t) return;
    push(t);
    inorder(t->l, out);
    out.push_back(t->val);
    inorder(t->r, out);
}

// cleanup
void delete_treap(Node* t) {
    if (!t) return;
    vector<Node*> st = {t};
    while (!st.empty()) {
        Node* n = st.back(); st.pop_back();
        if (n->l) st.push_back(n->l);
        if (n->r) st.push_back(n->r);
        delete n;
    }
}

// ---------- Demonstration / Tests ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // build initial sequence: [1,2,3,4,5]
    vector<int64> init = {1,2,3,4,5};
    Node* root = build_from_vector(init);

    cout << "Initial sequence: ";
    vector<int64> out;
    inorder(root, out);
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    // insert [10,20] at position 2 => [1,2,10,20,3,4,5]
    root = insert_at(root, 2, {10,20});
    out.clear(); inorder(root, out);
    cout << "After insert at pos 2: ";
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    // range add +5 to [1..4) => positions 1..3 (0-indexed)
    root = range_add(root, 1, 4, 5);
    out.clear(); inorder(root, out);
    cout << "After range add +5 [1,4): ";
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    // range assign [3..5) = 7
    root = range_assign(root, 3, 5, 7);
    out.clear(); inorder(root, out);
    cout << "After range assign =7 [3,5): ";
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    // range reverse [0..4)
    root = range_reverse(root, 0, 4);
    out.clear(); inorder(root, out);
    cout << "After reverse [0,4): ";
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    // range sum query [1..5)
    int64 s = range_sum(root, 1, 5);
    cout << "Range sum [1,5): " << s << "\n";

    // get kth
    cout << "Element at index 2: " << kth(root, 2) << "\n";

    // erase [2..4)
    root = erase_range(root, 2, 4);
    out.clear(); inorder(root, out);
    cout << "After erase [2,4): ";
    for (auto x : out) cout << x << ' ';
    cout << "\n";

    delete_treap(root);
    return 0;
}
