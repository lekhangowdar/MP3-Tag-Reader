#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;  // Alias for unsigned int as uint

/* Status will be used in function return type to indicate success or failure */
typedef enum
{
    success,   // Operation was successful
    failure    // Operation failed
} Status;

/* Enum for operation types passed via command-line arguments */
typedef enum
{
    help,        // Help option (--help)
    edit,        // Edit option (-e)
    view,        // View option (-v)
    unsupported  // Unsupported or invalid operation
} OperationType;

#endif
