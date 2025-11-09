#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <cfloat>
#include "bPlusTree.cpp"
#include <chrono>
using namespace std;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
struct Property_Listing {
    int id;
    std::string name;
    std::string city;
    int price;
    int room_type;
    int total_reviewers;
    int accommodates;
    int bathrooms;
    int bedrooms;
    int beds;
    std::string area;

    Property_Listing(string& id, string& name, string& city, string& price, string& room_type, string& total_reviewers, string& accommodates, string& bathrooms, string& bedrooms, string& beds, string& area) {
        this->id = stoi(id);
        this->name = name;
        this->city = city;
        this->price = stoi(price);
        this->room_type = stoi(room_type);
        this->total_reviewers = stoi(total_reviewers);
        this->accommodates = stoi(accommodates);
        this->bathrooms = stoi(bathrooms);
        this->bedrooms = stoi(bedrooms);
        this->beds = stoi(beds);
        this->area = area;
    }
};

class MinHeapStruct {
    int heap_size = 0;
    vector<Property_Listing> currHeap; // this can represent either min or max heap

    public:
        // Create min Heap as initial type of heap
        void insertMinHeap (Property_Listing& property) {
            currHeap.push_back(property);
            heap_size++;

            // Check if recently added index is in valid position
            if (heap_size != 1) {
                int child_index = heap_size - 1;
                int parent_index = (child_index - 1) / 2;
                while (currHeap[parent_index].id > currHeap[child_index].id) {
                    Property_Listing temp = currHeap[child_index];
                    currHeap[child_index] = currHeap[parent_index];
                    currHeap[parent_index] = temp;
                    child_index = parent_index;
                    parent_index = (child_index - 1) / 2;
                }
            }
        }

