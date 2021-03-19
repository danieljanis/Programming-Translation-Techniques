/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: README.txt                                             /
/  Last Edit: 4/5/2020                                         */
/***************************************************************/

I am using option 3 (FSA table + driver)

The FSA table is declared in the file:
	--> table.cpp

The driver() function is contained in the file:
	--> driver.cpp

/**************************************************************/
/* MAKEFILE */

The makefile for this program (scanner) is titled makefile.
This makefile can be ran by typing the command:
        > "make"

If the program has been made already (.o files and scanner* exists),
the following command will clean those files:
        > "make clean"

/**************************************************************/
/* RUNNING THE PROGRAM */

To run the program, you must first use the "make" command above.
Once the makefile does its magic you can run this program with
any of the following methods (type whats in the double quotes):

 > "scanner [file]"
                                (where file is an optional argument, the file
                                 extension can be left off with this call)

 > "scanner"
                        (If file was not provided, the program scans
                         data from the keyboard as a device)

 > "scanner < [file]"
                                (redirects input from the file instead of the
                                 keyboard, the file extension is required for
                                 this call to run successfully)

Delete output files as necessary using the "rm [file]" command.

Comments:
> For "scanner < [file]", the standard input is being read in from
  the [file] that was passed to this program using bash (MUST have the extension to work)

> The prompt "Please enter alphanumerical strings..."
  can be ignored if the above invocation method was used

> If you try the invokation using "scanner < [file]" and file does not exist,
  if the file is saved without adding anything and pressing CTRL+D then you can
  ignore "SCANNER ERROR:
		  Found a character that wasn't in alphabet.
		  (error at line: 1, index: 0)"
