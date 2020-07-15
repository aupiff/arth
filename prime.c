#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

long mod_pow(long a, long e, long n) {
	long cur_a = a;
	long acc = 1;

	while (e != 0) {
		if ((e & 1) == 1) {
			acc = (acc * cur_a) % n;
		}
		e = e >> 1;
		cur_a = (cur_a * cur_a) % n;
	}

	return acc;
}

int miller(long n, long a) {
	if (n == 2) return 1;
	if (n < 2 || n % 2 == 0) return 0;

	int t = 1;
	long e = (n - 1)/2;

	while (t == 1) {
		long u = mod_pow(a, e, n);
		if (u == n-1) {
			t = 0;
		} else if (u == 1 && e%2 == 0) {
			e /= 2;
		} else if (u == 1) {
			t = 0;
		} else {
			t = 2;
		}
	}

	return t == 0;
}

int miller_suite(long n) {
	long arr[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
	long t = 0;
	for (int i=0; i<9; i++) {
		if (arr[i] < n && miller(n, arr[i]) == 0) return 0;
	}
	return 1;
}

long next(int* active, long* sqarr, long limit, int high) {
	if (high == 0) {
		return 0;
	}

	int mode = 0;
	for (int i=0; i<high+2; i++) {
		if (mode == 0 && active[i] == 0) {
			active[i] = 1;
			break;
		} else if (mode == 0 && active[i] == 1) {
			active[i] = 0;
			mode = 1;
		} else if (mode == 1 && active[i] == 0) {
			active[i] = 1;
			break;
		} else {
			active[i] = 0;
		}
	}

	if (active[high+1] == 1) return 0;

	// if over limit go to next highest
	int parity = 0;
	long val = 1;
	for (int i=0; i<high+1; i++) {
		if (active[i] == 1) {
			parity++;
			val *= sqarr[i];
		}
	}
	
	if (val > limit) {
		val = 0;
		if (active[high - 1] != 1) {
			// find highest 1
			int new_high;
			for (int i = high-2; i > -1; i++) {
				if (active[i] == 1) {
					new_high = i+1;
					break;
				}
			}
			parity = 2;
			val = sqarr[0] * sqarr[new_high];
		}
	}
	// if we flipped the high bit return zero parity
	return (parity % 2 == 0) ? val : -val;
}

int main(int argc, char** argv) {
	// limit = 2^50 = 1125899906842624
	long limit = atol(argv[1]);
	int sqarr_size = ((int) sqrt((double) limit)) + 2;
	long* sqarr = (long*)malloc(sqarr_size * sizeof(long));

	// 4 = 2^2, the first square prime
	sqarr[0] = 4;
	int count = 1;
	for (long i=3; i<sqarr_size; i=i+2) {
		if (miller_suite(i)==1) {
			sqarr[count] = i*i;
			count++;
		}
		if (i % 1000001 == 0) printf("%li ...\n", i);
	}
	printf("count of primes: %d\n", count);

	long square_count = 0;
	int* active = (int*)malloc(sqarr_size * sizeof(int));
	int parity = 1;
	long val;
	for (int i=0; i<count && limit > sqarr[i]; i++) {
		memset(active, 0, count);
		active[i] = 1;
		val = -sqarr[i];
		while (val != 0) {
			printf("%li\n", val);
			int factor = val < 0 ? -1 : 1;
			square_count += factor * (limit / (factor * val));
			val = next(active, sqarr, limit, i);
		}
		printf("%d ... %li\n", i, square_count);
	}
	long squarefree_count = limit + square_count;

	printf("count of squarefreee numbers: %li\n", squarefree_count);
	return 0;
}