        // Implementation is similar to extractMax quiz by Anveer Aulakh
        void heapifyDownMaxPrice(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].price < passed_in_heap[index].price) && (passed_in_heap[right_child_index].price < passed_in_heap[index].price)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].price > passed_in_heap[index].price) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].price >= passed_in_heap[right_child_index].price) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMaxPrice(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxPrice(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].price > passed_in_heap[index].price) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].price < passed_in_heap[right_child_index].price) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxPrice(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void heapifyDownMinPrice(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].price > passed_in_heap[index].price) && (passed_in_heap[right_child_index].price > passed_in_heap[index].price)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].price < passed_in_heap[index].price) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].price <= passed_in_heap[right_child_index].price) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMinPrice(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinPrice(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].price < passed_in_heap[index].price) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].price > passed_in_heap[right_child_index].price) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinPrice(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void heapifyDownMaxBathrooms(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].bathrooms < passed_in_heap[index].bathrooms) && (passed_in_heap[right_child_index].bathrooms < passed_in_heap[index].bathrooms)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].bathrooms > passed_in_heap[index].bathrooms) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].bathrooms >= passed_in_heap[right_child_index].bathrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMaxBathrooms(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxBathrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].bathrooms > passed_in_heap[index].bathrooms) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].bathrooms < passed_in_heap[right_child_index].bathrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxBathrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void heapifyDownMinBathrooms(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].bathrooms > passed_in_heap[index].bathrooms) && (passed_in_heap[right_child_index].bathrooms > passed_in_heap[index].bathrooms)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].bathrooms < passed_in_heap[index].bathrooms) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].bathrooms <= passed_in_heap[right_child_index].bathrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMinBathrooms(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinBathrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].bathrooms < passed_in_heap[index].bathrooms) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].bathrooms > passed_in_heap[right_child_index].bathrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinBathrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void heapifyDownMaxBedrooms(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].bedrooms < passed_in_heap[index].bedrooms) && (passed_in_heap[right_child_index].bedrooms < passed_in_heap[index].bedrooms)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].bedrooms > passed_in_heap[index].bedrooms) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].bedrooms >= passed_in_heap[right_child_index].bedrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMaxBedrooms(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxBedrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].bedrooms > passed_in_heap[index].bedrooms) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].bedrooms < passed_in_heap[right_child_index].bedrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMaxBedrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void heapifyDownMinBedrooms(vector<Property_Listing>& passed_in_heap, int index, int curr_size) {
            int left_child_index = index * 2 + 1;
            int right_child_index = index * 2 + 2;
            if (left_child_index > curr_size - 1 && right_child_index > curr_size - 1) { // this checks if current index is a leaf
                return;
            }
            else if ((passed_in_heap[left_child_index].bedrooms > passed_in_heap[index].bedrooms) && (passed_in_heap[right_child_index].bedrooms > passed_in_heap[index].bedrooms)) { // this will stop recursion if index
                return;
            }
            else {
                if (left_child_index < curr_size && passed_in_heap[left_child_index].bedrooms < passed_in_heap[index].bedrooms) {
                    if (right_child_index > curr_size - 1 || passed_in_heap[left_child_index].bedrooms <= passed_in_heap[right_child_index].bedrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[left_child_index];
                        passed_in_heap[left_child_index] = temp;
                        heapifyDownMinBedrooms(passed_in_heap, left_child_index, curr_size);
                    }
                    else {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinBedrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
                else if (right_child_index < curr_size && passed_in_heap[right_child_index].bedrooms < passed_in_heap[index].bedrooms) {
                    if (left_child_index > curr_size - 1 || passed_in_heap[left_child_index].bedrooms > passed_in_heap[right_child_index].bedrooms) {
                        Property_Listing temp = passed_in_heap[index];
                        passed_in_heap[index] = passed_in_heap[right_child_index];
                        passed_in_heap[right_child_index] = temp;
                        heapifyDownMinBedrooms(passed_in_heap, right_child_index, curr_size);
                    }
                }
            }
        }

        void createMaxHeapPrice(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMaxPrice(currHeap, i, curr_size);
        }

        void createMinHeapPrice(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMinPrice(currHeap, i, curr_size);
        }

        void createMaxHeapBathroom(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMaxBathrooms(currHeap, i, curr_size);
        }

        void createMinHeapBathroom(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMinBathrooms(currHeap, i, curr_size);
        }

        void createMaxHeapBedroom(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMaxBedrooms(currHeap, i, curr_size);
        }

        void createMinHeapBedroom(int curr_size) {
            for(int i = curr_size/2; i >= 0; i--)
                heapifyDownMinBedrooms(currHeap, i, curr_size);
        }

        void heapSortAscendingPrice() {
            // The clock time code for all methods was inspired from Stack Overflow
            auto start_clock = chrono::high_resolution_clock::now(); // this is used to check time for how long to process
            int store_size = heap_size;
            createMaxHeapPrice(heap_size); // create max heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMaxPrice(currHeap ,0, store_size);
            }
            auto end_clock = chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        void heapSortDescendingPrice() {
            auto start_clock = std::chrono::high_resolution_clock::now();
            int store_size = heap_size;
            createMinHeapPrice(heap_size); // crete min heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMinPrice(currHeap, 0, store_size);
            }

            auto end_clock = std::chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            std::cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        void heapSortAscendingBathrooms() {
            auto start_clock = std::chrono::high_resolution_clock::now();
            int store_size = heap_size;
            createMaxHeapBathroom(heap_size); // create max heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMaxBathrooms(currHeap ,0, store_size);
            }
            auto end_clock = std::chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            std::cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        void heapSortDescendingBathrooms() {
            auto start_clock = std::chrono::high_resolution_clock::now();
            int store_size = heap_size;
            createMinHeapBathroom(heap_size); // crete min heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMinBathrooms(currHeap, 0, store_size);
            }
            auto end_clock = std::chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            std::cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        void heapSortAscendingBedrooms() {
            auto start_clock = std::chrono::high_resolution_clock::now();
            int store_size = heap_size;
            createMaxHeapBedroom(heap_size); // create max heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMaxBedrooms(currHeap ,0, store_size);
            }
            auto end_clock = std::chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            std::cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        void heapSortDescendingBedrooms() {
            auto start_clock = std::chrono::high_resolution_clock::now();
            int store_size = heap_size;
            createMinHeapBedroom(heap_size); // crete min heap first
            while (store_size > 1) {
                Property_Listing temp = currHeap[0];
                currHeap[0] = currHeap[store_size - 1];
                currHeap[store_size - 1] = temp;
                store_size = store_size - 1;
                heapifyDownMinBedrooms(currHeap, 0, store_size);
            }
            auto end_clock = std::chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end_clock-start_clock;
            std::cout << "Time for heaps: " << duration.count() << " seconds" << std::endl;
        }

        vector<Property_Listing> findKthLargestPrice(int k) {
            vector<Property_Listing> largest_holder;
            if (k == 0 || k > currHeap.size()) {
                cout << "Please input a number greater than 0 or less than number of entries" << endl;
                return largest_holder;
            }
            heapSortDescendingPrice(); // first sort current heap into descending order according to attribute
            Property_Listing temp = currHeap[0]; // initial object to compare
            int placeHolder = -1;
            int count_switch = 0;
            int index = 0;
            while (count_switch <= k && index < heap_size) {
                if (currHeap[index].price != placeHolder) {
                    count_switch = count_switch + 1;
                    temp = currHeap[index];
                    placeHolder = currHeap[index].price;
                }

                if (count_switch == k) {
                    largest_holder.push_back(currHeap[index]);
                    //cout << temp.price << endl;
                }
                index++;
            }
            return largest_holder;
        }

        vector<Property_Listing> findKthLargestBathroom(int k) {
            vector<Property_Listing> largest_holder;
            if (k == 0 || k > currHeap.size()) {
                cout << "Please input a number greater than 0 or less than number of entries" << endl;
                return largest_holder;
            }
            heapSortDescendingBathrooms(); // first sort current heap into descending order according to attribute
            Property_Listing temp = currHeap[0]; // initial object to compare
            int placeHolder = -1;
            int count_switch = 0;
            int index = 0;
            while (count_switch <= k && index < heap_size) {
                if (currHeap[index].bathrooms != placeHolder) {
                    count_switch = count_switch + 1;
                    temp = currHeap[index];
                    placeHolder = currHeap[index].bathrooms;
                }

                if (count_switch == k) {
                    largest_holder.push_back(currHeap[index]);
                    //cout << temp.bathrooms << endl;
                }
                index++;
            }
            return largest_holder;
        }

        vector<Property_Listing> findKthLargestBedrooms(int k) {
            vector<Property_Listing> largest_holder;
            if (k == 0 || k > currHeap.size()) {
                cout << "Please input a number greater than 0 or less than number of entries" << endl;
                return largest_holder;
            }
            heapSortDescendingBedrooms(); // first sort current heap into descending order according to attribute
            Property_Listing temp = currHeap[0]; // initial object to compare
            int placeHolder = -1;
            int count_switch = 0;
            int index = 0;
            while (count_switch <= k && index < heap_size) {
                if (currHeap[index].bedrooms != placeHolder) {
                    count_switch = count_switch + 1;
                    temp = currHeap[index];
                    placeHolder = currHeap[index].bedrooms;
                }

                if (count_switch == k) {
                    largest_holder.push_back(currHeap[index]);
                    //cout << temp.bedrooms << endl;
                }
                index++;
            }
            return largest_holder;
        }

        int getCurrentSize() {
            return heap_size;
        }

        vector<Property_Listing> getCurrentHeap() {
            return currHeap;
        }
};

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
    // <b>lang</b> variable name to see how CLion can help you rename it.
    std::string filename = "Airbnb_data.csv"; // Make sure this is the correct file name!
    std::ifstream file(filename);


    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file '" << filename << "'" << std::endl;
        return 1;
    }
    int counter = 0;
    vector<Property_Listing> list;

    // 2. Read the first 5 data rows
    if (file.is_open()) {
        string line;
        while (getline(file, line) && counter <= 20) {
            stringstream ss(line);
            string id;
            getline(ss, id, ',');

            string name;
            getline(ss, name, ',');

            string city;
            getline(ss, city, ',');

            string price;
            getline(ss, price, ',');

            string room_type;
            getline(ss, room_type, ',');

            string total_reviewers;
            getline(ss, total_reviewers, ',');

            string accommodates;
            getline(ss, accommodates, ',');

            string bathrooms;
            getline(ss, bathrooms, ',');

            string bedrooms;
            getline(ss, bedrooms, ',');

            string beds;
            getline(ss, beds, ',');

            string area;
            getline(ss, area);


            if (counter != 0) { // used to skip the first line in CSV file
                Property_Listing property = Property_Listing(id, name, city, price, room_type, total_reviewers, accommodates, bathrooms, bedrooms, beds, area);
                list.push_back(property);
            }
            counter++;
        }
    }
    file.close();

    // Used to test if program is reading file correctly (substitute characteristic (area) for other values)
    //for (int i = 0; i < list.size(); i++) {
    //    cout << list[i].area << endl;
    //}

    // Create initial heap (can be ordered according to any attribute, even id)
    MinHeapStruct minHeapStruct;
    for (int i = 0; i < list.size(); i++) {
        minHeapStruct.insertMinHeap(list[i]);
    }

    int store_heap_size = minHeapStruct.getCurrentSize();

    // Testing if method are working
    //minHeapStruct.createMaxHeapPrice(store_heap_size);
    //vector<Property_Listing> minHeap = minHeapStruct.getCurrentHeap();
    //for (int i = 0; i < minHeapStruct.getCurrentSize(); i++) {
    //    cout << minHeap[i].price << endl;
    //}

    minHeapStruct.heapSortAscendingPrice();
    //minHeapStruct.heapSortDescendingPrice(); // test if global heap is changing according to commands called


