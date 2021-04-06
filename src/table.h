#ifndef CODE_TABLE_H
#define CODE_TABLE_H

#include <iostream>

// #include <map>
// #define create_table(table)   std::map<uint8_t, int> table

/* 数组实现hash表 */
#define create_table(table) char table[256];

#define init_table()    table['A'] = 0; \
                        table['T'] = 1; \
                        table['C'] = 2; \
                        table['G'] = 3; \
                        table['N'] = 4


#define table(x) table[(uint8_t)x]


#endif //CODE_TABLE_H
