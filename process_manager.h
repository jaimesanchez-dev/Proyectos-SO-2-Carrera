#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

struct parametros {
    int id;
    int belt_size;
    int num_products;
};

void *process_manager(void *arg);
#endif