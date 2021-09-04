#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct driver {
	char *lastname;
	char *team;
	int total_time;
};

struct database {
	struct driver *racer; // DRIVER (dynamically allocated)
	struct database *next; // (pointer to the next driver in the database)
};

// FUNCTION FOR ADDING A RACER IN THE DATABASE
int addRacer(struct database *block, char *name, char *team) {
	// THEN WE CHECK IF THE NAME IS ALREADY IN THE DATABASE
	int already = 0;
	struct database *block2 = block;
	if (block2->next != NULL) {
		block2 = block2->next;
	} 
	while (block2->next != NULL) {
		if (strcmp(name, block2->racer->lastname) == 0) {
			already = 1;
		}
		block2 = block2->next;
	}
	if (block2->racer != NULL) {
		if (strcmp(name, block2->racer->lastname) == 0) {
			already = 1;
		}
	}
	// IF THE RACER IS ALREADY IN THE DATABASE WE WILL PRINT ERROR MESSAGE
	if (already == 1) {
		printf("Driver %s is already in the database.\n", name);
		free(name);
		free(team);
		return -2;
	// ELSE WE WILL ADD THE RACER TO THE DATABASE
	} else {
		// GET THE LAST NODE IN THE DATABASE
		while (block->next != NULL) {
			block = block->next;
		} 
		// FIRST WE ALLOCATE MEMORY FOR THE NEW DATABASE BLOCK FOR THE RACER
		struct database *new;
		new = malloc(sizeof(struct database));
		if (new == NULL) {
			return -1;
		}	
		// THEN WE CREATE THE DRIVER STRUCT FOR THE RACER AND ALLOCATE MEMORY FOR IT
		struct driver *racer; 
		racer = malloc(sizeof(struct driver));
		if (racer == NULL) {
			free(new);
			return -1;
		}
		// THEN WE ADD THE DATA TO THE DRIVER STRUCT
		racer->lastname = name;
		racer->team = team;
		racer->total_time = 0;
		// AND FINALLY ADD THE RACER TO THE DATABASE
		new->next = NULL;
		new->racer = racer;
		block->next = new;
		return 0;
	}
}

// FUNCTION FOR UPDATING THE TIME OF A DRIVER
int updateTime(struct database *block, char *name, int hours, int minutes, int seconds) {
	if (block->next != NULL) {
		block = block->next;
	}
	while (block->next != NULL) {
		if (strcmp(block->racer->lastname, name) == 0) {
			int time = seconds + minutes*60 + hours*60*60;
			block->racer->total_time += time;
			return 0;
		}
		block = block->next;
	}
	if (block->racer != NULL) {
		if (strcmp(block->racer->lastname, name) == 0) {
			int time = seconds + minutes*60 + hours*60*60;
			block->racer->total_time += time;
			return 0;
		}
	}
	printf("THE GIVEN DRIVER WAS NOT IN THE DATABASE\n");
	return -1;
}

struct driver** sortTable(struct database *block) {
	
	// FIRST BEFORE SORTING WE LOOK HOW MANY ELEMENTS
	int num = 0;	// FIRST NODE IS JUST THE STARTER-NODE

	struct database *sortblock = block;
	while (sortblock->next) {
		num++;
		sortblock = sortblock->next;
	}

	// FIRST WE SKIP THE STARTER-NODE
	if (block->next != NULL) {
		block = block->next;
	}

	struct driver **sorted_array = malloc(num*sizeof(struct driver));	// ARRAY FOR THE SORTED RACERS
	int num_array = 0;	// NUMBER OF ELEMENTS ALREADY IN THE ARRAY

	// THEN WE LOOP THROUGH ALL THE NODES IN THE DATABASE
	while (block->next != NULL) {

		if (num_array == 0) {	// IF WE HAVE NO ELEMENTS IN THE ARRAY
			sorted_array[0] = block->racer;	// WE ADD THE FIRST RACER IN THE DB AS FIRST
			num_array++;
		} else {
			int total_time = block->racer->total_time;
			int i = 0;
			while (i < num_array) {
				if (total_time < sorted_array[i]->total_time) {	// IF THE TIME IS SMALLER
					for (int j = num_array; j > i; j--) {	// WE SHIFT ALL THE ELEMENTS
						sorted_array[j] = sorted_array[j - 1];	// AFTER i FORWARD
					}
					break;	// THEN WE BREAK OUT THE LOOP TO ADD THE PLAYER IN PLACE i
				} else {
					i++;	// ELSE WE GO FORWARD IN THE ARRAY
				}
			}
			sorted_array[i] = block->racer; // ELSE WE SET THE VALUE AS LAST
			num_array++;
		}
		block = block->next;	// THEN WE PROCEED TO THE NEXT RACER
	}
	
