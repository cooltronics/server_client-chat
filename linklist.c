//Commands
//gcc linklist.c -o linklist
//valgrind --leak-check=yes --verbose ./linklist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linklist.h"

#define LINKLIST_DATABASE "xyz.db"

typedef enum {
	unSuccessful = -1,
	Successful = 0
}retVal;

//self referential structre
//contains at least one member which points to object of same data
//type
//linked list can grow and shrink dynamically
#define EXIT 0

typedef struct linklist
{
	char *logid;
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
	printf("10 : Check linklist length.\n");
	printf("11 : Save linklist in file.\n");
	printf("12 : Retrive linklist form file.\n");
	//printf("13 : Backup linklist in file.\n");
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
	if(head == NULL)
	{
		ERR("List is empty.");
	}
	while(curr != NULL)
	{
		PR("Name : %3s, ID : %3s, password : %3s",curr->name,curr->logid,curr->password);
		DBG("add : 0x%x",curr);
		curr = curr->next;
	}
}

void print_list_rev(linklist *head)
{
	linklist *curr = head;
	if(curr != NULL)
	{
		print_list_rev(curr->next);
		PR("Name : %3s, ID : %3s, password : %3s",curr->name,curr->logid,curr->password);
		DBG("add : 0x%x",curr);
	}
}

