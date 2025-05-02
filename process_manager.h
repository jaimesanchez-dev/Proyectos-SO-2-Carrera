#ifndef HEADER_FILE
#define HEADER_FILE

struct parametros {
    int id;
    int belt_size;
    int num_products;
};

void *process_manager(void *arg);
#endif