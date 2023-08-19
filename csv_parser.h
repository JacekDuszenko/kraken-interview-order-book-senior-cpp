#ifndef CSV_PARSER_H_
#define CSV_PARSER_H_

#include "order.h"
namespace kraken {
class CsvParser {
  Order* Parse(const std::string& csv_data);
};
} // namespace kraken

#endif //CSV_PARSER_H_
