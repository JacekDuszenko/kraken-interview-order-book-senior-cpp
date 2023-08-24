
#include "order_book.h"
#include <string>
#include<stdexcept>
#include<climits>


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
    if (first_bid == first_bid_after_insert || IsOrderExecutionPossible()) {
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
    if (first_ask == first_ask_after_insert || IsOrderExecutionPossible()) {
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

std::string GetFormattedTransaction(kraken::OrderMetadata bid, kraken::OrderMetadata ask, int price, int quantity) {
    return "T, " + std::to_string(bid.user_id) + ", " + std::to_string(bid.order_id) + ", " +
           std::to_string(ask.user_id) + ", " + std::to_string(ask.order_id) + ", " + std::to_string(price) + ", " +
           std::to_string(quantity);

}

void update_asks_top_of_book(std::set<kraken::OrderMetadata, kraken::AskComparator> asks,
                             std::vector<std::string> &results) {
    if (asks.empty()) {
        results.emplace_back("B, S, -, -");
    } else {
        auto new_ask = *asks.begin();
        results.push_back(
                "B, S, " + std::to_string(new_ask.price) + ", " + std::to_string(new_ask.quantity));
    }
}

void update_bids_top_of_book(std::set<kraken::OrderMetadata, kraken::BidComparator> bids,
                             std::vector<std::string> &results) {
    if (bids.empty()) {
        results.emplace_back("B, B, -, -");
    } else {
        auto new_bid = *bids.begin();
        results.push_back(
                "B, B, " + std::to_string(new_bid.price) + ", " + std::to_string(new_bid.quantity));
    }
}

std::vector<std::string> kraken::OrderBook::SettleTransactions() {
    std::vector<std::string> results;
    while (IsOrderExecutionPossible()) {
        auto bid_it = bids.begin();
        auto ask_it = asks.begin();
        auto bid = *bid_it;
        auto ask = *ask_it;
        if (bid.price == INT_MAX && ask.price == 0) {
            throw std::invalid_argument("Market-market orders are not supported");
        }

        if (bid.price == INT_MAX) { // reduce limit - market to limit - limit
            bid.price = ask.price;
        }
        if (ask.price == 0) {
            ask.price = bid.price;
        }
        int price = bid.arrived_at > ask.arrived_at ? ask.price : bid.price;
        bids.erase(bid_it);
        asks.erase(ask_it);
        if (bid.quantity == ask.quantity) {
            results.push_back(GetFormattedTransaction(bid, ask, price, bid.quantity));
            if (bid.arrived_at > ask.arrived_at) {
                update_asks_top_of_book(asks, results);
            } else {
                update_bids_top_of_book(bids, results);
            }
        } else if (bid.quantity > ask.quantity) {
            results.push_back(GetFormattedTransaction(bid, ask, price, ask.quantity));
            bids.insert(OrderMetadata(bid.order_id, bid.user_id, bid.price, bid.quantity - ask.quantity,
                                      bid.arrived_at));
            update_bids_top_of_book(bids, results);
        } else {
            results.push_back(GetFormattedTransaction(bid, ask, price, bid.quantity));
            asks.insert(OrderMetadata(ask.order_id, ask.user_id, ask.price, ask.quantity - bid.quantity,
                                      bid.arrived_at));
            update_asks_top_of_book(asks, results);
        }
    }
    return results;
}

bool kraken::OrderBook::IsOrderExecutionPossible() {
    if (bids.empty() || asks.empty()) {
        return false;
    }
    auto first_bid = *bids.begin();
    auto first_ask = *asks.begin();

    return first_ask.price <= first_bid.price || first_ask.price == 0 || first_bid.price == 0;
}
