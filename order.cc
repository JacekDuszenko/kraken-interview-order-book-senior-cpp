#include "order.h"
#include <iostream>

kraken::NewOrder::NewOrder(int user_id, const std::string &ticker, int price,
                           int quantity, kraken::OrderSide side,
                           int user_order_id)
        : user_id_(user_id), ticker_(ticker), price_(price), quantity_(quantity),
          order_side_(side), user_order_id_(user_order_id) {}

std::string kraken::NewOrder::Stringified() {
    return "NewOrder[user_id: " + std::to_string(user_id_) +
           ", ticker: " + ticker_ + ", price: " + std::to_string(price_) +
           ", quantity: " + std::to_string(quantity_) +
           ", order_side: " + (order_side_ == OrderSide::ASK ? "ASK" : "BID") +
           ", user_order_id: " + std::to_string(user_order_id_) + "]";
}

std::string kraken::CancelOrder::Stringified() {
    return "CancelOrder[user_id: " + std::to_string(user_id_) +
           ", user_order_id: " + std::to_string(user_order_id_) + "]";
}

kraken::CancelOrder::CancelOrder(int user_id, int user_order_id)
        : user_id_(user_id), user_order_id_(user_order_id) {}

std::string kraken::FlushOrderbook::Stringified() { return "FlushOrderbook[]"; }

std::string
kraken::NewOrder::Execute(const std::unique_ptr<MatchingEngine> &engine) {
    auto order_book = engine->GetOrderBookByTicker(ticker_);
}

std::string
kraken::CancelOrder::Execute(const std::unique_ptr<MatchingEngine> &engine) {}

std::string kraken::FlushOrderbook::Execute(
        const std::unique_ptr<MatchingEngine> &engine) {
    engine->Flush();
    return "flushed";
}

kraken::Order::~Order() = default;
