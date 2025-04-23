/***************************************************
 *  FILE        : main.c
 *  PROJECT     : MP3 Tag Reader and Editor (ID3v2.3.0)
 *  DESCRIPTION : Entry point to the MP3 Tag Reader project
 *  AUTHOR      : Lekhan Gowda R
 ***************************************************/
/*
Input : (CLA) /a.out -e beautiful.bmp secret.txt out.bmp

Output :(CLA) ./a.out -d out.bmp decoded.txt
*/

#include <stdio.h>
#include "types.h"
#include <string.h>
#include "mp3.h"

//pass the CLA in main function
int main(int argc, char *argv[])
{
    MP3Info mp3Info;
    //check if minimum 3 arguements is passed
    if (argc<2)
    {
        mp3_usage();
        return 0;
    }
    //call check operation type  
    int ret = check_operation_type(argv);
    if(ret == help)
    {
        mp3_help();
        return 0;
    }
    
    //check if -v is entered
    if(ret == view)
    {
        if (argc<3)  //check if minimum 3 arguements is passed
        {
            mp3_usage();
            return 0;
        }
        if(validate_and_open_file(argv[argc-1],&mp3Info) == failure)
        {
            printf("File is not in required mp3 format\n");
            return 0;
        }        
        mp3_view(&mp3Info); 
    }

    //check if -e is entered
    else if(ret == edit) 
    {
        if (argc<5)   //check if minimum 3 arguements is passed
        {
            mp3_usage();
            return 0;
        }
        if(validate_and_open_file(argv[argc-1],&mp3Info) == failure)
        {
            printf("File is not in required mp3 format\n");
            return 0;
        }
        if(create_dup_mp3(&mp3Info) == failure)
        {
            printf("Creating duplicate of mp3 file failed\n");
            return 0;
        }
        if(mp3_edit(argv[2],argv[3],&mp3Info) == failure)
        {
            printf("Editing %s tag in mp3 failed\n",argv[2]);
            return 0;
        }
    }
    else
        mp3_usage();

    return 0; 
}

//function defination to check operation type
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"--help") == 0)
    {
        return help;
    }
    else if(strcmp(argv[1],"-v") == 0)  //check if -d is entered
    {
        return view;
    }
    else if(strcmp(argv[1],"-e") == 0)   //check if -e is entered
    {
        return edit;
    }
    else
    {
        //mp3_usage();
        return unsupported;
    }
}