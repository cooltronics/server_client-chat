typedef enum retVal{
	unSuccessful = -1,
	Successful = 0
};

//self referential structre
//contains at least one member which points to object of same data
//type
//linked list can grow and shrink dynamically
#define EXIT 0
//uncomment this to make debug ON.
#define DEBUG 1

#ifdef DEBUG
#define DBG(fmt, ...) \
            do { if (DEBUG) fprintf(stdout, "%3d:: IN %s\(\) :: "fmt"\n",__LINE__,__FUNCTION__, \
                ##__VA_ARGS__); } while (0)
#else
#define DBG(fmt, ...) 
#endif

#define PR(fmt, ...) \
            do { fprintf(stdout, " %3d:: IN %s\(\) :: "fmt"\n",__LINE__,__FUNCTION__, \
                ##__VA_ARGS__); } while (0)

#define ERR(fmt, ...) \
            do { fprintf(stderr, "ERROR Line %3d , IN %s\(\) :: "fmt"\n",__LINE__,__FUNCTION__, \
                ##__VA_ARGS__); } while (0)
