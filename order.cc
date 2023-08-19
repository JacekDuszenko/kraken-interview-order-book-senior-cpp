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

int counter = 0;

std::vector<std::string>
kraken::NewOrder::Execute(const std::unique_ptr<MatchingEngine> &engine) {
    auto order_book = engine->GetOrderBookByTicker(ticker_);
    engine->IndexTickerByOrderId(user_id_, user_order_id_, ticker_);
    std::vector<std::string> logs = {};

    AcknowledgeNewOrder(logs);
    AddOrder(order_book, logs);
    std::vector<std::string> settlement_logs = order_book->SettleTransactions();
    logs.insert(logs.end(), settlement_logs.begin(), settlement_logs.end());

    return logs;
}

void kraken::NewOrder::AddOrder(std::shared_ptr<kraken::OrderBook> &order_book, std::vector<std::string> &logs) const {
    std::string top_of_book_change;
    if (BID == order_side_) {
        top_of_book_change = order_book->AddBid(OrderMetadata(user_order_id_, user_id_, price_, quantity_, ++counter));
    } else if (ASK == order_side_) {
        top_of_book_change = order_book->AddAsk(OrderMetadata(user_order_id_, user_id_, price_, quantity_, ++counter));
    }
    if (!top_of_book_change.empty()) {
        logs.push_back(top_of_book_change);
    }
}

void kraken::NewOrder::AcknowledgeNewOrder(std::vector<std::string> &logs) const {
    logs.push_back("A, " + std::to_string(
            user_id_) + ", " + std::to_string(user_order_id_));
}

std::vector<std::string>
kraken::CancelOrder::Execute(const std::unique_ptr<MatchingEngine> &engine) {
    auto ticker = engine->GetTickerByOrderId(user_id_, user_order_id_);
    auto order_book = engine->GetOrderBookByTicker(ticker);
    std::vector<std::string> logs = {};
    AcknowledgeCancelOrder(logs);
    std::string top_of_book_change = order_book->CancelOrder(user_order_id_, user_id_);
    if (!top_of_book_change.empty()) {
        logs.push_back(top_of_book_change);
    }
    return logs;
}

void kraken::CancelOrder::AcknowledgeCancelOrder(std::vector<std::string> &logs) const {
    logs.push_back("C, " + std::to_string(
            user_id_) + ", " + std::to_string(user_order_id_));
}

std::vector<std::string> kraken::FlushOrderbook::Execute(
        const std::unique_ptr<MatchingEngine> &engine) {
    engine->Flush();
    return {};
}

kraken::Order::~Order() = default;
