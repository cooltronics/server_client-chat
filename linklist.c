#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//assignment
//1.create a linked list of employees (empid, empname, salary)
//2.write a function to add node at start

//self referential structre
//contains at least one member which points to object of same data
//type
//linked list can grow and shrink dynamically
#define EXIT 0

#define	FREE_NODE(_NODE) do{	\
							free(_NODE->name); \
							free(_NODE->logid);	\
							free(_NODE->password);	\
							free(_NODE);	\
							}while(0)

typedef struct linklist
{
	char* logid;
	char *name;
	char *password;
	struct linklist *prev;
	struct linklist *next;
}linklist;

int choice_menu()
{
	int choice;
	printf("\n***************************\n");
	printf("1 : Print Linklist.\n");
	printf("2 : Print Linklist reverse.\n");
	printf("3 : Add node at start.\n");
	printf("4 : Add node at end.\n");
	printf("5 : Delete node from start.\n");
	printf("6 : Delete node from end.\n");
	printf("7 : Selected delete.\n");
	printf("8 : Match password.\n");
	printf("9 : Add node at n number.\n");
	printf("***************************");

	printf("\nEnter your choice : ");
	scanf("%d",&choice);
	return choice;
}

linklist *get_tail(linklist* head)
{
	linklist *tail;
	tail = head;
	while(tail->next != NULL)
	{
	tail = tail->next;
	}
	return tail;
}

void print_list(linklist *head)
{
	linklist *curr = head;
	while(curr != NULL)
	{
	printf("\nName : %3s, ID : %3s, passwordd : %3s",curr->name,curr->logid,curr->password);
	curr = curr->next;
	}
}

void print_list_rev(linklist *head)
{
	linklist *curr = head;
	if(curr != NULL)
	{
	print_list_rev(curr->next);
	printf("\nName : %3s, ID : %3s, passwordd : %3s",curr->name,curr->logid,curr->password);
	}
}

void scan_list(linklist *newnode)
{
	char buffer[128];
	printf("\nName :\t");
	scanf ("%s",buffer);
	newnode->name = (char *) malloc( (strlen(buffer) + 1) * sizeof(char) );
	strcpy(newnode->name,buffer);

	printf("ID :\t");
	scanf ("%s",buffer);
	newnode->logid = (char *) malloc( (strlen(buffer) + 1) * sizeof(char) );
	strcpy(newnode->logid,buffer);

	printf("Password :\t");
	scanf ("%s",buffer);
	newnode->password = (char *) malloc( (strlen(buffer) + 1) * sizeof(char) );
	strcpy(newnode->password,buffer);
}

linklist *create_node()
{
	linklist *new;
	new = (linklist *) malloc (sizeof(linklist));
	if(new != NULL)
	{
	new->prev = NULL;
	new->next = NULL;
	}
	return new;
}

int add_node(linklist** head,linklist** node,int number)		//add node at perticuler no.
{
	linklist *temp = *head;
	if(*head == NULL)
	{
		if(number > 1){
			printf("Not valid node number.\n");
			return -1;
		}
	}
	while(--number)
	{
		if(temp->next != NULL){
			temp = temp->next;
		}
		else{
			break;
		}
	}
	if(number == 1)
	{
		temp->next = *node;
		(*node)->prev = temp;
		goto out;
	}
	if(number == 0)
	{
		if(*temp == NULL){
			temp = *node;
		}
		else{
			*node->next = temp;
			*node->prev = temp->prev;
			if(temp->prev != NULL){
				temp->prev->next = *node;
			}
			temp->prev = *node;
		}
	}
	else{
		printf("Not valid node number.\n");
		return -1;
	}
	out:
		return 0;
}

linklist* add_node_at_start(linklist **head)
{
	linklist *newnode = create_node();
	scan_list(newnode);
	if(*head == NULL)
	{
	*head = newnode;
	//newnode->prev = *head;
	newnode->prev = NULL;
	}
	else
	{
	(*head)->prev = newnode;
	newnode->next = *head;
	(*head) = newnode;
	}
	return newnode;
}
linklist* add_node_at_end(linklist **head)
{
	linklist *newnode = create_node();
	scan_list(newnode);
	linklist *tail = NULL;
	if(*head == NULL)
	{
	*head = newnode;
	//newnode->prev = *head;
	newnode->prev = NULL;
	}
	else if((*head)->next == NULL)
	{
	(*head)->next = newnode;
	newnode->prev = *head;
	}
	else
	{
	tail = get_tail(*head);
	tail->next = newnode;

	newnode->prev = tail;
	}
	return newnode;
}

linklist* remove_node(linklist ** head,linklist **node)
{
	linklist *temp;
	if((*node)->prev == NULL && (*node)->next == NULL)
	{
		printf("1->\n");
		temp = (*node);
		*head = NULL;
	}
	else if((*node)->prev == NULL)
	{
		printf("2->\n");
		temp = (*node);
		*head = (*head)->next;
	}
	else if( (*node)->next == NULL)
	{
		printf("3->\n");
		temp = *node;
		(*node)->prev->next = NULL;
	}
	else
	{
		printf("4->\n");
		temp = *node;
		(*node)->next->prev = (*node)->prev;
		(*node)->prev->next = (*node)->next;
	}

	return temp;
}

