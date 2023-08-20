


#include <gtest/gtest.h>
#include "matching_engine.h"
#include "order_book.h"
#include "csv_parser.h"
#include <vector>
#include <string>

kraken::CsvParser csv_parser;

std::vector<std::string> RunOrderBook(const std::vector<std::string> &lines) {
    std::vector<std::string> res;
    std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > order_books;
    auto matching_engine_ptr = std::make_unique<kraken::MatchingEngine>(order_books);
    for (auto &l: lines) {
        auto order = csv_parser.Parse(l);
        auto execution_results = order->Execute(matching_engine_ptr);
        res.insert(res.end(), execution_results.begin(), execution_results.end());
    }
    return res;
}

TEST(OrderBook, BalancedBook) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,1,IBM,11,100,B,3",
                                    "N,2,IBM,10,100,S,103",
                                    "N,1,IBM,10,100,B,4",
                                    "N,2,IBM,11,100,S,104"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 1, 3",
            "T, 1, 3, 2, 102, 11, 100",
            "B, S, 12, 100",
            "A, 2, 103",
            "T, 1, 1, 2, 103, 10, 100",
            "B, B, 9, 100",
            "A, 1, 4",
            "B, B, 10, 100",
            "A, 2, 104",
            "B, S, 11, 100"
    }));
}

TEST(OrderBook, ShallowAsk) {
    auto res = RunOrderBook({
                                    "N,1,VAL,10,100,B,1",
                                    "N,2,VAL,9,100,B,101",
                                    "N,2,VAL,11,100,S,102",
                                    "N,1,VAL,11,100,B,2",
                                    "N,2,VAL,11,100,S,103"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 1, 2",
            "T, 1, 2, 2, 102, 11, 100",
            "B, S, -, -",
            "A, 2, 103",
            "B, S, 11, 100"
    }));
}

TEST(OrderBook, BalancedBookLimitAboveBestAsk) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,1,IBM,12,100,B,103"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 1, 103",
            "T, 1, 103, 2, 102, 11, 100",
            "B, S, 12, 100"
    }));
}

TEST(OrderBook, BalancedBookMarketBuy) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,1,IBM,0,100,B,3"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 1, 3",
            "T, 1, 3, 2, 102, 11, 100",
            "B, S, 12, 100"
    }));
}

TEST(OrderBook, BalancedBookMarketSellPartial) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,2,IBM,0,20,S,103",
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 2, 103",
            "T, 1, 1, 2, 103, 10, 20",
            "B, B, 10, 80"
    }));
}

TEST(OrderBook, BalancedBookLimitSellPartial) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,2,IBM,10,20,S,103"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "A, 2, 103",
            "T, 1, 1, 2, 103, 10, 20",
            "B, B, 10, 80",
    }));
}

TEST(OrderBook, BalancedBookMultipleOffersAtBestBid) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "N,2,IBM,10,50,B,103",
                                    "N,1,IBM,11,50,S,3",
                                    "N,1,IBM,11,100,B,4",
                                    "N,2,IBM,10,100,S,104",
                            });
    EXPECT_EQ(res, (std::vector<std::string>{"A, 1, 1", "B, B, 10, 100", "A, 1, 2", "B, S, 12, 100", "A, 2, 101",
                                             "A, 2, 102", "B, S, 11, 100", "A, 2, 103", "A, 1, 3", "A, 1, 4",
                                             "T, 1, 4, 2, 102, 11, 100", "B, S, 11, 50", "A, 2, 104",
                                             "T, 1, 1, 2, 104, 10, 100", "B, B, 10, 50"}));
}

TEST(OrderBook, BalancedBookCancelBehindBestBid) {
    auto res = RunOrderBook({
                                    "N,1,IBM,10,100,B,1",
                                    "N,1,IBM,12,100,S,2",
                                    "N,2,IBM,9,100,B,101",
                                    "N,2,IBM,11,100,S,102",
                                    "C,1,2",
                                    "C,2,101"
                            });
    EXPECT_EQ(res, (std::vector<std::string>{
            "A, 1, 1",
            "B, B, 10, 100",
            "A, 1, 2",
            "B, S, 12, 100",
            "A, 2, 101",
            "A, 2, 102",
            "B, S, 11, 100",
            "C, 1, 2",
            "C, 2, 101",
    }));
}

