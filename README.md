# MP3 Tag Reader

## Overview

The MP3 Tag Reader is a simple tool that allows users to read and display metadata from MP3 files, specifically the ID3 tags. ID3 tags are used to store metadata such as the song title, artist name, album name, year, and other relevant information embedded within an MP3 file.

This project parses the MP3 files and extracts the ID3 tag information to display in a human-readable format.

## Features

- **Read ID3 Tags**: Extracts and displays common MP3 metadata such as title, artist, album, year, genre, and track number.
- **Supports ID3v1 and ID3v2 Tags**: The program supports both ID3v1 and ID3v2 tag formats.
- **Simple CLI Interface**: Allows users to input an MP3 file and receive the tag information in the terminal.
- **Error Handling**: Displays error messages for unsupported or corrupt MP3 files.

## Project Structure

The project consists of the following key files:

- `mp3_reader.c`: Contains the logic for reading the MP3 file and extracting the ID3 tags.
- `mp3_tag.h`: Header file defining the structures and function declarations for handling MP3 metadata.
- `utils.c`: Utility functions to handle file reading, data parsing, and error handling.
- `Makefile`: Contains the build instructions for compiling the program.
  
## Data Structures

The program uses structures to store metadata from the MP3 file:

### `MP3Tag` Structure

```c
typedef struct {
    char title[30];    // Song title
    char artist[30];   // Artist name
    char album[30];    // Album name
    char year[4];      // Year of release
    char genre[20];    // Genre of the song
    int track;         // Track number
} MP3Tag;
