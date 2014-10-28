
#define EQ(s1,s2) (strcmp(s1,s2)==0)
#define LT(s1,s2) (strcmp(s1,s2)>0)
#define GT(s1,s2) (strcmp(s1,s2)<0)

/*
 * heapsort, taken almost verbatim from Aho, Hopcroft, and Ullman,
 * "The Design and Analysis of Computer Algorithms",
 * Addison-Wesley, 1974
 */


heapify( a, i, j )
char *a[];
int i, j;
{
	int k, start, two_i;
	register char *tmp;
	
	start = i;
#define LEFTSON a[i+i+1]
#define RIGHTSON a[i+i+2]
#define FATHER a[i]

	do
	{
	  	two_i = i + i;

		if( two_i+2 < j && 
		   ( LT( LEFTSON, FATHER ) || LT( RIGHTSON, FATHER ) ) )
		{
			if( LT( LEFTSON , RIGHTSON ) )
				k = two_i + 1;
			else
				k = two_i + 2;


			/* switch */
			tmp = FATHER;
			FATHER = a[k];
			a[k] = tmp;

			i = k;
		}
		else
			i = i/2;

	} while( i > start );
}

buildheap( a , n)
char *a[];
int n;
{
	int i;

	for( i=n-1; i>=0; i = i - 1 )
		heapify( a, i , n );

	return;
}

heapsort( a, n )
char *a[];
int n;
{
	int i;
	register char *tmp;

	buildheap( a, n );

	for( i=n-1; i>=1; i = i-1 )
	{
		tmp = a[0];
		a[0] = a[i];
		a[i] = tmp;

		heapify( a, 0, i-1 );
	}
	return;
}

/* end AHU heapsort code */


/*
 * front end to the sorting routine.
 */

void
sort_list( list, num_els )
char *list[];
int num_els;
{
	heapsort( list, num_els);
}
