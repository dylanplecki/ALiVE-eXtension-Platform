
#define TO_STRING(s) #s
#define TO_STRING2(s) TO_STRING(s)

/* Version and File Information */

#define INF_PRODUCT_NAME "ALiVE eXtension Platform"
#define INF_COMPANY_NAME "ALiVE Development Team"
#define INF_LEGAL_COPYRIGHT "Copyright (C) 2014"
#define INF_FILE_DESCRIPTION "ALiVE Native Extension Platform (Arma 3)"

#define INF_INTERNAL_NAME "axp.dll"
#define INF_ORIGINAL_FILE_NAME "axp.dll"

#define INF_VERSION_MAJOR 1
#define INF_VERSION_MINOR 1
#define INF_VERSION_RTYPE 1
#define INF_VERSION_BUILD 7

#define INF_FILE_VERSION_ARR INF_VERSION_MAJOR,INF_VERSION_MINOR,INF_VERSION_RTYPE,INF_VERSION_BUILD
#define INF_FILE_VERSION_STR TO_STRING2(INF_VERSION_MAJOR) "." TO_STRING2(INF_VERSION_MINOR) "." TO_STRING2(INF_VERSION_RTYPE) "." TO_STRING2(INF_VERSION_BUILD)

#define INF_PRODUCT_VERSION_ARR INF_FILE_VERSION_ARR
#define INF_PRODUCT_VERSION_STR INF_FILE_VERSION_STR
