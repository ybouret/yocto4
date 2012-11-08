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

double my_sum( double *arr, int n )
{
	double ans = 0;
	int i=0;
	for( ; i < n; ++i ) 
	{
		ans += arr[i];
		arr[i] = -i;
	}
	return ans;
} 


void my_mat( double *m, int nr, int nc )
{
	int i,j;
	for(i=0; i < nr; ++i )
	{
		double *row = m + (i*nc);
		for(j=0; j< nc; ++j)
		{
			row[j] = i;
		}
	}
}
