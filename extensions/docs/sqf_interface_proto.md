
SQF Interface Protocol
======================

## New Protocol Specification III

Note that the symbol '§' denotes the status flag, '▼' denotes the unit separator, and '•' denotes the SQF data type flag.
The two flags can hold 255 descrete values each, excluding the null character (0x00).

### All Interface Communications

SQF -> C++ : "§•data"
C++ -> SQF : "§data"

Notes:
 - The chunked/data return interface specification doesn't require a return status flag (C++ -> SQF) for performance reasons.
 - Return data (C++ -> SQF) must *always* be in an SQF compilable format.
 - Entry data arrays (SQF -> C++) are handled in the following format: "••data_1▼•data_2▼•data_3¶", where the '¶' character
   is the array boundary indicator (0x1E | 30), and where the first SQF data type flag identifies the array.

-------------------------------------------------

## Protocol Specification II (DEPRECATED)

Note that the symbol '§' denotes the status flag, while the symbol '▼' denotes the unit separator.

### SQF Calls

Session Call:	"§"	(status SF_NEW_SESSION || SF_DEL_SESSION)
	Return:		"§" (status SF_GOOD || SF_NONE || SF_ERROR)

Function Call:	"§library_name▼function_name▼argument▼list" (status SF_SYNC || SF_ASYNC)
	Return:		"§0x0000" || "§['sqf', 'return', 'value']" (status SF_HANDLE || SF_GOOD || SF_CHUNK || SF_ERROR)

Chunked Call:	"§0x0000" (status SF_CHUNK)
	Return:		"'sqf', 'return', 'value'" || ""

Note that a chunked call will not return brackets, while a normal function return call will.

-------------------------------------------------

## Protocol Specification I (DEPRECATED)

	+-------------------------------------------+
	| Byte		| Description					|
	+-------------------------------------------+
	| 0			| Packet Status Flag			|
	| 1			| Function Address				|
	| 2 - Inf	| Packet Message				|
	+-------------------------------------------+

## Guarantees

- Returned message is valid SQF code in string form.
- Messages sent to the extension can be delimited, but only by the following UTF character code(s):
	1. Unit separator (UTF+001F &#31)

## SQF Interaction

- After data is received and flags/addresses are processed, all data except the message is nulled
  with the space character (U+0020 &#20), and the entire packet is call compile'd for return data.
