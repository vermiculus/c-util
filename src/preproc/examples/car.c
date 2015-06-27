#include "car.h"

int main() {
  CarRef car = Car.alloc();
  Car.init(car);
  Car.drive(car, 20);
  printf("Done with the thing.\n");
}
