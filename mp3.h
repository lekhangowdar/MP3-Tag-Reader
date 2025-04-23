// mp3.h - Header file for MP3 tag editor project
// Contains declarations, struct definition, and function prototypes

#ifndef MP3_H
#define MP3_H

#include <stdio.h>            // Standard I/O library
#include "types.h"            // Contains user-defined types (Status, OperationType)

// Struct to hold MP3 file information and metadata fields
typedef struct _mp3Info
{
    char * mp3_file_name;     // Pointer to MP3 file name
    char * dup_file_name;     // Pointer to duplicate file name
    FILE * fptr_mp3_file;     // File pointer for original MP3 file
    FILE * fptr_dup_file;     // File pointer for duplicate MP3 file
    int skip_size;            // Skip size (unused or reserved)
    char * title;             // Title tag content
    char * artist;            // Artist tag content
    char * album;             // Album tag content
    char * year;              // Year tag content
    char * music;             // Genre/music tag content
    char * comment;           // Comment tag content
} MP3Info;

// Display help menu
void mp3_help(void);

// Display usage instructions
void mp3_usage(void);

// Create a duplicate MP3 file
Status create_dup_mp3(MP3Info *mp3Info);

// View metadata of MP3 file
void mp3_view(MP3Info *mp3Info); 

// Determine the type of operation requested
OperationType check_operation_type(char *argv[]);

// Validate and open original and duplicate files
Status validate_and_open_file( char * file_name, MP3Info *mp3Info);

// Edit specified metadata tag
Status mp3_edit(char * tag, char * edit_info,MP3Info *mp3Info);

// Read specific tag from MP3 file
void tag_read(FILE * fptr,MP3Info *mp3Info,char * op);

#endif
