#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
using namespace std;

class bPlusTreeOG {
    struct Node {
        //individual keys + listings
        struct listing {
            string name;
            string city;
            float price;
            string room_type;
            string reviews;
            string accommodations;
            float bathCount;
            float bedroomCount;
            int bedCount;
            string area; //continent

            listing(const string&  n, const string& c, const string& p, const string& room, const string& rev, const string& ac, const string& bath, const string& bedroom, const string& bed, const string& a) {
                name = n;
                city = c;
                price = stof(p);
                room_type = room;
                reviews = rev;
                accommodations = ac;
                bathCount = stof(bath);
                bedroomCount = stof(bedroom);
                bedCount = stoi(bed);
                area = a;
            }
        };
        vector<listing> bnbs; //elements in node
        vector<float> keys; //keys of elements, depends on user sorting
        bool isLeaf;
        vector<Node*> children;
        Node* parent;
        Node* next; //ONLY FOR LEAVES (linked list)
        Node(const float& k, const string&  n, const string& c, const string& p, const string& room, const string& rev, const string& ac, const string& bath, const string& bedroom, const string& bed, const string& a) {
            keys.push_back(k);
            bnbs.push_back(listing(n, c, p, room, rev, ac, bath, bedroom, bed, a));
            isLeaf = true;
            next = nullptr;
            parent = nullptr;
        }
        Node(Node* p, vector<listing> b, vector<float> k, vector<Node*> c) {
            parent = p;
            bnbs = b;
            keys = k;
            children = c;
            next = nullptr;
            isLeaf = false;
        }
        Node(Node* p, vector<listing> b, vector<float> k) {
            parent = p;
            bnbs = b;
            keys = k;
            children = {};
            next = nullptr;
            isLeaf = true;
        }
        bool operator<(const Node* other) const {
            return keys[0] < other->keys[0];
        }
        void insert(const int& i, const string&  n, const string& c, const string& p, const string& room, const string& rev, const string& ac, const string& bath, const string& bedroom, const string& bed, const string& a) {
            listing l(n, c, p, room, rev, ac, bath, bedroom, bed, a);
            bnbs.insert(bnbs.begin()+i, l);
        }


    };
    Node* root; //actual tree root
    Node* head; // linked list
    string sortKey; // either "price", "bedrooms", or "bathrooms"
    int num; //max num of nodes (n-ary)
    string fileName;

    void buildTree(string f) {
        ifstream file(f);
        if (!file.is_open()) {
            cout<< "Error: file not open"<< endl;
        } else {
            string row;
            int i=0;
            while (getline(file, row)) {
                stringstream s(row);
                if (i == 0) {
                    i++; //initial row is headers
                } else if (row == "") {
                    cout<< "Error: empty line, i = "<< i<< endl;
                }else {
                    string id, name, city, price, room, reviews, ac, bathCount, bedroomCount, bedCount, area;
                    getline(s, id, ',');
                    getline(s, name, ',');
                    getline(s, city, ',');
                    getline(s, price, ',');
                    getline(s, room, ',');
                    getline(s, reviews, ',');
                    getline(s, ac, ',');
                    getline(s, bathCount, ',');
                    getline(s, bedroomCount, ',');
                    getline(s, bedCount, ',');
                    getline(s, area, ',');
                    cout<< name<< "   ";
                    insert(name, city, price, room, reviews, ac, bathCount, bedroomCount, bedCount, area);
                }

            }
        }
    }

public:
    bPlusTreeOG(const int& n, const string& key, string f) {
        fileName = f;
        root = nullptr;
        head = nullptr;
        sortKey = key;
        num = n;
        buildTree(fileName);
    }
    ~bPlusTreeOG() {
        deleteSubTree(root);
        root = nullptr;
        head = nullptr;
    }

    void deleteSubTree(Node* cur){
        if (cur == nullptr) {
            // base case? do nothing
        } else {
            for (Node* child: cur->children) {
                deleteSubTree(child);
            }
            delete cur;
            cur = nullptr;
        }

    }