void scan_list(linklist *newnode)
{
	char buffer[128] = {0};
	printf("\nName :\t");
	scanf ("%*c%127[^\n]",buffer);
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

void allocate_memory(linklist *newnode, char* logid, char* name, char* password)
{
	DBG("IN");

	DBG("name : %s", name);
	newnode->name = (char *) malloc( (strlen(name) + 1) * sizeof(char) );
	strcpy(newnode->name, name);

	DBG("logid : %s",logid);
	newnode->logid = (char *) malloc( (strlen(logid) + 1) * sizeof(char) );
	strcpy(newnode->logid, logid);

	DBG("password : %s",password);
	newnode->password = (char *) malloc( (strlen(password) + 1) * sizeof(char) );
	strcpy(newnode->password, password);

	DBG("OUT");
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

int linklist_length(linklist* head)
{
	int count = 0;
	DBG("head is %x", head);
	linklist * curr = head;
	while(curr != NULL)
	{
		curr = curr->next;
		count++;
	}
	PR("count : %d",count);
	return count;
}

bool validate_node_number(linklist* head, int no)
{
	int count = 0;
	if(no == 0)
	{
		return 0;
	}

	count = linklist_length(head);
	if( (count+1) >= no)
	{
		return 1;
	}
	return 0;
}

int add_node(linklist** head,linklist** node,int number)		//add node at perticuler no.
{
	linklist *temp = *head;
	if(*head == NULL)
	{
		if(number != 1){
			ERR("Not valid node number.");
			return unSuccessful;
		}
	}
	while(--number)
	{
		if(temp->next != NULL){
			temp = temp->next;
		}
		else{
			break;				//break if number > node length
		}
	}
	if(number == 1)
	{
		temp->next = *node;
		(*node)->prev = temp;
		PR("added at tail.");
		goto out;
	}
	if(number == 0)
	{
		if(temp == NULL){
			*head = (*node);

			DBG("node->prev : %x",(*node)->prev);
			DBG("node->next : %x",(*node)->next);
			DBG("head : %x",(*head));
			DBG("node : %x",(*node));
		}
		else{
			(*node)->next = temp;
			(*node)->prev = temp->prev;
			temp->prev = *node;
			if((*node)->prev != NULL){
				(*node)->prev->next = (*node);
				PR("added in between.");
			}
			else{
				*head = (*node);
				PR("added at head.");
			}

			DBG("node->prev : %x",(*node)->prev);
			DBG("node->next : %x",(*node)->next);
			DBG("head : %x",(*head));
			DBG("node : %x",(*node));
			goto out;
		}
	}
	else{
		ERR("Not valid node number.");
		ERR("Enter valid number.");
		return unSuccessful;
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

linklist* remove_node(linklist ** head,linklist *node)
{
	linklist *temp;
	if((node)->prev == NULL && (node)->next == NULL)
	{
		DBG(">>1");
		temp = (node);
		*head = NULL;
	}
	else if((node)->prev == NULL)
	{
		DBG(">>2");
		temp = (node);
		*head = (*head)->next;
		(*head)->prev = NULL;
	}
	else if( (node)->next == NULL)
	{
		DBG(">>3");
		temp = node;
		(node)->prev->next = NULL;
	}
	else
	{
		DBG(">>4");
		temp = node;
		(node)->next->prev = (node)->prev;
		(node)->prev->next = (node)->next;
	}

	return temp;
}

linklist *del_from_start(linklist **head)
{
#if 0
	linklist *temp = NULL;
	if(*head == NULL)
	{
		ERR("List is empty.\n");
	}
	else if((*head)->next == NULL)
	{
		temp = *head;
		*head = NULL;
		PR("Linklist is now empty.\n");
	}
	else
	{
		temp = *head;
		*head = (*head)->next;
		(*head)->prev = NULL;
		//	temp = remove_node(head,*head);
	}
	return temp;
#else
	if(*head == NULL)
	{
		ERR("List is empty.\n");
		return NULL;
	}
	linklist *temp = NULL;
	temp = remove_node(head, *head);
	return temp;
#endif
}

linklist *del_from_end(linklist **head)
{
#if 0
	linklist *temp = NULL;
	linklist *tail = *head;

	if(*head == NULL)
		ERR("List is empty.\n");
	else if((*head)->next == NULL)
	{
		temp = *head;
		*head = NULL;
		PR("Linklist is now empty.\n");
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
#else
	if(*head == NULL)
	{
		ERR("List is empty.\n");
		return NULL;
	}
	linklist *temp = NULL;
	temp = remove_node(head, get_tail(*head));
	return temp;
#endif
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
	PR("Node : %s is going to delete.\n",name);
	linklist *temp = NULL;
	temp = search_node(head,name);
	if(temp != NULL)
	{
		PR("Node : %s is @ %p.\n",name,temp);
		temp = remove_node(head,temp);
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

void linklist_save(linklist *head, char* filename)
{
	//save linklist all node in file
	//this value can retrive in next linklist start up.
	linklist* curr = head;
	FILE*fp = fopen(filename,"w");
	for(;curr != NULL; )
	{
		fprintf(fp,"%s %s %s ",curr->logid, curr->name, curr->password );
		curr = curr -> next;
	}
	fclose(fp);
}

void linklist_retrive(linklist **head, char *filename)
{
	linklist* curr = *head;
	FILE*fp = fopen(filename,"r");
	int i = 0;
	char logid[128] = {0};
	char name[128] = {0};
	char password[128] = {0};

	for(i = 0 ; fscanf(fp,"%s %s %s ", logid, name, password ) != EOF ; i++)
	{
		curr = create_node();
		allocate_memory(curr, logid, name, password);
		add_node(head, &curr, i + 1);
	}
	fclose(fp);
}

//Only for singly linklist
//linklist* linklist_reverse(linklist **node)
void* linklist_reverse(linklist **node)
{
	/*linklist* curr;
	if(node == NULL || node->next == NULL)
	{
		return node;
	}
	curr = linklist_reverse(node->next);
	node->next->next = node;
	node->next = NULL;
	return curr;*/

	/*linklist* curr1 = node;
	linklist* curr2 = (node)->next;
	static linklist* temp;
	if(curr2->next != NULL)
	{
		linklist_reverse(curr1->next);
	}
	else
	{
		temp = curr2;
	}
	curr2->next = curr1;
	return temp;*/
	
	linklist* curr1 = *node;
	linklist* curr2 = curr1->next;

	DBG("curr1 : 0x%x curr2 : 0x%x",curr1, curr2);
	if(curr2 == NULL)
	{
		DBG("Now reversing recursively");
		return;
	}

	linklist_reverse(&curr2);
	DBG("curr1 : 0x%x->0x%x curr2 : 0x%x->0x%x",curr1, curr1->next, curr2, curr2->next);
	curr1->next->next = curr1;
	curr1->next = NULL;
	*node = curr2;
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
			break;
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
					DBG("start node->prev : %x",(newnode)->prev);
					DBG("start node->next : %x",(newnode)->next);
					DBG("start head : %x",head);
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
					newnode = add_node_at_end(&head);			
					DBG("end node->prev : %x",(newnode)->prev);
					DBG("end node->next : %x",(newnode)->next);
					DBG("end head : %x",head);
					break;
				}

			case 5: //delete from start
				{
					temp = del_from_start(&head);
					if(temp == NULL)
					{
						ERR("This name is not in entry.\n *** Try again *** \n");
					}
					else
					{
						PR("Name : %3s,ID : %3s,password : %3s Removed Successful",temp->name,temp->logid,temp->password);
						FREE_NODE(temp);
					}
					break;
				}

			case 6: //delete from end
				{
					temp = del_from_end(&head);
					if(temp == NULL)
					{
						ERR("This name is not in entry.\n *** Try again *** \n");
					}
					else
					{
						PR("Name : %3s,ID : %3s,password : %3s Removed Successful\n",temp->name,temp->logid,temp->password);
						FREE_NODE(temp);
					}
					break;
				}

			case 7: //selected delete
				{
					printf("Enter name want to delete : ");
					scanf("%*c%s",name1);
					temp = delete_selected(&head,name1);
					if(temp == NULL)
					{
						ERR("%s is not in entry.\n *** Try again *** \n",name1);
					}
					else
					{
						PR("Nama : %3s, ID : %3s, password : %3s Removed Successful\n",temp->name,temp->logid,temp->password);
						FREE_NODE(temp);
					}
					break;
				}
			case 8: //search and check password
				{
					char passwd[128];

					printf("Enter name : ");
					scanf("%*c%s",name1);
					printf("Enter password : ");
					scanf("%s",passwd);
					temp = search_node(&head,name1);
					if(temp == NULL)
					{
						ERR("%s is not in entry.\n *** Try again *** \n",name1);
					}
					else
					{
						PR("Nama : %3s,ID : %3s,password : %3s",temp->name,temp->logid,temp->password);
						if( check_password(temp,passwd) )
						{
							PR("Password matched.");
						}
						else
						{
							PR("Password not matched.");
						}
					}
					break;
				}
			case 9: //add node at given number
				{
					int no;
					int ret = 0;
					linklist *newnode = create_node();
					printf("Enter no. to add node @ : ");
					scanf("%d",&no);
					if(validate_node_number(head,no))			//check valid node number
					{
						scan_list(newnode);
						ret = add_node(&head, &newnode,no);		//add node at perticuler no.
						if(ret == unSuccessful)
						{
							FREE_NODE(newnode);
						}
					}
					else
					{
						PR("Node number validation failed.");
					}
					break;
				}
			case 10:
				{
					int length = 0;
					length = linklist_length(head);
					PR("Linklist length is : %d",length);
					break;
				}
			case 11:		//save in file
				{
					linklist_save(head, "xyz.db");
					PR("Linklist save in : %s",LINKLIST_DATABASE);
					break;
				}
			case 12:		//retrive from file
				{
					PR("Linklist retrive from : %s",LINKLIST_DATABASE);
					linklist_retrive(&head, "xyz.db");
					break;
				}
			case 66:		//retrive from file
				{
					int no = 0;
					printf("Enter no at which node added : ");
					scanf("%d",&no);
					linklist *newnode = create_node();
					scan_list(newnode);
					add_node(&head, &newnode, no);		//add node at perticuler no.
					break;
				}
			case 67:
				{
					linklist_reverse(&head);
					break;
				}
			default:
				{
					PR("No any case selected.");
				}

		}
	}
	// printf("Want to continue...\n then press 'y' : ");
	// scanf("%c%*c",&choice);
	// }while(1); //while(choice == 'y' || choice == 'Y');
	//exit main

while(head != NULL)
{
	DBG("head node->prev : %x",(head)->prev);
	DBG("head node->next : %x",(head->next));
	DBG(" head : %x",head);
	temp = remove_node(&head,head);
	DBG(" Node removed : %x\n",temp);
	FREE_NODE(temp);
}
exit(0);
}
