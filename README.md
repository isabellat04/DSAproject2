<H1> COP3530 - Project 2 </h1>

# Housing Property Finder  
### Team Name: Home-inators  
**Team Members:** Anveer Aulakh, Isabella Torres, Nancy Vutuan  
**Course:** COP 3530 – Data Structures and Algorithms  
**Project 2**
## Overview  
The **Housing Property Finder** is a C++ application designed to help users efficiently explore housing and rental properties. Using **data structures such as Heaps and B+ Trees**, the program allows users to quickly filter, compare, and visualize listings based on desired features like price, location, and room type.  

Our goal is to make the property-search process simpler, faster, and more insightful by organizing and visualizing data drawn from real Airbnb listings.

## Features  
- Filter properties based on **user-specified parameters** (price range, location, room type, number of bathrooms, etc.).  
- **Find optimal listings** (e.g., cheapest, location, or a specfied amount of bedrooms/bathrroms) using **Heap** operations.  
- **Efficiently search** within a price range using a **B+ Tree**, which links properties through sorted leaf nodes for quick range queries.  
- **Interactive visualization** using **SFML graphics**, showing listings and comparisons through a simple interface.  
- **Performance comparison** between the Heap and B+ Tree structures by printing the time it takes to build in the console.

## Data Source  
We use a publicly available Airbnb dataset:  
**[Airbnb Site Hotel Dataset – Kaggle](https://www.kaggle.com/datasets/willianoliveiragibin/airbnb-site-hotel)**  

Key dataset fields include:
- name – property name (string)  
- city – location (string)  
- price – nightly cost (float)  
- room_type – type of property (string)  
- reviews – number of reviews (int)  
- accommodations, bathCount, bedroomCount, beds – additional features (various numeric types)

## Data Structures & Algorithms  
### **Heap**
Used to efficiently retrieve the *minimum* or *maximum* property for a given feature (e.g., lowest price, largest number of bedrooms).  
- Supports finding the *kth* largest/smallest listing.  
- Handles ties (e.g., multiple listings with the same feature value).

### **B+ Tree**
Used for **range queries**, such as finding all properties within a price range.  
- Leaf nodes form a linked list for efficient traversal.  
- Outperforms standard B-Trees for this type of query.  
  

### How to Build & Run  
Here is a simple build & run process (adjust path/names as necessary):  

```bash
# Clone the repository  
git clone https://github.com/isabellat04/DSAproject2.git  
cd DSAproject2  

# Compile (example using g++)  
g++ -std=c++17 -O2 -o project2 main.cpp bPlusTree.cpp Heaps.cpp  

# Run  
./project2 resources/Airbnb_data_edited.csv  