	// NOW WE STILL HAVE THE LAST NODE (iff the database was NOT empty)
	if (block->racer != NULL) {
		if (num_array == 0) {	// IF WE HAVE NO ELEMENTS IN THE ARRAY
			sorted_array[0] = block->racer;	// WE ADD THE FIRST RACER IN THE DB AS FIRST
			num_array++;
		} else {
			int total_time = block->racer->total_time;
			int i = 0;
			while (i < num_array) {
				if (total_time < sorted_array[i]->total_time) {	// IF THE TIME IS SMALLER
					for (int j = num_array; j > i; j--) {	// WE SHIFT ALL THE ELEMENTS
						sorted_array[j] = sorted_array[j - 1];	// AFTER i FORWARD
					}
					sorted_array[i] = block->racer; // AND PLACE THE RACER TO THE ith PLACE
					num_array++;
					break;
				} else {
					i++;	// ELSE WE GO FORWARD IN THE ARRAY
				}
			}
			if (num_array != num) {
				sorted_array[i] = block->racer; // ELSE WE SET THE VALUE AS LAST
				num_array++;
			}
		}
	}

	return sorted_array;
}

// FUNCTION FOR CALCULATING NUMBER OF DRIVERS
int numberOfDrivers(struct database *block) {
	int num = 0;
	struct database *sortblock = block;
	while (sortblock->next) {
		num++;
		sortblock = sortblock->next;
	}
	return num;
}

// FUNCTION FOR PRINTING THE RESULTS
void printResults(struct database *block) {
	struct driver **sorted_array = sortTable(block);
	int num = numberOfDrivers(block);
	// THEN WE PRINT OUT THE RACERS FROM THE SORTED ARRAY
	for (int k = 0; k < num; k++) {
		struct driver *racer = sorted_array[k];
		int h = racer->total_time / (60*60);
		int m = (racer->total_time - h*60*60) / 60;
		int s = racer->total_time - 60*m - 60*60*h; 
		printf("%s %s %d %d %d\n", racer->lastname, racer->team, h, m , s);
	}
	free(sorted_array);
}

// FUNCTION FOR SAVING THE RESULTS TO A FILE
int saveResults(struct database *block, char *filename) {
	FILE *file = NULL;
	file = fopen(filename, "w");
	if (file != NULL) {	
		struct driver **sorted_array = sortTable(block);
		int num = numberOfDrivers(block);
		// WE WRITE EACH LINE TO THE FILE
		for (int k = 0; k < num; k++) {
			struct driver *racer = sorted_array[k];
			int h = racer->total_time / (60*60);
			int m = (racer->total_time - h*60*60) / 60;
			int s = racer->total_time - 60*m - 60*60*h; 
			fprintf(file, "%s %s %d %d %d\n", racer->lastname, racer->team, h, m , s);
		}
		fclose(file);
		free(sorted_array);
		return 0;
	} else {
		printf("FILE OPEN ERROR");
		return -1;
	}
}


// FUNCTION FOR EMPTYING A DATABASE
void emptyList(struct database *block) {
	struct database *next;
	if (block->next != NULL) {
		struct database *current = block->next;
		while (current->next != NULL) {
			next = current->next;
			free(current->racer->lastname);
			free(current->racer->team);
			free(current->racer);
			free(current);
			current = next;
		}
		free(current->racer->lastname);
		free(current->racer->team);
		free(current->racer);
		free(current);
	}
}


// FUNCTION FOR LOADING THE RESULTS FROM A FILE
int loadResults(struct database *block, char *filename) {
	FILE *file = NULL;
	file = fopen(filename, "r");
	if (file != NULL) {
		char *name = malloc(sizeof(char) * 101);
		if (name == NULL) {
			return -1;
		}
		char *team = malloc(sizeof(char) * 101);
		if (team == NULL) {
			free(name);
			return -1;
		}
		int h;
		int m;
		int s;

		emptyList(block);

		while (fscanf(file, "%s %s %d %d %d\n", name, team, &h, &m, &s) != EOF) {
			int size_team = strlen(team);
			int size_name = strlen(name);
			char *n = realloc(name, sizeof(char) * (size_name + 1));
			char *t = realloc(team, sizeof(char) * (size_team + 1));	
			if (n == NULL) {
				return -1;
			}
			if (t == NULL) {
				free(n);
				return -1;
			}
			// FIRST WE ALLOCATE MEMORY FOR THE NEW DATABASE BLOCK FOR THE RACER
			struct database *new;
			new = malloc(sizeof(struct database));
			if (new == NULL) {
				free(n);
				free(t);
				return -1;
			}
			// THEN WE CREATE THE DRIVER STRUCT FOR THE RACER AND ALLOCATE MEMORY FOR IT
			struct driver *racer; 
			racer = malloc(sizeof(struct driver));
			if (racer == NULL) {
				free(n);
				free(t);
				free(new);
				return -1;
			}
			// THEN WE ADD THE DATA TO THE DRIVER STRUCT
			racer->lastname = n;
			racer->team = t;
			racer->total_time = s + 60*m + 60*60*h;
			// AND FINALLY ADD THE RACER TO THE DATABASE
			new->next = NULL;
			new->racer = racer;
			block->next = new;
			block = block->next;

			name = malloc(sizeof(char) * 101);
			team = malloc(sizeof(char) * 101);
			if (name == NULL) {
				return -1;
			}
			if (team == NULL) {
				free(name);
				return -1;
			}
		}
		fclose(file);
		free(name);
		free(team);
		return 0;
	} else {
		return -1;
	}
}


