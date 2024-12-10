#include <iostream>
using namespace std;

template<class T>
struct BtreeNode
{
    // curr num of keys
    int n;
    // order of the tree and size of keys and childern array
    int t;
    // position to put value in 
    int position = -1;

    T* keys = new T[t]; // only t-1, but t for the extra violating key
    BtreeNode** children = new BtreeNode*[t];

    BtreeNode(int t) {
        this->t = t;
        this->n = 0;
    }

    bool insert(T newKey) {

        // if the node is leaf
        if (children[0] == nullptr) {
            position++;
            keys[position] = newKey;
            n++;

            // resort the nodes keys
            for (int i = position; i > 0; i--) {
                if (keys[i] < keys[i - 1]) {
                    swap(keys[i], keys[i - 1]);
                    
                } 
            }

        // if the node is not leaf
        } else { 

            // get the location of child to insert in
            int i = 0;
            while (i < n && newKey > keys[i]) {
                i++;
            }

            // if node full, need spliting that leaf node
            if (children[i]->insert(newKey)) {
                T mid;

                int oldi = i;
                //splitted node will store the values and children after mid
                BtreeNode<T>* newSplittedNode = split(children[i], &mid); 

                //get location to put mid value
                while ( i < n && mid > keys[i]) {
                    i++;
                }

                // shifting to make room for mid key
                for (int j = n; j > i; j--) {
                    keys[j] = keys[j - 1];
                }

                //insert mid key
                keys[i] = mid;
                n = n + 1;
                position = position + 1;

                //shifting to make room for mid child pointer
                int k = 0;
                for (k = n; k > oldi + 1; k--) {
                    children[k] = children[k - 1];
                }

                // set the child
                children[k] = newSplittedNode;
            }
        }

        // do we need spliting?
        if (n == t) {
            return true; 
        } else {
            return false;
        } 
    }

    BtreeNode<T>* split(BtreeNode* node, T* mid) {
        int NumberOfKeys = node->n;
        
        BtreeNode<T>* newNode = new BtreeNode<T>(t);

        int midIndex = NumberOfKeys / 2;
        *mid = node->keys[midIndex];
        
        int i;

        // fill splited new node with big oldnode values (t-1)
        for (i = midIndex + 1; i < NumberOfKeys; i++) {
            newNode->position++;
            newNode->keys[newNode->position] = node->keys[i];
            newNode->children[newNode->position] = node->children[i];

            newNode->n++;
            node->position--;
            node->n--;

            node->children[i] = nullptr;
        }

        //now we set the last child because children are t
        newNode->children[newNode->position + 1] = node->children[NumberOfKeys];
        node->children[NumberOfKeys] = nullptr;

        // because the mid value is also removed from the old node
        node->position--;
        node->n--; 

        return newNode;
    }

    void print() {
        printRec(0);
    }

    void printRec(int depth) {
        
        for (int i = 0; i < depth; ++i)
            cout << "  "; 

        for (int i = 0; i < n; i++) {
            if (i > 0)
                cout << ",";
            cout << keys[i];
        }
        cout << endl;

        for (int i = 0; i <= n; i++) {
            if (children[i] != nullptr) {
                children[i]->printRec(depth + 1);
            }
        }
    }
};

template <class T, int Order>
class BTree
{
private:
    //root of the tree
    BtreeNode<T>* root;
    // order of the tree
    int t;
    // tracking curr num of elements in the entire tree
    int numOfElements = 0; 

public:
    BTree() {
        this->t = Order;
        this->root = nullptr;
    }

    void Insert(T value) {
        numOfElements++;

        // if Tree is empty
        if (root == nullptr) {
            root = new BtreeNode<T>(t);

            root->position++;
            root->keys[root->position] = value;
            root->n = 1;

        }
        // if tree not empty
        else {

            // if i need spliting the root after inserting
            if (root->insert(value)) {
                T mid;

                BtreeNode<T>* newSplittedNode = root->split(root, &mid);
                BtreeNode<T>* newNode = new BtreeNode<T>(t);

                newNode->position++;
                newNode->keys[newNode->position] = mid;
                newNode->n = 1;

                newNode->children[0] = root;
                newNode->children[1] = newSplittedNode;
                
                root = newNode;
            }
        }
    }

    void Print() const {
        if (root != nullptr) {
            root->print();
        } else {
            cout << "tree empty" << endl;
        } 
    }

};


int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}