//
// Protocol static definitions file
//
// Each defType is 1 byte in length,
// with a max of 255 values (excl. 0)
//
// Used in conversation with SQF
//

// Protocol Status Flags

#define SF_NONE			1	// In replacement for none of the below
#define SF_GOOD			2	// Return with data, not for requests
#define SF_ERROR		3	// Error contained in message
#define SF_CHUNK		4	// Part of or request for a chunk
#define SF_EOF			5	// End of chunked data output
#define SF_SYNC			6	// Request for synchronous code
#define SF_ASYNC		7	// Request for async code
#define SF_HANDLE		8	// Return for async calls

// Function Addresses

#define FA_NONE			1
#define FA_TEST			2
#define FA_VERSION		3
#define FA_LOG			4
