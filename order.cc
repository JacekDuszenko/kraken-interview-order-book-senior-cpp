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

kraken::CancelOrder::CancelOrder(int user_id, int user_order_id) : user_id_(
    user_id), user_order_id_(user_order_id) {}
