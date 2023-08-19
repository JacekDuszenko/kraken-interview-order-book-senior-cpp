#include "matching_engine.h"
#include "order_book.h"
#include <unordered_map>
#include<string>
#include <memory>


std::shared_ptr<kraken::OrderBook> kraken::MatchingEngine::GetOrderBookByTicker(const std::string &ticker) {
    if (order_books_.find(ticker) == order_books_.end()) {
        order_books_[ticker] = std::make_shared<kraken::OrderBook>();
    }
    return order_books_[ticker];
}

kraken::MatchingEngine::MatchingEngine(
        std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > &order_books) : order_books_(
        order_books) {}

void kraken::MatchingEngine::Flush() {
    order_books_.clear();
}

void kraken::MatchingEngine::IndexTickerByOrderId(int user_id, int order_id, const std::string &ticker) {
    order_to_ticker[std::to_string(user_id) + "#" + std::to_string(order_id)] = ticker;
}

std::string kraken::MatchingEngine::GetTickerByOrderId(int user_id, int order_id) {
    return order_to_ticker[std::to_string(user_id) + "#" + std::to_string(order_id)];
}


