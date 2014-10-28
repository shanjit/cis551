main()
{
	char c;

	while( read( 7, &c, 1 ) > 0 )
		write( 9, &c, 1 );

	exit( 0 );
}
