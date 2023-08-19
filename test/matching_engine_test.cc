
#include <gtest/gtest.h>
#include <iostream>
#include "matching_engine.h"
#include "order_book.h"
#include "order.h"

TEST(MatchingEngine, FlushOrderbook) {
    std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > order_books;
    auto matching_engine_ptr = std::make_unique<kraken::MatchingEngine>(order_books);

    auto execution_results = kraken::FlushOrderbook().Execute(matching_engine_ptr);

    ASSERT_EQ(execution_results, "flushed");
    ASSERT_TRUE(order_books.empty())
}
