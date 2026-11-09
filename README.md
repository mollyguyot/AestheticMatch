# AestheticMatch

AestheticMatch is a C++ program that helps users find clothing products based on their preferences and budget. Users can filter clothing by occasion, category, color, and maximum price.

The project uses Quick Sort, implemented from scratch, and the C++ library's stable_sort function to sort clothing products by price.

## Features

- Search for clothing by occasion, including gameday, athleisure, and date functions
- Filter by clothing category and color
- Set a maximum budget
- Sort products by price using Quick Sort or stable_sort
- Compare sorting execution times
- View up to 20 matching products
- Clear filters and start a new search

## Languages and Tools

- C++
- Python
- CSV files
- GitHub

## Dataset

The project uses clothing information from the H&M Personalized Fashion Recommendations dataset. A Python script combines product information with transaction prices to create products_hm.csv.

The prices are scaled estimates based on transaction data, not verified retail prices.

## How to Run

1. Download or clone this repository.
2. Make sure main.cpp and products_hm.csv are in the same folder.
3. Open a terminal in the project folder.
4. Compile the program:

   `g++ -std=c++17 main.cpp -o aestheticmatch`

5. Run the program:

   `./aestheticmatch`

6. Follow the menu to select filters and sorting options.

## Algorithms

**Quick Sort:** Implemented manually using a partition function and recursion.

**Library Stable Sort:** Uses the C++ stable_sort function for comparison.

Both algorithms sort products from lowest to highest price.

## Course

COP3530 - Data Structures and Algorithms

University of Florida