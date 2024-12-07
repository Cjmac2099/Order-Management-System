#include "warehouse.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

// take given string and produce hash value
uint64_t warehouse::fnv_hash(string sku) {
  // hash := FNV_OFFSET_BASIS
  uint64_t hash = FNV_OFFSET_BASIS;

  // for each character to be hashed do
  for(char c : sku){
    hash ^= static_cast<uint64_t>(c); 
    hash *= FNV_PRIME;
  }

  // Log output
  lout << "Computed hash '" << hash << "' for SKU '" << sku + "'" << endl;

  return hash;
}

// inserts a given product into hash table
void warehouse::insert(const product &_product) {
  // compute hash value
  uint64_t hash = fnv_hash(_product.sku);
  uint64_t index = hash % this->warehouse_capacity;

  // Log ouput
  lout << "Computed index '" << index << "' for SKU '" << _product.sku + "'" << endl;

  // check for collision
  if(this->products[index].has_value()){
    lout << "Collision detected!" << endl;
    expand();
    insert(_product);
  } else {
    this->products[index] = _product;
  }
}

// expand internal produts array by Block Size
void warehouse::expand() {
  // save the address and capacity of old array
  std::optional<product>* temp  = new optional<product>[this->warehouse_capacity];
  size_t oldCapacity = this->warehouse_capacity;
  
  for(size_t i = 0; i < oldCapacity; i++){
    temp[i] = this->products[i];
  }

  // allocate a new larger array and update internal capacity
  delete[] this->products;
  this->warehouse_capacity += WAREHOUSE_BLOCK_SIZE;
  this->products = new optional<product>[this->warehouse_capacity];

  // log prompt
  lout << "Warehouse capacity resized to " << this->warehouse_capacity << ", re-shelving products!" << endl;

  // loop over old array and re-insert elements
  for(size_t i = 0; i < oldCapacity; i++){
    if(temp[i].has_value()){
      insert(temp[i].value());
    }
  }

  // deallocate old array
  delete[] temp;
}

// retrieve product for given SKU
product &warehouse::at(string sku) {
  // get position
  uint64_t hash = fnv_hash(sku);
  uint64_t index = hash % this->warehouse_capacity;
  lout << "Computed index '" << index << "' for SKU '" << sku + "'" << endl;

  // get product and return
  if(this->products[index].has_value()){
    return this->products[index].value();
  } 

  // if no product found -> throw exception
  throw std::out_of_range("SKU not found! Try again.");
}
