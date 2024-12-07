#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "warehouse.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " [catalog]" << endl;
    return 1;
  }

  ifstream catalog_file(argv[1]);
  if (!catalog_file) {
    cout << "Error opening file: '" << argv[1] << "'" << endl;
    return 1;
  }

  cout << "Loading catalog..." << endl;
  warehouse catalog;

  // LOAD CATALOG FILE HERE
  // load catalog file into custom hash table
  string line;
  getline(catalog_file, line);  // skip first line

  // read rest of catalog
  while(getline(catalog_file,line)){
    if(line.empty()){ // skip empty line
      break;  
    }

    string name, price, sku;
    stringstream ss(line);

    getline(ss, name, ',');
    getline(ss, price, ',');
    getline(ss, sku, ',');

    // create product to add
    product temp;
    temp.name = name; temp.price_per_pound = stod(price); temp.sku = sku;

    // add to map
    catalog.insert(temp);
  }
  //cout << "test2" << endl;
  catalog_file.close();
  
  // user input
  cout << fixed << setprecision(2);
  vector<string> order_skus;
  vector<double> order_qtys;
  cout << "Enter 'END' to finish order." << endl;
  do {
    string sku;
    double quantity = 0.0;
    
    // READ IN ORDER HERE
    // sku
    cout << "SKU> ";
    cin >> sku;
    cout << endl;

    if(sku == "END" || sku == "end"){ // 'end' bc I want to
      break;         // exit loop
    }
    cin.clear();
    cin.ignore(256, '\n');

    // attempt to find SKU
    try{
      product temp = catalog.at(sku);
      cout << "SKU matches product " << temp.name << ", price per pound: $" 
      << temp.price_per_pound << endl;

      // quantity
      cout << "Quantity> ";
      cin >> quantity;
      cout << endl;

      if(cin.fail()){
        cin.clear();
        cin.ignore(256, '\n');
        throw std::invalid_argument("Invalid quantity. Try Again.");
      }

      cin.clear();
      cin.ignore(256, '\n');

      order_skus.push_back(sku);
      order_qtys.push_back(quantity);

    } catch(const std::exception& e) {
      // catch exceptions and re-prompt
      cout << "Error: " << e.what() << endl;
    }
  } while (true);

  // Print final output
  cout << "Printing order summary:" << endl;
  for (size_t i = 0; i < order_skus.size(); i++) {
    product current = catalog.at(order_skus[i]);
    cout << order_qtys[i] << "lbs " << current.name << ": $"
         << order_qtys[i] * current.price_per_pound << endl;
  }

  return 0;
}
