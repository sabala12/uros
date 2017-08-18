//
// Created by eran on 12/08/17.
//

#ifndef UROS_MEMORY_H
#define UROS_MEMORY_H

//#include <utils/fmt.h>

typedef uint64_t PTE;
typedef uint64_t PT;

extern PTE p4_table[];
extern PTE p3_table[];
extern PTE p2_table[];

//typedef void (*void_function_ptr)(void);
//extern void_function_ptr enable_paging;
//extern uint64_t enable_paging;

#define MAX_PT_ENTRIES      512

#define WRITABLE            0x0
#define EXECUTABLE          0x1
#define HUGE_PAGE           0x7

#define HUGE_PAGE_SIZE      0x200000

#define FOREACH_PT_ENTRY(pt, entry)                                                \
    for (int index = 0, entry = &pt[index]; index < MAX_PT_ENTRIES; index++)

namespace memory
{
    /* Identity map out initial page table.
     * PML4 first entry points to PDP first entry.
     * PDP first entry points to PD first entry.
     * All PDE entries are set to huge pages and map
     * the first 1GB. */
    void setup_initial_page_table()
    {
        // map p4 first entry.
        PTE p4_entry = *p3_table;
        p4_entry |= WRITABLE | EXECUTABLE;
        p4_table[0] = p4_entry;

        // map p3 first entry.
        PTE p3_entry = *p2_table;
        p3_entry |= WRITABLE | EXECUTABLE;
        p3_table[0] = p3_entry;

        // map all p2 entries as huge pages.
        for (int index = 0; index < MAX_PT_ENTRIES; index++) {
            PTE p2_entry = HUGE_PAGE_SIZE * index;
            p2_entry |= HUGE_PAGE | WRITABLE | EXECUTABLE;
            p2_table[index] = p2_entry;
        }
    }

    void start_paging()
    {
        //typedef void (*void_function_ptr)(void);
        //void_function_ptr enable_paging_ptr;
        //enable_paging_ptr = (void_function_ptr)enable_paging;
        //enable_paging();
    }
}

#endif //UROS_MEMORY_H
