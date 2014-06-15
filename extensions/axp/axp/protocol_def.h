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
#define SF_ASYNC		6	// Request for asynchronous code
#define SF_HANDLE		7	// Return for asynchronous calls
#define SF_NEW_SESSION	8	// Start a new native session
#define SF_DEL_SESSION	9	// Stop the current native session
#define SF_VERSION		10	// Get extension version information
#define SF_COPY			11	// Copy input data to output buffer
#define SF_AUTH			12	// Get extension authorization key
