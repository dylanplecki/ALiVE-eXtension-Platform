
SQF Interface Protocol
======================

## Protocol Specification

	+-------------------------------------------+
	| Byte		| Description					|
	+-------------------------------------------+
	| 0			| Packet Flag					|
	| 1			| Function Address				|
	| 2 - Inf	| Packet Message				|
	+-------------------------------------------+

## Guarantees

- Returned message is valid SQF code in string form.
- Messages sent to the extension can be delimited, but only by the following UTF character codes:
	- File separator (UTF+001C &#28)
	- Group separator (UTF+001D &#29)
	- Record separator (UTF+001E &#30)
	- Unit separator (UTF+001F &#31)
  These delimiters are listed in decreasing hierarchicy, from most encompassing to least significant.
  The implementation of these separators are dependent on the called function, and are not universal.

## SQF Interaction

- After data is received and flags/addresses are processed, all data except the message is nulled
  with the space character (U+0020 &#20), and the entire packet is call compile'd for return data.
