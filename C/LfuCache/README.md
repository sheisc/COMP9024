# LfuCache:  Least Frequently Used Cache

Doubly linked list + Hashtable

### How to use

```sh
LfuCache$ make CFLAGS="-fsanitize=address -O3 -Wall"

make main
make[1]: Entering directory '/home/iron/Downloads/LfuCache'
gcc -fsanitize=address -O3 -Wall -c src/main.c -o build/main.o
gcc -fsanitize=address -O3 -Wall -o main ./build/LfuCache.o ./build/main.o
make[1]: Leaving directory '/home/iron/Downloads/LfuCache'

```

### Output

```
--------  PrintLfuCache(), put(1, 10) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 1, value = 10



--------  PrintLfuCache(), put(2, 20) -----------

	 ======  freq = 1, n = 2 ====== 

		 key = 2, value = 20
		 key = 1, value = 10



--------  PrintLfuCache(), put(3, 30) -----------

	 ======  freq = 1, n = 3 ====== 

		 key = 3, value = 30
		 key = 2, value = 20
		 key = 1, value = 10



--------  PrintLfuCache(), put(4, 40) -----------

	 ======  freq = 1, n = 4 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20
		 key = 1, value = 10



--------  PrintLfuCache(), get(1) -----------

	 ======  freq = 1, n = 4 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20
		 key = 1, value = 10
<1, 10>



--------  PrintLfuCache(), get(2) -----------

	 ======  freq = 1, n = 3 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20
	 ======  freq = 2, n = 1 ====== 

		 key = 1, value = 10
<2, 20>



--------  PrintLfuCache(), get(3) -----------

	 ======  freq = 1, n = 2 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
	 ======  freq = 2, n = 2 ====== 

		 key = 2, value = 20
		 key = 1, value = 10
<3, 30>



--------  PrintLfuCache(), get(4) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 4, value = 40
	 ======  freq = 2, n = 3 ====== 

		 key = 3, value = 30
		 key = 2, value = 20
		 key = 1, value = 10
<4, 40>



--------  PrintLfuCache(), put(5, 50) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 5, value = 50
	 ======  freq = 2, n = 3 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20



--------  PrintLfuCache(), put(6, 60) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 6, value = 60
	 ======  freq = 2, n = 3 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20



--------  PrintLfuCache(), put(7, 70) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 7, value = 70
	 ======  freq = 2, n = 3 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20



--------  PrintLfuCache(), put(8, 80) -----------

	 ======  freq = 1, n = 1 ====== 

		 key = 8, value = 80
	 ======  freq = 2, n = 3 ====== 

		 key = 4, value = 40
		 key = 3, value = 30
		 key = 2, value = 20

```

