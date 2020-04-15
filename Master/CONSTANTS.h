#ifndef CONSTANTS
#define CONSTANTS
#define STORE S
#define COMPARE C
typedef enum { ACK, OVERFLOW_NACK, ADDR_NACK, DATA_NACK, UNKNOWN_NACK } RESULTS; // Acknowledged, too much data, address not acknowledged, data not acknowledged, unknown error
typedef enum { STORE, COMPARE } MODES; // Store mode or compare mode
#endif