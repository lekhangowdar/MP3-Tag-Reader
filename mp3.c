/***************************************************
 *  FILE        : mp3.c
 *  PROJECT     : MP3 Tag Reader and Editor (ID3v2.3.0)
 *  DESCRIPTION : Provides functions to validate, view, and edit
 *                metadata of MP3 files supporting ID3v2.3.0 tags.
 *  AUTHOR      : Lekhan Gowda R
 ***************************************************/

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include "mp3.h"
 #include "types.h"
 
 // Displays help information
 void mp3_help(void)
 {
     printf("\n--------------------------HELP MENU--------------------------\n");
     printf("\n1. -v -> to view mp3 file contents\n");
     printf("2. -e -> to edit mp3 file contents\n");
     printf("\t2.1. -t -> to edit song title\n");
     printf("\t2.2. -a -> to edit artist name\n");
     printf("\t2.3. -A -> to edit album name\n");
     printf("\t2.4. -y -> to edit year\n");
     printf("\t2.5. -m -> to edit content\n");
     printf("\t2.6. -c -> to edit comment\n");
     printf("--------------------------------------------------------------\n");
 }
 
 // Displays usage error message and proper usage instructions
 void mp3_usage(void)
 {
     printf("\n--------------------------------------------------------------\n");
     printf("\nERROR : ./a.out: INVALID ARGUMENTS\n");
     printf("USAGE  :\n");
     printf("To view please pass like  : ./a.out -v mp3filename\n");
     printf("To edit please pass like  : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
     printf("To get help pass like     : ./a.out --help\n");
     printf("--------------------------------------------------------------\n");
 }
 
 // Validates MP3 file and opens it in read+ mode
 Status validate_and_open_file(char *file_name, MP3Info *mp3Info)
 {
     char *src_file_ext = strstr(file_name, ".mp3");
     int src_file_len = strlen(file_name);
 
     // Check if filename ends with ".mp3"
     if ((src_file_ext - file_name == src_file_len - 4) && src_file_ext)
         mp3Info->mp3_file_name = file_name;
     else
         return failure;
 
     // Open MP3 file in read+ mode
     mp3Info->fptr_mp3_file = fopen(mp3Info->mp3_file_name, "r+");
     if (mp3Info->fptr_mp3_file == NULL)
     {
         perror("fopen");
         fprintf(stderr, "ERROR: Unable to open file %s\n", mp3Info->mp3_file_name);
         return failure;
     }
 
     // Read and verify "ID3" tag
     char tag[3];
     fread(tag, 3, 1, mp3Info->fptr_mp3_file);
     if (strcmp(tag, "ID3") != 0)
     {
         printf("MP3 file is not ID3 tag\n");
         return failure;
     }
 
     // Check version - only accept v2.3.0
     char ver[2];
     fread(ver, 2, 1, mp3Info->fptr_mp3_file);
     if (ver[0] != 3 || ver[1] != 0)
     {
         printf("MP3 file is not ID3V2.3.0\n");
         return failure;
     }
 
     printf("\n--------------------------Validate Successful--------------------------\n");
     printf("MP3 file is ID3v2.3.0\n");
 
     // Open duplicate MP3 file for editing operations
     mp3Info->dup_file_name = "duplicate.mp3";
     mp3Info->fptr_dup_file = fopen(mp3Info->dup_file_name, "w+");
     if (mp3Info->fptr_dup_file == NULL)
     {
         perror("fopen");
         fprintf(stderr, "ERROR: Unable to open file %s\n", mp3Info->dup_file_name);
         return failure;
     }
 
     return success;
 }
 
 // Displays MP3 tag content to the user
 void mp3_view(MP3Info *mp3Info)
 {
     tag_read(mp3Info->fptr_mp3_file, mp3Info, "nt");
     printf("\n--------------------------SELECTED VIEW DETAILS--------------------------\n");
     printf("\t\tMP3 TAG READER AND EDITOR FOR ID3v2\n");
     printf("Title\t: %s\n", mp3Info->title);
     printf("Artist\t: %s\n", mp3Info->artist);
     printf("Album\t: %s\n", mp3Info->album);
     printf("Year\t: %s\n", mp3Info->year);
     printf("Music\t: %s\n", mp3Info->music);
     printf("Comment\t: %s\n", mp3Info->comment);
     printf("-------------------------------------------------------------------------\n");
 }
 
 // Reads tag frames and populates the structure fields
 void tag_read(FILE *fptr, MP3Info *mp3Info, char *op)
 {
     fseek(fptr, 10, SEEK_SET); // Skip header
 
     char tag[5], size_c[4], *name;
     unsigned int size;
 
     for (int i = 0; i < 11; i++)
     {
         fread(tag, 4, 1, fptr);      // Read frame ID
         tag[4] = '\0';
         if (strcmp(tag, op) == 0) return;
 
         fread(size_c, 4, 1, fptr);   // Read size of the frame
         size = (size_c[0] << 21) | (size_c[1] << 14) | (size_c[2] << 7) | size_c[3];
 
         fseek(fptr, 3, SEEK_CUR);    // Skip flags and encoding
         name = (char *)malloc(size - 1);
         fread(name, size - 1, 1, fptr); // Read text
         name[size - 1] = '\0';
 
         // Assign tags to respective structure fields
         if (strcmp(tag, "TIT2") == 0) mp3Info->title = name;
         else if (strcmp(tag, "TPE1") == 0) mp3Info->artist = name;
         else if (strcmp(tag, "TALB") == 0) mp3Info->album = name;
         else if (strcmp(tag, "TYER") == 0) mp3Info->year = name;
         else if (strcmp(tag, "TCON") == 0) mp3Info->music = name;
         else if (strcmp(tag, "COMM") == 0) mp3Info->comment = name;
     }
 }
 
 // Creates a duplicate of the original MP3 file
 Status create_dup_mp3(MP3Info *mp3Info)
 {
     fseek(mp3Info->fptr_mp3_file, 0, SEEK_SET);
     fseek(mp3Info->fptr_dup_file, 0, SEEK_SET);
     char ch;
     while (fread(&ch, 1, 1, mp3Info->fptr_mp3_file) > 0)
     {
         fwrite(&ch, 1, 1, mp3Info->fptr_dup_file);
     }
     return success;
 }
 
 // Edits specified MP3 tag field and writes updated content
 Status mp3_edit(char *tag, char *edit_info, MP3Info *mp3Info)
 {
     char op[5], new_size_s[4], option[8];
     int new_size;
 
     // Determine frame ID and label based on argument
     if (strcmp(tag, "-t") == 0) { strcpy(op, "TIT2"); strcpy(option, "TITLE"); }
     else if (strcmp(tag, "-a") == 0) { strcpy(op, "TPE1"); strcpy(option, "ARTIST"); }
     else if (strcmp(tag, "-A") == 0) { strcpy(op, "TALB"); strcpy(option, "ALBUM"); }
     else if (strcmp(tag, "-y") == 0) { strcpy(op, "TYER"); strcpy(option, "YEAR"); }
     else if (strcmp(tag, "-m") == 0) { strcpy(op, "TCON"); strcpy(option, "MUSIC"); }
     else if (strcmp(tag, "-c") == 0) { strcpy(op, "COMM"); strcpy(option, "COMMENT"); }
     else { mp3_usage(); return failure; }
 
     printf("\n----------------------CHANGE THE %s----------------------\n", option);
     printf("\n\t%s\t: %s\n", option, edit_info);
     printf("\n-----------------%s CHANGED SUCCESSFULLY-----------------\n\n", option);
 
     // Read tag from both files to locate correct frame
     tag_read(mp3Info->fptr_mp3_file, mp3Info, op);
     tag_read(mp3Info->fptr_dup_file, mp3Info, op);
 
     new_size = strlen(edit_info);
     int temp_size = new_size;
     int i = 0;
 
     // Encode size into 4-byte format for ID3v2
     for (i = 3; i >= 0; i--)
     {
         if (new_size > 254)
         {
             new_size = temp_size - 254;
             new_size_s[i] = 255;
         }
         else
         {
             new_size_s[i] = (i == 3) ? temp_size + 1 : temp_size;
             temp_size = 0;
         }
     }
 
     // Overwrite the frame in mp3 file
     fwrite(new_size_s, 4, 1, mp3Info->fptr_mp3_file);
     fseek(mp3Info->fptr_mp3_file, 3, SEEK_CUR);
     fwrite(edit_info, new_size, 1, mp3Info->fptr_mp3_file);
 
     // Adjust and skip in duplicate file
     fread(new_size_s, 4, 1, mp3Info->fptr_dup_file);
     new_size = (new_size_s[0] << 21) | (new_size_s[1] << 14) | (new_size_s[2] << 7) | new_size_s[3];
     fseek(mp3Info->fptr_dup_file, (2 + new_size), SEEK_CUR);
 
     // Copy rest of the data
     char ch;
     while (fread(&ch, 1, 1, mp3Info->fptr_dup_file) > 0)
     {
         fwrite(&ch, 1, 1, mp3Info->fptr_mp3_file);
     }
 
     return success;
 }
 