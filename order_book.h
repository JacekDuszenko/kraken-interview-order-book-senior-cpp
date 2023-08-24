#ifndef _ORDERBOOK_H_
#define _ORDERBOOK_H_

#include <unordered_map>
#include <vector>
#include<set>
#include<string>

namespace kraken {

    struct OrderMetadata {
        int order_id;
        int user_id;
        int price;
        int quantity;
        int arrived_at;

        OrderMetadata(int orderId, int userId, int price, int quantity, int arrived_at);

        bool operator==(const OrderMetadata &rhs) const;

        bool operator!=(const OrderMetadata &rhs) const;
    };

    struct BidComparator {
        bool operator()(const OrderMetadata& a, const OrderMetadata& b) const {
            if (a.price != b.price) {
                return b.price < a.price;
            }
            return a.arrived_at < b.arrived_at;
        }
    };
    struct AskComparator {
        bool operator()(const OrderMetadata& a, const OrderMetadata& b) const {
            if (a.price != b.price) {
                return a.price < b.price;
            }
            return a.arrived_at < b.arrived_at;
        }
    };

    class OrderBook {
    public:
        OrderBook();

        std::string AddBid(const OrderMetadata &bid_order_metadata);

        std::string AddAsk(const OrderMetadata &ask_order_metadata);

        std::string CancelOrder(int order_id, int user_id);

        std::vector<std::string> SettleTransactions();

    private:
        std::set<OrderMetadata, BidComparator> bids;
        std::set<OrderMetadata, AskComparator> asks;

        bool IsOrderExecutionPossible();
    };
} // namespace kraken

#endif //_ORDERBOOK_H_
