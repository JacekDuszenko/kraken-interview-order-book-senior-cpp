#include "matching_engine.h"
#include<string>


std::shared_ptr<kraken::OrderBook> kraken::MatchingEngine::GetOrderBookByTicker(const std::string &ticker) {
    if (order_books_.find(ticker) == order_books_.end()) {
        order_books_[ticker] = std::make_shared<kraken::OrderBook>();
    }
    return order_books_[ticker];
}

kraken::MatchingEngine::MatchingEngine(
        std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook>> &order_books) : order_books_(order_books) {}

std::shared_ptr<kraken::OrderBook> kraken::MatchingEngine::Flush() {
    order_books_.clear();
}

