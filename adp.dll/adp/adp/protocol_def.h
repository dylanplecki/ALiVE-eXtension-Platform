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
#define SF_GOOD			2	// Okay return with or without data
#define SF_ERROR		3	// Error contained in message
#define SF_CHUNK		4	// Signals that data is chunked
#define SF_SYNC			5	// Request for synchronous code
#define SF_ASYNC		6	// Request for async code
#define SF_HANDLE		7	// Return for async calls
