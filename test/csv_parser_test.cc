#include "csv_parser.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(CsvParser, NewOrder) {
  kraken::CsvParser parser;

  auto order = parser.Parse(std::string("N,1,IBM,10,100,B,1"));

  EXPECT_EQ(order->Stringified(),
            "NewOrder[user_id: 1, ticker: IBM, price: 10, quantity: 100, "
            "order_side: BID, user_order_id: 1]");
}

TEST(CsvParser, CancelOrder) {
  kraken::CsvParser parser;

  auto order = parser.Parse(std::string("C,2,102"));

  EXPECT_EQ(order->Stringified(),
            "CancelOrder[user_id: 2, user_order_id: 102]");
}

TEST(CsvParser, FlushOrderbook) {
  kraken::CsvParser parser;

  auto order = parser.Parse(std::string("F"));

  EXPECT_EQ(order->Stringified(), "FlushOrderbook[]");
}