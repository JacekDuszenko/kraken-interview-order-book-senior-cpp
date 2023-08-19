#ifndef ORDER_H_
#define ORDER_H_

#include <iostream>
#include <memory>

#include "matching_engine.h"

namespace kraken {

enum OrderSide { BID, ASK };

class Order {
public:
  virtual std::string Stringified() = 0;
  virtual std::string Execute(const std::unique_ptr<MatchingEngine>&) = 0;
  virtual ~Order();
};

class NewOrder : public Order {
public:
  NewOrder(int user_id, const std::string &ticker, int price, int quantity,
           OrderSide side, int user_order_id);
  std::string Stringified() override;
  std::string Execute(const std::unique_ptr<MatchingEngine>&) override;

private:
  int user_id_;
  const std::string &ticker_;
  int price_;
  int quantity_;
  OrderSide order_side_;
  int user_order_id_;
};

class CancelOrder : public Order {
public:
  CancelOrder(int user_id, int user_order_id);
  std::string Stringified() override;
  std::string Execute(const std::unique_ptr<MatchingEngine>&) override;

private:
  int user_id_;
  int user_order_id_;
};

class FlushOrderbook : public Order {
public:
  explicit FlushOrderbook() = default;
  std::string Stringified() override;
  std::string Execute(const std::unique_ptr<MatchingEngine>&) override;
};

} // namespace kraken

#endif // ORDER_H_
