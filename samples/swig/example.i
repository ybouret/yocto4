/* example.i */
 %module example
 %{
 /* Put header files here or function declarations like below */
 extern double My_variable;
 extern int fact(int n);
 extern int my_mod(int x, int y);
 extern char *get_time();
 extern double my_sum( const double *arr, int n );
 extern void my_mat( double *m, int nr, int nc );

 %}
 
 extern double My_variable;
 extern int fact(int n);
 extern int my_mod(int x, int y);
 extern char *get_time();
 extern double my_sum( const double *arr, int n ); 
 extern void my_mat( double *m, int nr, int nc );
