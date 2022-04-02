
#include <stdio.h>

#define ARRAY_SIZE_1 4
#define ARRAY_SIZE_2 8

#if _PTYPE < '4'
extern long int ocfunc1(long int *parm1);

extern void ocproc1(char vb, short int vi16, 
		    long int *vi32, int vi,
		    float *vr32, double *vr64,
		    int vbo, 
		    int varr1[], int varr1_size, 
		    int varr2[ARRAY_SIZE_2],
		    char *b, short int *i16, 
		    long int *i32, int *i,
		    float *r32, double *r64, 
		    char *bo,
		    int arr1[], int arr1_size, 
		    int arr2[ARRAY_SIZE_2]);
#else
extern long int ocfunc1(long int parm1);

extern void ocproc1(char vb, short int vi16, 
		    long int vi32, int vi,
		    float vr32, double *vr64,
		    int vbo, 
		    int varr1[], int varr1_size, 
		    int varr2[ARRAY_SIZE_2],
		    char *b, short int *i16, 
		    long int *i32, int *i,
		    float *r32, double *r64,
		    char *bo, 
		    int arr1[], int arr1_size, 
		    int arr2[ARRAY_SIZE_2]);
#endif

#pragma IMS_nolink(ocfunc1)
#pragma IMS_nolink(ocproc1)


int main(void)
{
  int j;
  long int res, temp;
  char vb = 'X', b;
  short int vi16 = 32000, i16;
  long int vi32 = -64000L, i32;
  int vi = 42, i;
  float vr32 = 3.142F, r32;
  double vr64 = 9.81636373737373, r64;
  int vbo = 0;
  char bo;
  int varr1[ARRAY_SIZE_1] = { 56, 23, 671, -13 }, 
      arr1[ARRAY_SIZE_1];
  int varr2[ARRAY_SIZE_2] = { 23, 45, 56, 72, -12, 39, 287, 2}, 
      arr2[ARRAY_SIZE_2];

  temp = -654;

#if _PTYPE < '4'
  res = ocfunc1(&temp);
#else
  res = ocfunc1(temp);
#endif

  printf("res = %ld\n", res);

#if _PTYPE < '4'
  ocproc1(vb, vi16, &vi32, vi, &vr32, &vr64, 
	  vbo, varr1, ARRAY_SIZE_1, varr2,
	  &b, &i16, &i32, &i, &r32, &r64, 
	  &bo, arr1, ARRAY_SIZE_1, arr2);
#else
  ocproc1(vb, vi16, vi32, vi, vr32, &vr64, 
	  vbo, varr1, ARRAY_SIZE_1, varr2,
	  &b, &i16, &i32, &i, &r32, &r64, 
	  &bo, arr1, ARRAY_SIZE_1, arr2);
#endif  

  printf("vb = %c, b = %c\n", vb, b);
  printf("vi16 = %hd, i16 = %hd\n", vi16, i16);
  printf("vi32 = %ld, i32 = %ld\n", vi32, i32);
  printf("vi = %d, i = %d\n", vi, i);
  printf("vr32 = %g, r32 = %g\n", vr32, r32);
  printf("vr64 = %.15g, r64 = %.15g\n", vr64, r64);
  printf("vbo = %d, bo = %d\n", vbo, bo);
  for (j = 1; j < ARRAY_SIZE_1; j++)
    printf("varr1[%d] = %d, arr1[%d] = %d\n", j, varr1[j], j, arr1[j]);
  for (j = 1; j < ARRAY_SIZE_2; j++)
    printf("varr2[%d] = %d, arr2[%d] = %d\n", j, varr2[j], j, arr2[j]);
}
