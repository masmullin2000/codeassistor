#include <stdio.h>

int main(void)
{
  unsigned i = 0,j;
  for( i = 0; i < 0x4FFFFF; i++ )
  {
      printf("the value is 0x%06x\n",i);
  }

  return 0;
}