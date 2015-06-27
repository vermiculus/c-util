#include <stdio.h>
#include <stdlib.h>
#ifndef _Car_h
#define _Car_h
//Class file for Car
//Produced by https://github.com/vermiculus/c-util
typedef struct _CarRef
{
  int wheels;
  const char *color;
  double speed;
} *CarRef;
struct _CarRef_meth
{
  CarRef (*alloc) ();
  void (*dealloc) ();
  void (*init) (CarRef self);
  void (*drive) (CarRef self, int miles);
  void (*internal_method) (CarRef self, int var);
};
CarRef _Car_h_alloc ();
void _Car_h_dealloc (CarRef self);
void _Car_h_init (CarRef self);
void _Car_h_drive (CarRef self, int miles);
void _Car_h_internal_method (CarRef self, int var);
struct _CarRef_meth Car =
  { _Car_h_alloc, _Car_h_dealloc, _Car_h_init, _Car_h_drive,
_Car_h_internal_method };
CarRef
_Car_h_alloc ()
{
  return (CarRef) calloc (sizeof (CarRef), 1);
}

void
_Car_h_dealloc (CarRef self)
{
  free (self);
}

void
_Car_h_init (CarRef self)
{
  printf ("How fast does this car go?\n");
  scanf ("%lf", &self->speed);
  Car.internal_method (self, 108);
}

void
_Car_h_drive (CarRef self, int miles)
{
  printf ("speed = %lf and miles = %d\n", self->speed, miles);
}

void
_Car_h_internal_method (CarRef self, int var)
{
  printf ("magic number is %d \n", var);
}
#endif //_Car_h
