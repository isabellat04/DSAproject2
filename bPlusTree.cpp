#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
using namespace std;

class bPlusTree {
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
            // listing() {
            //     name = "";
            //     city = "";
            //     price = 0;
            //     capacity = 0;
            //     bathCount = 0;
            //     bedroomCount = 0;
            //     bedCount = 0;
            //     area = "";
            // }
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
            isLeaf = true; //check this
            next = nullptr;
            parent = nullptr;
        }


    };
    Node* root; //actual tree root
    Node* head; // linked list
    string sortKey; // either "price", "bedrooms", or "bathrooms"
    int num; //max num of nodes (n-ary)

public:
    bPlusTree(const int& n, const string& key, string fileName) {
        root = nullptr;
        head = nullptr;
        sortKey = key;
        num = n;
        ifstream file(fileName);
        if (!file.is_open()) {
            cout<< "Error: file not open"<< endl;
        } else {
            string row;
            while (getline(file, row)) {
                stringstream s(row);
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
                insert(name, city, price, room, reviews, ac, bathCount, bedroomCount, bedCount, area);
            }
        }


    }
    ~bPlusTree() {
        Node* cur = head;
        Node* next;
        while (cur != nullptr) {
            next = cur->next;
            delete cur;
            cur = next;
        }
        root = nullptr;
        head = nullptr;
    }
    //sets the actual key to sort by:
    //price, bedrooms, or bathrooms --> chosen by user
    void sortBy(const string& s) {
        sortKey = s;
        reSort();
    }
    void balance(Node* cur) {
        //potentially recursive, look into how we'd do this
        //look at examples or write out/say logic
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
                balance(curNode);
                return true;
            }else if ((key > i) && (i == curNode->keys.size())) {
                curNode->keys.insert(curNode->keys.begin() +curNode->keys.size(), key);
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
        //literally resorts all the nodes based on new sortKey
        cout<< "Re-sorting !"<< endl;
        //try doing something that iterates through tree changing keys for each node
        //then call a rebalance function or something after each iteration
    }

    float search(float n) {
        return 0.0;
        //figure this out diva
    }
};


int main() {
    // bPlusTree testing(3, "price", "testingCSV.csv");
    // testing.inOrderLL();
    return 0;
}