int main(void) {
	char cmd[1000];
	struct database db = { NULL, NULL };
	while (1) {	
		fgets(cmd, 1000, stdin);
		if (strcmp(cmd, "quit\n") == 0) {
			emptyList(&db);
			return 0;
		}
		if (strcmp(cmd, "exit\n") == 0) {
			emptyList(&db);
			return 0;
		}


		// ADDING A PLAYER TO THE DATABASE
		if (cmd[0] == 'A') {
			char *name = malloc(sizeof(char) * 101);
			char *team = malloc(sizeof(char) * 101);
			if (name == NULL) {
				printf("ERROR ALLOCATING MEMORY");	// MEMORY ALLOCATION FAILED
			} else if (team == NULL) {
				free(team);
				printf("ERROR ALLOCATING MEMORY");	// MEMORY ALLOCATION FAILED
			} else {
				if (sscanf(cmd, "%*c %s %s\n", name, team) != 2) {
					printf("A should be followed by exactly 2 arguments.\n");
					free(name);
					free(team);
				} else {
					// ALLOCATE MEMORY
					int size_team = strlen(team);
					int size_name = strlen(name);
					char *n = realloc(name, sizeof(char) * (size_name + 1));
					char *t = realloc(team, sizeof(char) * (size_team + 1));	
					if (n == NULL) {
						printf("ERROR ALLOCATING MEMORY");	// MEMORY ALLOCATION FAILED
					} else if (t == NULL) {
						free(n);
						printf("ERROR ALLOCATING MEMORY");	// MEMORY ALLOCATION FAILED
					} else {
						// THEN WE ADD THE RACER	
						int r = addRacer(&db, n, t);
						if (r == 0) {
							printf("SUCCESS\n");
						} else if (r == -2) {
							continue;
						} else {
							printf("ERROR ADDING A PLAYER\n");
						}
					}
				}
			}


		// UPDATING THE TOTAL TIME		
		} else if (cmd[0] == 'U') {
			int hour;
			int min;
			int sec;
			char *name = malloc(sizeof(char) * 101);
			// FIRST WE SCAN THE VARIABLES FROM THE INPUT
			if (sscanf(cmd, "%*c %s %d %d %d\n", name, &hour, &min, &sec) != 4) {
				printf("U should be followed by exactly 4 arguments.\n");
				free(name);
			} else {
				if (sec < 0 || sec > 59) {
					printf("Second cannot be negative or greater than 59.\n");
					free(name);
				} else if (min < 0 || min > 59) {
					printf("Minute cannot be negative or greater than 59.\n");
					free(name);
				} else if (hour < 0) {
					printf("Hour cannot be negative.\n");
					free(name);
				} else {
					int size_name = strlen(name);
					char *n = realloc(name, sizeof(char) * (size_name + 1));
					int r = updateTime(&db, n, hour, min, sec);
					if (r == 0) {
						printf("SUCCESS\n");
						free(n);
					} else {
						printf("ERROR UPDATING THE TIME\n");
						free(n);
					}
				}
			}


		// PRINTING THE RESULTS
		} else if (cmd[0] == 'L') {
			printResults(&db);
			printf("SUCCESS\n");


		// SAVING THE RESULTS TO A FILE
		} else if (cmd[0] == 'W') {
			char *filename = malloc(sizeof(char) * 101);
			if (sscanf(cmd, "%*c %s", filename) != 1) {
				printf("Command W needs one argument");
			} else {
				int size_filename = strlen(filename);
				char *n = realloc(filename, sizeof(char) * (size_filename + 1));
				int r = saveResults(&db, n);
				if (r == 0) {
					printf("SUCCESS\n");
					free(n);
				} else {
					printf("SOMETHING WENT WRONG WHEN SAVING THE RESULTS\n");
					free(n);
				}
			}
		

		// LOADING THE RESULTS FROM A FILE
		} else if (cmd[0] == 'O') {
			char *filename = malloc(sizeof(char) * 101);
			if (sscanf(cmd, "%*c %s", filename) != 1) {
				printf("Cannot open file %s for reading.\n", filename);
				free(filename);
			} else {
				// ELSE WE WILL LOAD THE DATA FROM THE FILE TO THE DATABASE
				int size_filename = strlen(filename);
				char *n = realloc(filename, sizeof(char) * (size_filename + 1));
				if (n == NULL) {
					printf("ERROR ALLOCATING MEMORY");
				} else {
					int r = loadResults(&db, n);
					if (r == 0) {
						printf("SUCCESS\n");
						free(n);
					} else {
						printf("Cannot open file %s for reading.\n", filename);
						free(n);
					}
				}
			}


		// QUITTING FROM THE APPLICATION
		} else if (cmd[0] == 'Q') {
			emptyList(&db);
			printf("SUCCESS");
			return 0;


		// ELSE THE COMMAND IS INVALID
		} else {
			printf("Invalid command %c\n", cmd[0]);
		}
	}
}