//sfml visuals
//create window
sf::RenderWindow window(sf::VideoMode(1850, 900), "Home-inator: Housing Property Finder");
    //generate font
sf::Font font;
if (!font.loadFromFile("font.ttf")) {
    cout << "Error loading font!" << endl;
    return 1;
}

//tabs
vector<string> tabs = {"Menu", "Sort Price", "Sort Bedrooms", "Sort Bathrooms", "Search Location"};
int currentTab = 0;

vector<sf::RectangleShape> tabButtons;
vector<sf::Text> tabTexts;
float tabX = 100;
for (int i = 0; i < tabs.size(); i++) {
    sf::RectangleShape tab(sf::Vector2f(200, 40));
    tab.setPosition(tabX, 20);
    tab.setOutlineColor(sf::Color::Black);
    tab.setOutlineThickness(2);
    tabButtons.push_back(tab);

    sf::Text text(tabs[i], font, 18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(tabX + 20, 25);
    tabTexts.push_back(text);

    tabX += 210;
}

//title
sf::Text title("Home-inator: Housing Property Finder", font, 28);
title.setFillColor(sf::Color::Black);
title.setPosition(320, 90);

//all input boxes
sf::RectangleShape inputBoxes[7];
sf::Text inputTexts[7];
std::string inputStrings[7];
std::string featureNames[7] = {"Min Price", "Max Price", "Min Bedrooms", "Max Bedrooms", "Min Baths", "Max Baths", "Location"};

for (int i = 0; i < 7; i++) {
    inputBoxes[i].setSize(sf::Vector2f(200, 30));
    inputBoxes[i].setPosition(30 + (i % 2) * 250, 170 + (i / 2) * 70);
    inputBoxes[i].setFillColor(sf::Color::White);
    inputBoxes[i].setOutlineColor(sf::Color::Black);
    inputBoxes[i].setOutlineThickness(1);

    inputTexts[i].setFont(font);
    inputTexts[i].setCharacterSize(18);
    inputTexts[i].setFillColor(sf::Color::Black);
    inputTexts[i].setPosition(inputBoxes[i].getPosition().x + 10, inputBoxes[i].getPosition().y + 5);
}

//labels for input boxes
vector<sf::Text> labels;
for (int i = 0; i < 7; i++) {
    sf::Text label(featureNames[i], font, 18);
    label.setFillColor(sf::Color::Black);
    label.setPosition(inputBoxes[i].getPosition().x, inputBoxes[i].getPosition().y - 25);
    labels.push_back(label);
}

//create list button
sf::RectangleShape calcButton(sf::Vector2f(290, 45));
calcButton.setPosition(150, 530);
calcButton.setFillColor(sf::Color(200, 220, 255));
calcButton.setOutlineColor(sf::Color::Black);
calcButton.setOutlineThickness(2);

sf::Text calcText("Calculate and Create List", font, 18);
calcText.setFillColor(sf::Color::Black);
calcText.setPosition(160, 540);
//where the listings are being printed
sf::RectangleShape listPanel(sf::Vector2f(1300, 600));
listPanel.setPosition(530, 160);
listPanel.setFillColor(sf::Color(240, 240, 240));
listPanel.setOutlineColor(sf::Color::Black);
listPanel.setOutlineThickness(2);

vector<sf::Text> listingTexts;
vector<Property_Listing> listings = minHeapStruct.getCurrentHeap();


int activeBox = -1;
sf::Clock cursorClock;
bool showCursor = false;


sf::Text subTitle("", font, 22);
subTitle.setFillColor(sf::Color(70, 70, 70));
subTitle.setPosition(980, 120);

while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            //tab switching
            for (int i = 0; i < tabButtons.size(); i++) {
                if (tabButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentTab = i;
                    listingTexts.clear();
                }
            }

            //checks if an input button was clicked on
            activeBox = -1;
            for (int i = 0; i < 7; i++) {
                if (inputBoxes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    activeBox = i;
                }
            }

            //creates list when button is clicked
            if (calcButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                listingTexts.clear();
                vector<Property_Listing> sorted = listings;
                int minPrice = 0;
                int maxPrice = INT_MAX;
                float minBedrooms = 0;
                float maxBedrooms = FLT_MAX;
                float minBath = 0;
                float maxBath = FLT_MAX;
                if (!inputStrings[0].empty()) {
                    minPrice = stoi(inputStrings[0]);
                }
                if (!inputStrings[1].empty()) {
                    maxPrice = stoi(inputStrings[1]);
                }
                if (!inputStrings[2].empty()) {
                    minBedrooms = stoi(inputStrings[2]);
                }
                if (!inputStrings[3].empty()) {
                    maxBedrooms = stoi(inputStrings[3]);
                }
                if (!inputStrings[4].empty()) {
                    minBath = stof(inputStrings[4]);
                }
                if (!inputStrings[5].empty()) {
                    maxBath = stof(inputStrings[5]);
                }

                //each type of sort per tab
                if (currentTab == 1) {
                    minHeapStruct.heapSortAscendingPrice();
                    sorted = minHeapStruct.getCurrentHeap();
                    subTitle.setString("Sorted by Price (Ascending)");
                    bPlusTree tree(256, "price", "Airbnb_data.csv");
                }
                else if (currentTab == 2) {
                    minHeapStruct.heapSortAscendingBedrooms();
                    sorted = minHeapStruct.getCurrentHeap();
                    subTitle.setString("Sorted by Bedrooms (Ascending)");
                    bPlusTree tree(256, "bedrooms", "Airbnb_data.csv");
                }
                else if (currentTab == 3) {
                    minHeapStruct.heapSortAscendingBathrooms();
                    sorted = minHeapStruct.getCurrentHeap();
                    subTitle.setString("Sorted by Bathrooms (Ascending)");
                    bPlusTree tree(256, "bathrooms", "Airbnb_data.csv");
                }
                else if (currentTab == 4) {
                    string location = inputStrings[6];
                    sorted.clear();
                    subTitle.setString("Filtered by Location: " + location);

                    //matches listing location area even if its lowercase/uppercase

                    for (auto &p : listings) {
                        string lowerCity = p.city;
                        string lowerArea = p.area;
                        string lowerLoc = location;
                        for (int i=0; i<lowerCity.size(); i++) {
                            lowerCity[i] = tolower(lowerCity[i]);
                        }
                        for (int i =0; i<lowerArea.size(); i++) {
                            lowerArea[i] = tolower(lowerArea[i]);
                        }
                        for (int i=0; i<lowerLoc.size(); i++) {
                            lowerLoc[i] = tolower(lowerLoc[i]);
                        }
                        if (lowerCity.find(lowerLoc) != string::npos || lowerArea.find(lowerLoc) != string::npos)
                            sorted.push_back(p);
                    }
                }
                //prints the filtered vector
                vector<Property_Listing> filtered;
                for (auto &p : sorted) {
                    bool priceOK = (p.price >= minPrice && p.price <= maxPrice);
                    bool bathOK = (p.bathrooms >= minBath && p.bathrooms <= maxBath);
                    bool bedroomOK = (p.bedrooms >= minBedrooms && p.bedrooms <= maxBedrooms);
                    if (priceOK && bathOK&& bedroomOK) {
                        filtered.push_back(p);
                    }
                }
                //if no listings match specfied filters
                if (filtered.empty()) {
                    sf::Text none("No listings match your filters.", font, 18);
                    none.setFillColor(sf::Color::Red);
                    none.setPosition(540, 170);
                    listingTexts.push_back(none);
                } else {
                    //prints listings
                    for (int i = 0; i < filtered.size() && i < 14; i++) {
                        string info = filtered[i].name +
                            " | $" + to_string(filtered[i].price) +
                            " | Bedrooms: " + to_string(filtered[i].bedrooms) +
                            " | Baths: " + to_string(filtered[i].bathrooms) +
                            " | Acc: " + to_string(filtered[i].accommodates) +
                            " | Reviews: " + to_string(filtered[i].total_reviewers) +
                            " | " + filtered[i].city +
                            ", " + filtered[i].area;

                        sf::Text listing(info, font, 16);
                        listing.setFillColor(sf::Color::Black);
                        listing.setPosition(540, 170 + i * 30);
                        listingTexts.push_back(listing);
                    }
                }
            }
        }

        //typing in input box
        if (event.type == sf::Event::TextEntered && activeBox != -1) {
            if (event.text.unicode == 8) { // backspace
                if (!inputStrings[activeBox].empty())
                    inputStrings[activeBox].pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode != 13) {
                inputStrings[activeBox] += static_cast<char>(event.text.unicode);
            }
            inputTexts[activeBox].setString(inputStrings[activeBox]);
        }
    }

    if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        showCursor = !showCursor;
        cursorClock.restart();
    }

    window.clear(sf::Color::White);

    for (int i = 0; i < tabButtons.size(); i++) {
        tabButtons[i].setFillColor(i == currentTab ? sf::Color(180, 200, 255) : sf::Color(230, 230, 230));
        window.draw(tabButtons[i]);
        window.draw(tabTexts[i]);
    }

    window.draw(title);

    if (currentTab == 0) {
        sf::Text menuText("Welcome to Home-inator!\nSelect a tab to sort or search listings.", font, 22);
        menuText.setFillColor(sf::Color::Black);
        menuText.setPosition(330, 300);
        window.draw(menuText);
    } else {
        //only shows relevant input boxes
        for (int i = 0; i < 7; i++) {
            bool visible = false;
            if (currentTab == 1 && (i == 0 || i == 1)) {
                visible = true;
            }
            if (currentTab == 2 && (i == 2 || i == 3)) {
                visible = true;
            }
            if (currentTab == 3 && (i == 4 || i == 5)) {
                visible = true;
            }
            if (currentTab == 4 && i == 6) {
                visible = true;
            }
            //draws the correct input boxes for each tab
            if (visible) {
                inputBoxes[i].setOutlineColor(i == activeBox ? sf::Color::Blue : sf::Color::Black);
                window.draw(inputBoxes[i]);
                window.draw(inputTexts[i]);
                window.draw(labels[i]);
                if (i == activeBox && showCursor) {
                    sf::RectangleShape cursor(sf::Vector2f(2, 20));
                    cursor.setFillColor(sf::Color::Black);
                    cursor.setPosition(inputTexts[i].getPosition().x + inputTexts[i].getLocalBounds().width + 2,
                                       inputTexts[i].getPosition().y + 5);
                    window.draw(cursor);
                }
            }
        }

        window.draw(calcButton);
        window.draw(calcText);
        window.draw(listPanel);
        window.draw(subTitle);

        for (auto &t : listingTexts)
            window.draw(t);
    }

    window.display();
}

    return 0;
}
