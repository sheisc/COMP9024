
void swap(char **p, char **q) {
   char *t;

   t = *p;
   *p = *q;
   *q = t;
}

int main(void) {
   char aa = '1', bb = '2';

   char *pa = &aa;
   char *pb = &bb;

   swap(&pa, &pb);

   return 0;
}



