#ifndef QUEUE_H
#define QUEUE_H

struct element {
  int num_edition;
  int id_belt;
  int last;
};

int queue_init (int size);
int queue_destroy (void);
int queue_put (struct element* elem);
struct element * queue_get(void);
int queue_empty (void);
int queue_full(void);

#endif
