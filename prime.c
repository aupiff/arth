#include<stdio.h>
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

int main(int argc, char** argv) {
	long i = atol(argv[1]);
	printf("%li: %d\n", i, miller_suite(i));
	return 0;
}
