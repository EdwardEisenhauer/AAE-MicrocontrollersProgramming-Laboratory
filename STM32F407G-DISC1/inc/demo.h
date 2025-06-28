#ifndef DEMO_H_
#define DEMO_H_

#include "main.h"

typedef enum {
  MCO_NO_DIV = 0b000,
  MCO_DIV_2 = 0b100,
  MCO_DIV_3 = 0b101,
  MCO_DIV_4 = 0b110,
  MCO_DIV_5 = 0b111
} MCO_Division;

#define HORIZONTAL_TRESHOLD   1500
#define SCALER                 168

void demo (void);

#endif /* DEMO_H_ */
