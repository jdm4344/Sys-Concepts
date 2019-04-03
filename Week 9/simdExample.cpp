
#include <iostream>
#include <cstdint>
#include <emmintrin.h> // Define SSE2 intrinsic functions

using namespace std;

#define SIZE 1024
short int a[SIZE], b[SIZE], c[SIZE];
short int a2[SIZE], b2[SIZE], c2[SIZE];
float vec1[3] = { 1,2,3 };
float vec2[3] = { 1,2,3 };

// dot product function
// assumes both arrays are the same size
// so we don't check for that error
void dot_product(float v1[], float v2[], float &result, int size) {
	for (int i = 0; i < size; ++i) {
		result += v1[i] * v2[i];
	}

}

void automaticVectorizationExample() {
	// This will auto vectorize with most compilers
	for (int i = 0; i < SIZE; i++) {
		a[i] = b[i] + 2;
	}
}

// Loop with branch
void SelectAddMul(short int aa[], short int bb[], short int cc[]) {
	for (int i = 0; i < SIZE; i++) {
		aa[i] = (bb[i] > 0) ? (cc[i] + 2) : (bb[i] * cc[i]);
		//printf("%d\r\n", aa[i]);
	}

}

// Branch/loop function vectorized:
void SelectAddMul2(short int aa[], short int bb[], short int cc[]) {
	// Make a vector of (0,0,0,0,0,0,0,0)
	__m128i zero = _mm_set1_epi16(0);
	// Make a vector of (2,2,2,2,2,2,2,2)
	__m128i two = _mm_set1_epi16(2);
	// Roll out loop by eight to fit the eight-element vectors:
	for (int i = 0; i < SIZE; i += 8) {
		// Load eight consecutive elements from bb into vector b:
		__m128i b = _mm_loadu_si128((__m128i const*)(bb + i));
		// Load eight consecutive elements from cc into vector c:
		__m128i c = _mm_loadu_si128((__m128i const*)(cc + i));
		// Add 2 to each element in vector c
		__m128i c2 = _mm_add_epi16(c, two);
		// Multiply b and c
		__m128i bc = _mm_mullo_epi16 (b, c);
		// Compare each element in b to 0 and generate a bit-mask:
		__m128i mask = _mm_cmpgt_epi16(b, zero);
		// AND each element in vector c2 with the bit-mask:
		c2 = _mm_and_si128(c2, mask);
		// AND each element in vector bc with the inverted bit-mask:
		bc = _mm_andnot_si128(mask, bc);
		// OR the results of the two AND operations:
		__m128i a = _mm_or_si128(c2, bc);
		// Store the result vector in eight consecutive elements in aa:
		_mm_storeu_si128((__m128i *)(aa + i), a); 
	}
}

void fillArrays() {
	for (int j = 0; j<SIZE; j++) {
		a[j] = rand() % 100;
		a2[j] = a[j];
		b[j] = rand() % 100;
		b2[j] = b[j];
		c[j] = rand() % 100;
		c2[j] = c2[j];
	}
}

// get three numbers and average them
int main(int argc, char* argv[])
{
	fillArrays();
	automaticVectorizationExample();
	fillArrays();

	float result = 0;
	dot_product(vec1, vec2, result, 3);

	for (int i = 0; i < 6000; i++) {
		SelectAddMul(a, b, c);
		SelectAddMul2(a2, b2, c2);
	}
}


