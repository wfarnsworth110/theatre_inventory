// Programming Assignment 7
// Walker Farnsworth
// wa934080

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXLEN 19
#define TABLESIZE 300007
#define COMMAND 300000

// Structs
typedef struct item{
	
	char name[MAXLEN + 1];
	int quantity;
	int saleprice;
	
}item;

typedef struct node{
	
	item* iPtr;
	struct node * next;
	
}node;

typedef struct hashtable{
	
	node** lists;
	int size;
	
}hashtable;

// Function prototypes
node* create_node(char* word, int quantity, int totalprice);
void free_node(node* target);
void free_list(node* head);
void buy(hashtable* table, char* word, int quantity, int totalprice);
void sell(hashtable* table, char* word, int quantity);
void change_price(hashtable* table, char* word, int new_price);
int hashfunc(char* word, int size);
void chomp(char* word);
void clearBuffer();

// Global variables
int theatercash = 100000;
int runtime = 0;

int main()
{
	// Hash table
	hashtable* table = malloc(sizeof(hashtable));
	
	if(table == NULL) //failsafe
	{
		printf("hashtable malloc error in main...program exiting\n");
		exit(1);
	}
	
	// Hash table components
	table->lists = calloc(TABLESIZE, sizeof(node*));
	table->size = TABLESIZE;
	
	if(table->lists == NULL) //failsafe
	{
		printf("hashtable lists malloc error in main...program exiting\n");
		exit(1);
	}
	
	// n number of commands
	int n;
	int nFlag = 1;
	
	// Input n
	do{
		scanf("%d", &n);
		clearBuffer();
		
		if(n < 0 || n > COMMAND)
			printf("Invalid input.\n");
		else
			nFlag = 0;
	}while(nFlag);
	
	// Execute n commands
	for(int i = 0; i < n; ++i)
	{
		int flag = 1;
		do{
			// Hash table key
			int key;
			
			// Variables for collecting input
			char select[47] = {};
			char command[13] = {};
			char item_name[MAXLEN + 1] = {};
			char input2[7] = {};
			char input3[7] = {};
			int quantity = 0;
			int totalprice = 0;
			int new_price = 0;
			
			// Helper variables
			int selSize = 0;
			int spaceCheck = 0;
			int checkPoint = 0;
			
			fgets(select, 34, stdin);
			chomp(select);
			selSize = strlen(select);
			
			// Read command
			for(int x = 0; x < selSize; ++x)
			{
				if(select[x] == ' ')
				{
					++spaceCheck;
					strncpy(command, select, x);
					break;
				}
			}
			
			// Fail safe
			if(spaceCheck == 0)
			{
				printf("Invalid input.\n");
				continue;
			}
			
			// Process command
			if(strcmp(command, "buy") == 0)
			{
				// Update helper variables
				spaceCheck = 0;
				checkPoint = strlen(command) + 1;
				
				// Get item_name name
				for(int x = checkPoint; x < selSize; ++x)
				{
					if(select[x] == ' ')
					{
						++spaceCheck;
						strncpy(item_name, &select[checkPoint], x - checkPoint);
						break;
					}
				}
				
				// Fail safe
				if(spaceCheck == 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Update
				spaceCheck = 0;
				checkPoint = strlen(command) + strlen(item_name) + 2;
				
				// Collect quantity
				for(int x = checkPoint; x < selSize; ++x)
				{
					if(select[x] == ' ')
					{
						++spaceCheck;
						strncpy(input2, &select[checkPoint], x - checkPoint);
						quantity = atoi(input2);
						break;
					}
				}
				
				// Multipurpose fail safe
				if(spaceCheck == 0 || quantity < 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Collect totalprice
				checkPoint = strlen(command) + strlen(item_name) + strlen(input2) + 3;
				strcpy(input3, &select[checkPoint]);
				totalprice = atoi(input3);
				
				// Last fail safe
				if(totalprice < 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Execute buy command
				buy(table, item_name, quantity, totalprice);
				
				flag = 0;
			}
			else if(strcmp(command, "sell") == 0)
			{
				// Update helper variables
				spaceCheck = 0;
				checkPoint = strlen(command) + 1;
				
				// Get item_name name
				for(int x = checkPoint; x < selSize; ++x)
				{
					if(select[x] == ' ')
					{
						++spaceCheck;
						strncpy(item_name, &select[checkPoint], x - checkPoint);
						break;
					}
				}
				
				// Fail safe
				if(spaceCheck == 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Collect quantity
				checkPoint = strlen(command) + strlen(item_name) + 2;
				strcpy(input2, &select[checkPoint]);
				quantity = atoi(input2);
				
				// Last fail safe
				if(quantity < 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Execute sell command
				sell(table, item_name, quantity);
				
				flag = 0;
			}
			else if(strcmp(command, "change_price") == 0)
			{
				// Update helper variables
				spaceCheck = 0;
				checkPoint = strlen(command) + 1;
				
				// Get item_name name
				for(int x = checkPoint; x < selSize; ++x)
				{
					if(select[x] == ' ')
					{
						++spaceCheck;
						strncpy(item_name, &select[checkPoint], x - checkPoint);
						break;
					}
				}
				
				// Fail safe
				if(spaceCheck == 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Collect new_price
				checkPoint = strlen(command) + strlen(item_name) + 2;
				strcpy(input2, &select[checkPoint]);
				new_price = atoi(input2);
				
				// Last fail safe
				if(new_price < 0)
				{
					printf("Invalid input.\n");
					continue;
				}
				
				// Execute change_price command
				change_price(table, item_name, new_price);
				
				flag = 0;
			}
			else
				printf("Invalid input.\n");
			
		}while(flag);
	}
	
	// End output
	printf("%d\n%d\n", theatercash, runtime);
	
	// Free memory
	for(int i = 0; i < TABLESIZE; ++i)
		free_list(table->lists[i]);
	free(table->lists);
	free(table);
	table = NULL;
	
	return 0;
}

node* create_node(char* word, int quantity, int totalprice)
{
	// Create node pointer
	node* temp = malloc(sizeof(node));
	
	if(temp == NULL) //failsafe
	{
		printf("node malloc error in create_node...program exiting\n");
		exit(1);
	}
	
	// Create item pointer
	temp->iPtr = malloc(sizeof(item));
	temp->next = NULL;
	
	if(temp->iPtr == NULL) //failsafe
	{
		printf("item malloc error in create_node...program exiting\n");
		exit(1);
	}
	
	// Initial price of item (theoretically useless, but whatever)
	int initial_price = totalprice / quantity;
	if(initial_price == 0)
		++initial_price;
	
	// Populate item struct
	strcpy(temp->iPtr->name, word);
	temp->iPtr->quantity = quantity;
	temp->iPtr->saleprice = initial_price;
	
	// Update global variables and return node
	theatercash -= totalprice;
	++runtime;
	return temp;
}

void free_node(node* target)
{
	free(target->iPtr);
	free(target);
}

void free_list(node* head)
{
	if(head == NULL)
		return;
	
	free_list(head->next);
	free_node(head);
}

void buy(hashtable* table, char* word, int quantity, int totalprice)
{
	// Generate and use hash key
	int key = hashfunc(word, table->size);
	node* target = table->lists[key];
	
	// Traversal variables
	int isFound = 0;
	int checkFlag = 1;
	
	if(target == NULL)
	{
		target = create_node(word, quantity, totalprice);
		table->lists[key] = target;
	}
	else
	{
		// We need to check for collision
		do{
			// Each check is O(1) complexity
			++runtime;
			
			if(strcmp(target->iPtr->name, word) == 0)
			{
				// Target acquired
				isFound = 1;
				checkFlag = 0;
			}
			else
			{
				// Next node if applicable
				if(target->next != NULL)
					target = target->next;
				else
					checkFlag = 0;
			}
		}while(checkFlag);
		
		// Does the node exist?
		if(isFound == 1)
		{
			// Add quantity and update theatercash
			target->iPtr->quantity += quantity;
			theatercash -= totalprice;
		}
		else
		{
			// Must add node
			node* new = create_node(word, quantity, totalprice);
			new->next = table->lists[key];
			table->lists[key] = new;
		}
	}
	
	// Output
	printf("%s %d %d\n", target->iPtr->name, target->iPtr->quantity, theatercash);
}

void sell(hashtable* table, char* word, int quantity)
{
	// Generate and use hash key
	int key = hashfunc(word, table->size);
	node* target = table->lists[key];
	
	// Traversal variables
	int isFound = 0;
	int checkFlag = 1;
	
	// Find target node
	do{
		// Each check is O(1) complexity
		++runtime;
		
		if(strcmp(target->iPtr->name, word) == 0)
		{
			// Target acquired
			isFound = 1;
			checkFlag = 0;
		}
		else
		{
			// Next node if applicable
			if(target->next != NULL)
				target = target->next;
			else
				checkFlag = 0;
		}
	}while(checkFlag);
	
	if(isFound == 1)
	{
		// We can only sell what we have
		if(quantity > target->iPtr->quantity)
			quantity = target->iPtr->quantity;
		
		// Update *iPtr quantity and theatercash
		theatercash += quantity * target->iPtr->saleprice;
		target->iPtr->quantity -= quantity;
	}
	else
	{
		// It is guaranteed that item should be in inventory
		printf("This message should not appear.\n");
	}
	
	// Output
	printf("%s %d %d\n", target->iPtr->name, target->iPtr->quantity, theatercash);
}

void change_price(hashtable* table, char* word, int new_price)
{
	// Generate and use hash key
	int key = hashfunc(word, table->size);
	node* target = table->lists[key];
	
	// Traversal variables
	int isFound = 0;
	int checkFlag = 1;
	
	// Find target node
	do{
		// Each check is O(1) complexity
		++runtime;
		
		if(strcmp(target->iPtr->name, word) == 0)
		{
			// Target acquired
			isFound = 1;
			checkFlag = 0;
		}
		else
		{
			// Next node if applicable
			if(target->next != NULL)
				target = target->next;
			else
				checkFlag = 0;
		}
	}while(checkFlag);
	
	if(isFound == 1)
	{
		// Update saleprice
		target->iPtr->saleprice = new_price;
	}
	else
	{
		// It is guaranteed that item should be in inventory
		printf("This message should not appear.\n");
	}
}

int hashfunc(char* word, int size)
{
	int len = strlen(word);
	int res = 0;
	
	for(int i = 0; i < len; ++i)
		res = (1151 * res + (word[i] - 'a')) % size;
	
	return res;
}

void chomp(char* word)
{
	if(word[strlen(word) - 1] == '\n')
		word[strlen(word) - 1] = '\0';
}

void clearBuffer()
{
	while(getchar() != '\n');
}