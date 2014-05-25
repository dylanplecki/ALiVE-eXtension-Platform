//
// Protocol static definitions file
//
// Each defType is 1 byte in length,
// with a max of 255 values (excl. 0)
//
// Used in conversation with SQF
//

// Protocol Status Flags

#define SF_NONE		1	// Shouldn't be used (ever)
#define SF_GOOD		2
#define SF_ERROR	3	// Error contained in message
#define SF_CHUNK	4
#define SF_EOF		5	// End of chunked data
#define SF_HANDLE	6	// For async calls

// Function Addresses

#define FA_NONE		1
