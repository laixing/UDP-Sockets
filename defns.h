struct request
{
	char m[ 24 ]; /* Name of machine on which client is running */
	int c; /* Client number */
	int r; /* Request number of client */
	int i; /* Incarnation number of clienta€?s machine */
	char operation[ 80 ]; /* File operation (actual request) client sends to server */
};