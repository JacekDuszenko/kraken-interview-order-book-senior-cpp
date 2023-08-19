#include "order.h"
#include<iostream>
#include <utility>

kraken::NewOrder::NewOrder(int user_id,
                           const std::string& ticker,
                           int price,
                           int quantity,
                           kraken::OrderSide side,
                           int user_order_id)
    : user_id_(user_id),
      ticker_(ticker),
      price_(price),
      quantity_(quantity),
      order_side_(side),
      user_order_id_(user_order_id) {}

std::string kraken::NewOrder::Stringified() {
  return "NewOrder[user_id: " + std::to_string(user_id_) + ", ticker: "
      + ticker_
      + ", price: " + std::to_string(price_) + ", quantity: "
      + std::to_string(quantity_) + ", order_side: "
      + (order_side_ == OrderSide::ASK ? "ASK" : "BID") + ", user_order_id: "
      + std::to_string(user_order_id_) + "]";
}

std::string kraken::CancelOrder::Stringified() {
  return "CancelOrder[user_id: " + std::to_string(user_id_)
      + ", user_order_id: "
      + std::to_string(user_order_id_) + "]";
}

kraken::CancelOrder::CancelOrder(int user_id, int user_order_id) : user_id_(
    user_id), user_order_id_(user_order_id) {}

std::string kraken::FlushOrderbook::Stringified() {
  return "FlushOrderbook[]";
}
