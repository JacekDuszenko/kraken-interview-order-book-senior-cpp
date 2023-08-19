#ifndef _MATCHING_ENGINE_H_
#define _MATCHING_ENGINE_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include<string>

#include "order_book.h"

namespace kraken {

    class MatchingEngine {
    public:
        explicit MatchingEngine(
                std::unordered_map<std::string, std::shared_ptr<OrderBook> > &order_books);

        std::shared_ptr<OrderBook> GetOrderBookByTicker(const std::string &ticker);

        void Flush();

        void IndexTickerByOrderId(int user_id, int order_id, const std::string& ticker);

        std::string GetTickerByOrderId(int user_id, int order_id);


    private:
        std::unordered_map<std::string, std::shared_ptr<OrderBook> > &order_books_;
        std::unordered_map<std::string, std::string> order_to_ticker;
    };

} // namespace kraken

#endif //_MATCHING_ENGINE_H_
