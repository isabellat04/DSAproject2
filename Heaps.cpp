#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
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
