
SQF Interface Protocol
======================

## Protocol Specification

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
