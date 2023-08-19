#include "csv_parser.h"
#include <unordered_map>
#include <vector>

std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> results;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    results.push_back(token);
  }

  results.push_back(s.substr(pos_start));
  return results;
}

std::optional<kraken::OrderSide>
OrderSideFromString(const std::string &orderStr) {
  static std::unordered_map<std::string, kraken::OrderSide> const table = {
      {"B", kraken::OrderSide::BID}, {"S", kraken::OrderSide::ASK}};
  auto it = table.find(orderStr);
  if (it != table.end()) {
    return it->second;
  }
  return std::nullopt;
}

void ExitWithError(const std::string &msg) {
  std::cerr << msg << std::endl;
  exit(1);
}

kraken::Order *kraken::CsvParser::Parse(const std::string &csv_data) {
  char type = csv_data[0];

  switch (type) {
  case 'N': {
    auto split_data = split(csv_data.substr(2), ",");
    int user_id = std::stoi(split_data[0]);
    std::string ticker = split_data[1];
    int price = std::stoi(split_data[2]);
    int quantity = std::stoi(split_data[3]);
    std::optional<kraken::OrderSide> side = OrderSideFromString(split_data[4]);
    if (!side) {
      ExitWithError("invalid side");
    }
    int user_order_id = std::stoi(split_data[5]);
    return new NewOrder(user_id, ticker, price, quantity, side.value(),
                        user_order_id);
  }
  case 'C': {
    auto split_data = split(csv_data.substr(2), ",");
    int user_id = std::stoi(split_data[0]);
    int user_order_id = std::stoi(split_data[1]);
    return new CancelOrder(user_id, user_order_id);
  }
  case 'F': {
    return new FlushOrderbook();
  }
  default: {
    ExitWithError("invalid data");
  }
  }
  return nullptr;
}
