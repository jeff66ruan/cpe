#include "vec.h"
#include "rdtsc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define CASE_NO_MIN CASE_NO_ONE
#define CASE_NO_ONE   1
#define CASE_NO_TWO   2
#define CASE_NO_THREE 3
#define CASE_NO_FOUR  4
#define CASE_NO_FIVE  5
#define CASE_NO_MAX CASE_NO_FIVE

uint64_t time1, time2;
uint32_t t1_lo, t1_hi, t2_lo, t2_hi;


//create vector of specified length
vec_ptr new_vec(long int len)
{
  // allocate header structure
  vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));

  if (!result) return NULL;
  result->len = len;

  // allocate array
  if (len > 0){
    data_t *data = (data_t *)calloc(len, sizeof(data_t));
    if (!data){
      free((void *)result);
      return NULL;
    }
    result->data = data;
  }else
    result->data = NULL;
  return result;
}

//retrieve vector element and store at dest
//return 0 (out of bound) or 1 (successful)
int get_vec_element(vec_ptr v, long int index, data_t *dest)
{
  if (index < 0 || index >= v->len)
    return 0;
  *dest = v->data[index];
  return 1;
}

//return length of vector
long int vec_length(vec_ptr v)
{
  return v->len;
}

data_t *get_vec_start(vec_ptr v)
{
  return v->data;
}

void combine1(vec_ptr v, data_t *dest)
{
  long int i;
  *dest = IDENT;

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  for (i=0; i < vec_length(v); i++){
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
  STOP_COUNTER(t2_lo, t2_hi);

  time1 = MAKE_TIME_FROM_COUNTERS(t1_lo, t1_hi);
  time2 = MAKE_TIME_FROM_COUNTERS(t2_lo, t2_hi);
  
}

void combine2(vec_ptr v, data_t *dest)
{
  long int i;
  long int length = vec_length(v);

  *dest = IDENT;
  
  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  for (i=0; i < length; i++){
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
  STOP_COUNTER(t2_lo, t2_hi);
  time1 = MAKE_TIME_FROM_COUNTERS(t1_lo, t1_hi);
  time2 = MAKE_TIME_FROM_COUNTERS(t2_lo, t2_hi);
}

void combine3(vec_ptr v, data_t *dest)
{
  long int i;
  long int length = vec_length(v);
  data_t *data = get_vec_start(v);

  *dest = IDENT;

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  for (i = 0; i < length; i++) {
    *dest = *dest OP data[i];
  }
  STOP_COUNTER(t2_lo, t2_hi);
  time1 = MAKE_TIME_FROM_COUNTERS(t1_lo, t1_hi);
  time2 = MAKE_TIME_FROM_COUNTERS(t2_lo, t2_hi);
}

void combine4(vec_ptr v, data_t *dest)
{
  long int i;
  long int length = vec_length(v);
  data_t *data = get_vec_start(v);
  data_t acc = IDENT;

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  for (i = 0; i < length; i++) {
    acc = acc OP data[i];
  }
  *dest = acc;

  STOP_COUNTER(t2_lo, t2_hi);
  time1 = MAKE_TIME_FROM_COUNTERS(t1_lo, t1_hi);
  time2 = MAKE_TIME_FROM_COUNTERS(t2_lo, t2_hi);
}

/* Unroll loop by 2 */
void combine5(vec_ptr v, data_t *dest)
{
  long int i;
  long int length = vec_length(v);
  long int limit = length-1;
  data_t *data = get_vec_start(v);
  data_t acc = IDENT;

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);
  STOP_COUNTER(t2_lo, t2_hi);

  START_COUNTER(t1_lo, t1_hi);

  /* Combine 2 elements at a time */
  for (i = 0; i < limit; i+=2) {
    acc = (acc OP data[i]) OP data[i+1];
  }

  /* Finish any remaining elements */
  for (; i < length; i++) {
    acc = acc OP data[i];
  }
  *dest = acc;

  STOP_COUNTER(t2_lo, t2_hi);
  time1 = MAKE_TIME_FROM_COUNTERS(t1_lo, t1_hi);
  time2 = MAKE_TIME_FROM_COUNTERS(t2_lo, t2_hi);
}


static inline void print_header(char *routine_name, long int len)
{
  printf("Cycles used by the measured routine %s with %ld items:\n",
	 routine_name, len); 
}

static inline void print_cycles()
{
  printf("  %lu\n",
	  (time2 - time1)); 
}

static void usage(char *name)
{
  printf("%s <loop> <round> <case>\n"
         "   <loop>  : The number of iterations in the combine routine.\n"
         "   <round> : The round of the combine routine is evaluated.\n"
         "   <case>  : select which combine routine to be evaluated.\n"
         "             1 - comibne1\n"
         "             2 - comibne2\n"
         "             3 - comibne3\n"
         "             4 - comibne4\n"
         "             5 - comibne5\n"
         "             1 - comibne1\n"
         "             ?? - lower1\n"
         "             ?? - lower2\n"
         , name);
}

static inline void evaluate(long int len, long int round, long int case_no)
{
  int i;
  data_t result = 0;

  vec_ptr vecp = new_vec(len);
  if (!vecp) {
    printf("new vec fails\n");
    exit(EXIT_FAILURE);
  }

  switch (case_no) {
  case CASE_NO_ONE:
    print_header("comibne1", len);
    for (i = 0; i < round; i++) {
      combine1(vecp, &result);
      print_cycles();
    }
    break;
  case CASE_NO_TWO:
    print_header("combine2", len);
    for (i = 0; i < round; i++) {
      combine2(vecp, &result);
      print_cycles();
    }
    break;
  case CASE_NO_THREE:
    print_header("combine3", len);
    for (i = 0; i < round; i++) {
      combine3(vecp, &result);
      print_cycles();
    }
    break;
  case CASE_NO_FOUR:
    print_header("combine4", len);
    for (i = 0; i < round; i++) {
      combine4(vecp, &result);
      print_cycles();
    }
    break;
  case CASE_NO_FIVE:
    print_header("combine5", len);
    for (i = 0; i < round; i++) {
      combine5(vecp, &result);
      print_cycles();
    }
    break;
  default:
    printf("wrong case number\n");
  }
}

int main(int argc, char **argv)
{
  long int i = 0;
  if (argc != 4) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  long int len = atol(argv[1]);
  long int round = atol(argv[2]);
  long int case_no = atol(argv[3]);

  if ((case_no < CASE_NO_MIN) || (case_no > CASE_NO_MAX)) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  evaluate(len, round, case_no);
}
