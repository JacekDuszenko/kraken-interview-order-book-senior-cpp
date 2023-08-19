#ifndef _MATCHING_ENGINE_H_
#define _MATCHING_ENGINE_H_

#include "order_book.h"
#include <unordered_map>
#include <vector>

namespace kraken {

    class MatchingEngine {
    public:
        explicit MatchingEngine(
                std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > &order_books);

        std::shared_ptr<kraken::OrderBook> GetOrderBookByTicker(const std::string &ticker);
        std::shared_ptr<kraken::OrderBook> Flush();


    private:
        std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > &order_books_;
    };

} // namespace kraken

#endif //_MATCHING_ENGINE_H_
