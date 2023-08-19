
#include "order_book.h"
#include <string>

kraken::OrderBook::OrderBook() = default;

kraken::OrderMetadata::OrderMetadata(int orderId, int userId, int price, int quantity, int arrived_at) : order_id(
        orderId), user_id(userId), price(price), quantity(quantity), arrived_at(arrived_at) {}

bool kraken::OrderMetadata::operator==(const kraken::OrderMetadata &rhs) const {
    return order_id == rhs.order_id &&
           user_id == rhs.user_id;
}

bool kraken::OrderMetadata::operator!=(const kraken::OrderMetadata &rhs) const {
    return !(rhs == *this);
}

std::string kraken::OrderBook::AddBid(const kraken::OrderMetadata &bid_order_metadata) {
    if (bids.empty()) {
        bids.insert(bid_order_metadata);
        return "B, B, " + std::to_string(bid_order_metadata.price) + ", " + std::to_string(bid_order_metadata.quantity);
    }
    auto first_bid = *bids.begin();
    bids.insert(bid_order_metadata);
    auto first_bid_after_insert = *bids.begin();
    if (first_bid == first_bid_after_insert) {
        return {};
    }
    return "B, B, " + std::to_string(first_bid_after_insert.price) + ", " +
           std::to_string(first_bid_after_insert.quantity);
}

std::string kraken::OrderBook::AddAsk(const kraken::OrderMetadata &ask_order_metadata) {
    if (asks.empty()) {
        asks.insert(ask_order_metadata);
        return "B, S, " + std::to_string(ask_order_metadata.price) + ", " + std::to_string(ask_order_metadata.quantity);
    }
    auto first_ask = *asks.begin();
    asks.insert(ask_order_metadata);
    auto first_ask_after_insert = *asks.begin();
    if (first_ask == first_ask_after_insert) {
        return {};
    }
    return "B, S, " + std::to_string(first_ask_after_insert.price) + ", " +
           std::to_string(first_ask_after_insert.quantity);
}

std::string kraken::OrderBook::CancelOrder(int order_id, int user_id) {
    OrderMetadata search_key = OrderMetadata(order_id, user_id, 0, 0, 0);
    auto it = bids.find(search_key);
    if (it != bids.end()) {
        auto first_bid = *bids.begin();
        bids.erase(it);
        if (bids.empty()) {
            return "B, B, -, -";
        }
        auto first_bid_after_removal = *bids.begin();
        if (first_bid == first_bid_after_removal) {
            return {};
        }
        return "B, B, " + std::to_string(first_bid_after_removal.price) + ", " +
               std::to_string(first_bid_after_removal.quantity);
    }
    it = asks.find(search_key);
    if (it != asks.end()) {
        auto first_ask = *asks.begin();
        asks.erase(it);
        if (asks.empty()) {
            return "B, S, -, -";
        }
        auto first_ask_after_removal = *asks.begin();
        if (first_ask == first_ask_after_removal) {
            return {};
        }
        return "B, S, " + std::to_string(first_ask_after_removal.price) + ", " +
               std::to_string(first_ask_after_removal.quantity);
    }
    return {};
}

std::vector<std::string> kraken::OrderBook::SettleTransactions() {
    return std::vector<std::string>();
}
