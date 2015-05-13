//uncomment this to make debug ON.
//#define DEBUG 1

#ifdef DEBUG
#define DBG(fmt, ...)	do { if (DEBUG) fprintf(stdout, "%3d:: IN %s\() :: "fmt" \n",__LINE__,__FUNCTION__, \
	                ##__VA_ARGS__); } while (0)
#else
#define DBG(fmt, ...) 
#endif

#define PR(fmt, ...)	do { fprintf(stdout, " %3d:: IN %s\() :: "fmt" \n",__LINE__,__FUNCTION__, \
        	        ##__VA_ARGS__); } while (0)

#define ERR(fmt, ...)	do { fprintf(stderr, "ERROR Line %3d , IN %s\() :: "fmt" \n",__LINE__,__FUNCTION__, \
                	##__VA_ARGS__); } while (0)

#define	FREE_NODE(_NODE) do{	\
							free(_NODE->name); \
							free(_NODE->logid);	\
							free(_NODE->password);	\
							free(_NODE);	\
							}while(0)
