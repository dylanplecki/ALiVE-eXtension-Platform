
SQF Interface Protocol
======================

## New Protocol Specification

Note that the symbol '§' denotes the status flag, while the symbol '▼' denotes the unit separator.

### SQF Calls

Session Call:	"§"	(status SF_NEW_SESSION || SF_DEL_SESSION)
	Return:		"§" (status SF_GOOD || SF_NONE || SF_ERROR)

Function Call:	"§library_name▼function_name▼argument▼list" (status SF_SYNC || SF_ASYNC)
	Return:		"§0x0000" || "§['sqf', 'return', 'value']" (status SF_HANDLE || SF_GOOD || SF_CHUNK || SF_ERROR)

Chunked Call:	"§0x0000" (status SF_CHUNK)
	Return:		"['sqf', 'return', 'value']" || ""

## Protocol Specification (DEPRECATED)

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
