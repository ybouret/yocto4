 /* File : example.c */
 
 #include <time.h>
 double My_variable = 3.0;
 
 int fact(int n) {
     if (n <= 1) return 1;
     else return n*fact(n-1);
 }
 
 int my_mod(int x, int y) {
     return (x%y);
 }
 	
 char *get_time()
 {
     time_t ltime;
     time(&ltime);
     return ctime(&ltime);
 }

double my_sum( const double *arr, int n )
{
	double ans = 0;
	int i=0;
	for( ; i < n; ++i ) ans += arr[i];	
	return ans;
} 

