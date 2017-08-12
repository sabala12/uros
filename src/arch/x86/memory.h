//
// Created by eran on 12/08/17.
//

#ifndef UROS_MEMORY_H
#define UROS_MEMORY_H

extern uint32_t p4_table[];
extern uint32_t p3_table[];
extern uint32_t p2_table[];

namespace memory
{
    void setup_page_table()
    {
        uint32_t p4_table_entry = (uint32_t )p3_table << 12;
        p4_table_entry &= 0x3;
        *p4_table = p4_table_entry ;
    }
}

#endif //UROS_MEMORY_H
