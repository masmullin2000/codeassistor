#include <stdio.h>

int main(int argc, char *argv[])
{
  unsigned i = 0;
  
  if( argc < 2 ) return 0;
  
  int sz = atoi(argv[1]);
  int tmr = 0;
  
  if( argc == 3 ) tmr = atoi(argv[2]);
  
  for( i = 0; i < sz; i++ )
  {
      sleep(tmr);
      if( i % 81 != 80 || i == 0) printf("a",i);
      else printf("\n");
      
      fflush(stdout);
  }

  return 0;
}