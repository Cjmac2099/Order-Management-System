#ifndef WAREHOUSE_HPP_INCLUDED
#define WAREHOUSE_HPP_INCLUDED
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>

const size_t WAREHOUSE_BLOCK_SIZE = 16;
const uint64_t FNV_PRIME = 1099511628211ULL;
const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
const std::string log_filename = "warehouse.log";

struct product {
  std::string sku;
  std::string name;
  double price_per_pound;
};

class warehouse {
private:
  std::ofstream lout;
  std::optional<product> *products;
  size_t warehouse_capacity;
  uint64_t fnv_hash(std::string);
  void expand();

public:
  warehouse()
      : lout(log_filename),
        products(new std::optional<product>[WAREHOUSE_BLOCK_SIZE]),
        warehouse_capacity(WAREHOUSE_BLOCK_SIZE) {}
  ~warehouse() { delete[] products; }
  void insert(const product &);
  product &at(std::string);
};

#endif /* WAREHOUSE_HPP_INCLUDED */
