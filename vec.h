#ifndef VEC_H

#define DATATYPE int 
#define IDENT 1
#define OP *

typedef DATATYPE data_t;

typedef struct {
  long int len;
  data_t *data;
} vec_rec, *vec_ptr;

#endif