linklist *del_from_start(linklist **head)
{
	linklist *temp = NULL;
	if(*head == NULL)
	printf("List is empty.\n");
	else if((*head)->next == NULL)
	{
	temp = *head;
	*head = NULL;
	printf("Linklist is now empty.\n");
	}
	else
	{
	temp = *head;
	*head = (*head)->next;
	(*head)->prev = NULL;
//	temp = remove_node(head,*head);
	}
	return temp;
}

linklist *del_from_end(linklist **head)
{
	linklist *temp = NULL;
	linklist *tail = *head;
	
	if(*head == NULL)
	printf("List is empty.\n");
	else if((*head)->next == NULL)
	{
	temp = *head;
	*head = NULL;
	printf("Linklist is now empty.\n");
	}
	else
	{
	while(tail->next->next != NULL)
	{
	tail = tail->next;
	}
	temp = tail->next;
	tail->next = NULL;
	}
	return temp;
}

linklist *search_node(linklist **head,char a[])
{
	char i;
	linklist *temp;
	temp = *head;

	if(*head == NULL)
	{
		printf("Linklist is empty.\n");
	}
	while(temp != NULL)
	{
		i = strcmp(temp->name,a);
/*		if(i == 0 && temp->next != NULL && temp->prev != NULL)
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			break;
		}
		else if(i == 0 && temp->next != NULL && temp->prev == NULL)
		{
			temp->next->prev = NULL;
			*head = temp->next;
			break;
		}
		else if(i==0 && temp->next == NULL && temp->prev != NULL)
		{
			temp->prev->next = NULL;
			break;
		}*/
		if(i == 0)
		{
			break;
		}
		temp = temp->next;
	}

	return temp;
}

linklist *delete_selected(linklist **head,char *name)
{
	printf("Node : %s is going to delete.\n",name);
	linklist *temp = NULL;
	temp = search_node(head,name);
	if(temp != NULL)
	{
		printf("Node : %s is @ %p.\n",name,temp);
		temp = remove_node(head,&temp);
	}
	return temp;
}

bool check_password(linklist* temp, char *password)
{
	if( strcmp(temp->password,password) )
		return false;
	else
		return true;
}

int main (int argc,char *argv[])
{
	linklist *head = NULL;
	linklist *tail = NULL;
	linklist *newnode = NULL;
	linklist *temp;
	char name1[32];

	char choice;
	int ch;
while(1)
{
	ch = choice_menu();
	if(ch == EXIT)
	{
		exit(1);
	}
	switch (ch)
	{
		case 1: //print linklist
		{
			print_list(head);
			break;
		}

		case 2: //print reverse
		{
			print_list_rev(head);
			break;
		}
		case 3: //add node at start
		{
			linklist* newnode = NULL;
			//newnode = create_node();
			//printf("Newnode : %p",newnode);
			//if(newnode == NULL)
			//{
			//	fprintf(stderr,"Node not created");
			//	exit(2);
			//}
			//scan_list(newnode);
			newnode = add_node_at_start(&head);
			printf("Newnode : %p",newnode);
			break;
		}

		case 4: //add node at end
		{
			linklist* newnode = NULL;
			//newnode = create_node();
			// printf("Newnode : %u",newnode);
			//if(newnode == NULL)
			//{
			//	fprintf(stderr,"Node not created");
			//	exit(2);
			//}
			//scan_list(newnode);
			add_node_at_end(&head);
			printf("Newnode : %p",newnode);
			break;
		}

		case 5: //delete from end
		{
			temp = del_from_start(&head);
			if(temp == NULL)
			{
				printf("This name is not in entry.\n *** Try again *** \n");
			}
			else
			{
				printf("Nama : %s,ID : %d,password : %s Removed Successful\n",temp->name,temp->logid,temp->password);
				free(temp);
			}
			break;
		}

		case 6: //delete from end
		{
			temp = del_from_end(&head);
			if(temp == NULL)
			{
				printf("This name is not in entry.\n *** Try again *** \n");
			}
			else
			{
				printf("Nama : %s,ID : %d,password : %s Removed Successful\n",temp->name,temp->logid,temp->password);
				free(temp);
			}
			break;
		}

		case 7: //search node
		{
			printf("Enter name : \n");
			scanf("%*c%s",name1);
			temp = delete_selected(&head,name1);
			if(temp == NULL)
			{
				printf("%s is not in entry.\n *** Try again *** \n",name1);
			}
			else
			{
				printf("Nama : %s,ID : %d,password : %s Removed Successful\n",temp->name,temp->logid,temp->password);
				free(temp);
		}
		break;
		}
		case 8: //search and check password
		{
			char passwd[128];

			printf("Enter name : \n");
			scanf("%*c%s",name1);
			printf("Enter password : \n");
			scanf("%s",passwd);
			temp = search_node(&head,name1);
			if(temp == NULL)
			{
				printf("%s is not in entry.\n *** Try again *** \n",name1);
			}
			else
			{
				printf("Nama : %s,ID : %d,password : %s \n",temp->name,temp->logid,temp->password);
				if( check_password(temp,passwd) )
				{
					printf("Password matched.\n");
				}
				else
				{
					printf("Password not matched.\n");
				}
			}
			break;
		}
		case 9: //
		{
			printf("Add node at n th number.\n");
			//add routine.
		}
	}
}
	// printf("Want to continue...\n then press 'y' : ");
	// scanf("%c%*c",&choice);
	// }while(1); //while(choice == 'y' || choice == 'Y');
	//exit main
	while(head->next != NULL)
	{
		temp = remove_node(&head,&head);
		FREE_NODE(temp);
	}
	exit(0);
}