    //sets the actual key to sort by:
    //price, bedrooms, or bathrooms --> chosen by user
    void sortBy(const string& s) {
        sortKey = s;
        reSort();
    }
    //insert helper functions: insertNode, insertionIndex, balance
    Node* insertNode(Node* parent, vector<Node::listing>& bnbs, vector<float>& keys, vector<Node*>& children) {
        //add child node to 'parent' with preexisting children 'children'
        int index = -1;
        for (int i=0; i<parent->children.size(); i++) {
            if (keys[0] < parent->children[i]->keys[0]) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            index = parent->children.size();
        }
        parent->isLeaf = false;
        parent->children.insert(parent->children.begin() + index, new Node(parent, bnbs, keys, children));
        sort(parent->children.begin(), parent->children.end());
        return parent->children[index];
    }
    Node* insertNode(Node* parent, vector<Node::listing>& bnbs, vector<float>& keys) {
        //add leaf node to 'parent', check number of children in 'parent'
        Node* newNode = new Node(parent, bnbs, keys);
        parent->isLeaf = false;
        parent->children.push_back(newNode);
        sort(parent->children.begin(), parent->children.end());
        return newNode;
    }    int insertionIndex(vector<float> keys, float k) {
        for (int i = 0; i < keys.size(); i++) {
            if (k < keys[i]) {
                return i;
            }
        }
        return keys.size();
    }
    void balance(Node* cur) {
        if (cur->keys.size() == num) {
            vector<float> lastKeys(cur->keys.begin()+ cur->keys.size()/2, cur->keys.end()); // <--used for splitting keys
            vector<float> firstKeys(cur->keys.begin(), cur->keys.begin()+ cur->keys.size()/2); // <--used for splitting keys
            vector<Node::listing> lastBnbs(cur->bnbs.begin()+ cur->bnbs.size()/2, cur->bnbs.end());
            vector<Node::listing> firstBnbs(cur->bnbs.begin(), cur->bnbs.begin()+ cur->bnbs.size()/2);

            if (root == head) {
                //split node in half, middle goes up to become root
                //set leaves to be LL
                cur->keys = {lastKeys[0]};
                cur->bnbs = {lastBnbs[0]};
                cur->isLeaf = false;
                head = insertNode(cur, firstBnbs, firstKeys);
                head->next = insertNode(cur, lastBnbs, lastKeys);
            } else if (cur->isLeaf) {
                int i = insertionIndex(cur->parent->keys, lastKeys[0]);
                cur->parent->keys.insert(cur->parent->keys.begin() + i, lastKeys[0]);
                cur->parent->bnbs.insert(cur->parent->bnbs.begin() + i, lastBnbs[0]);
                cur->keys = firstKeys;
                cur->bnbs = firstBnbs;
                Node* thirdNode = cur->next;
                cur->next = insertNode(cur->parent, lastBnbs, lastKeys);
                cur->next->next = thirdNode;
            } else if (cur == root) {
                vector<Node*> firstKids(cur->children.begin(), cur->children.begin()+ cur->children.size()/2);
                vector<Node*> lastKids(cur->children.begin()+ cur->children.size()/2 +1, cur->children.end());
                cur->keys = {cur->keys[cur->keys.size()/2]};
                cur->children = {};
                cur->children.push_back(insertNode(cur, firstBnbs, firstKeys, firstKids));
                cur->children.push_back(insertNode(cur, lastBnbs, lastKeys, lastKids));
            } else {
                vector<Node*> firstKids(cur->children.begin(), cur->children.begin()+ cur->children.size()/2);
                vector<Node*> lastKids(cur->children.begin()+ cur->children.size()/2 +1, cur->children.end());
                int i = insertionIndex(cur->parent->keys, lastKeys[0]);
                cur->parent->keys.insert(cur->parent->keys.begin() + i, cur->keys[cur->keys.size()/2]);
                cur->parent->bnbs.insert(cur->parent->bnbs.begin() + i, cur->bnbs[cur->bnbs.size()/2]);

                cur->keys = firstKeys;
                cur->bnbs = firstBnbs;
                cur->children = firstKids;
                insertNode(cur->parent, lastBnbs, lastKeys, lastKids);
            }
            balance(cur->parent);
        }
    }
    bool insert(const string&  n, const string& c, const string& p, const string& room, const string& rev, string ac, const string& bath, const string& bedroom, const string& bed, const string& a) {
        float key;
        if (sortKey == "price") {
            key = stof(p);
        } else if (sortKey == "bedrooms") {
            key = stof(bedroom);
        } else if (sortKey == "bathrooms") {
            key = stof(bath);
        } else {
            cout<< "Error: invalid key value. User sorted with: "<< sortKey<< endl;
            return false;
        }
        if (root == nullptr) {
            root = new Node(key, n, c, p, room, rev, ac, bath, bedroom, bed, a);
            head = root;
            return true;
        }
        Node* curNode = root;
        //just finds the node to insert into
        while (curNode != nullptr) {
            //checks if node is leaf
            if (curNode->isLeaf) {
                break;
            }
            int mid = curNode->keys.size()/2;
            for (int i=0; i<curNode->keys.size(); i++) {
                if ((key > curNode->keys[mid])) {
                    if (mid == curNode->keys.size()-1) {
                        curNode = curNode->children[i+1];
                        break;
                    }
                    i = mid;
                    mid = (curNode->keys.size()+mid)/2;
                } else if (key < curNode->keys[mid]) {
                    mid = (i+mid)/2;
                } else if (key == curNode->keys[mid]) {
                    curNode = curNode->children[i];
                    break;
                }

                if (key <= curNode->keys[i]) {
                    if (curNode->children.size() > i) {
                        curNode = curNode->children[i];
                    } else {
                        cout<< "Error: children smaller than current key index"<< endl;
                        return false;
                    }
                } else if (key > curNode->keys[i]) {
                    if (i == curNode->keys.size()-1) {
                        if (curNode->children.size() > i) {
                            curNode = curNode->children[i+1];
                        }
                    }
                }
            }
        }
        //does actual insertion into node
        //curNode = inserting node
        for (int i = 0; i< curNode->keys.size(); i++) {
            if (key <= i) {
                curNode->keys.insert(curNode->keys.begin()+i, key);
                curNode->insert(i, n, c, p, room, rev, ac, bath, bedroom, bed, a);
                balance(curNode);
                return true;
            }else if ((key > i) && (i == curNode->keys.size())) {
                curNode->keys.insert(curNode->keys.begin() +curNode->keys.size(), key);
                curNode->insert(curNode->keys.size(), n, c, p, room, rev, ac, bath, bedroom, bed, a);
                balance(curNode);
                return true;
            }
        }

        cout<< "Insert error: something went wrong, insertion not completed."<< endl;
        return false;
    }

    void inOrderLL() {
        //node root has vector of node keys
        Node* curr = root;
        while (root != nullptr) {
            cout<< "Node: ";
            for (float k: curr->keys) {
                cout<< k<< " ";
            }
            cout<< endl;
            root = root->next;
        }
    }
    void reSort() {
        cout<< "Re-sorting !"<< endl;
        deleteSubTree(root);
        root = nullptr;
        head = nullptr;
        buildTree(fileName);
    }
    vector<Node::listing> search(string location) {
        vector<Node::listing> match;
        Node* cur = head;
        while (cur != nullptr) {
            for (Node::listing l: cur->bnbs) {
                if (l.area == location) {
                    match.push_back(l);
                }
            }
            cur = cur->next;
        }
        return match;
    }
};


int main() {
    // bPlusTree testing(3, "price", "testingCSV.csv");
    // testing.inOrderLL();
    cout<< "test";
    return 0;
}
