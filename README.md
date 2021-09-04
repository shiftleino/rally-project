# rallyProject

## Description

This project was made for the course "Basic Course for C Programming". 
It is a results system for a rally race that consists of several special stages.

## Commands
The program has the following functions:

### A lastname team : Add driver
Adds driver named ”lastname” to the database, who is representing ”team”. Initially, the total time will be 0 seconds. For example: A Kankkunen Renault

### U lastname hours minutes seconds : Update total time
Adds the given time to drivers total time. For example: U Kankkunen 0 52 16 will add 52 minutes and 16 seconds to the total time of driver Kankkunen.

### L: Print results
Prints the current results of the race each on a separate line. 

### W filename: Save results
Writes the current database to file with given filename.

### O filename: Load results
Loads the database from file, and replaces the existing results.

### Q : Exit program
Exits program and releases all allocated memory.

## Compiling
Use the following gcc command for compiling:
-c -std=c99 -g -Wall -Wextra -Wno-missing-field-initializers
