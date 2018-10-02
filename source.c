/*
	
	Title			: Library Management System
	Language		: C
	Author			: Daljeet Singh Chhabra
	Date Created	: 22-07-2017
	Last Modified	: 15-10-2017

*/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

struct books {
	unsigned int num;
	char name[50],author[50];					
};

struct members {
        unsigned int num;
        char name[50],addr[100],mob[11],email[50];                   
};

struct records {
        unsigned int b_num,m_num;
};

///###########################################		FUNCTION  DECLARATIONS          #########################################################

void home();														//To display home page
void head();														//To create a banner that will be displayed on every page
void login();														//To verify the actual owner of the system
void menu();														//To display a menu so that the system will be user friendly

void addBook();														//To add books to file books.bin
void deleteBook();													//To delete book from file books.bin
void updateList(int);												//To maintain issue and return status in records.bin file
int searchList(int);												//To search a book issued in records.bin file
void viewSearchedList()	;											//To display details of searched book in records.bin file
int searchBook(int);												//To search a book from books.bin file
void viewSearchedBook();											//To display details of book searched in books.bin file
void addMember();													//To add a member of library in file members.bin
void deleteMember();												//To delete a member of library from file members.bin
int searchMember(int);												//To search the member  from the file members.bin
void viewSearchedMember();											//To display details of member searched in members.bin file

///#########################################     Function Declarations Ended       ########################################################

int main() {
	system("cls");
	login();
	return 0;
}

void head() {
    printf("\t###########################################################################");
	printf("\n\t############              Library management System            ############");
	printf("\n\t############                (UNDER DEVELOPMENT)                ############");
	printf("\n\t###########################################################################");
}


void login() {
	char us[20],pass[20],ch='\0';
	int i=3,j=0;
	while(i>=1) {
		system("cls");
		head();

		printf("\n\t\t\t\t\t***Login Page***");
		printf("\n\n\n\t\t\tusername => ");
		gets(us);
		printf("\n\t\t\tpassword => ");
		
		while(ch!=13) {
			ch=getch();
	
			if(ch!=13 && ch!=8) {
			putch('*');
			pass[j] = ch;
			j++;
			}
		}
		pass[j] = '\0';	
		
		if((strcmp(us,"admin")==0)&&strcmp(pass,"pass")==0) {
			printf("\n\n\n\t\t\tLOGIN SUCCESSFULL!!!\n\t\t\tPress any key to continue\n\n\n\n");
			getch();
			menu();	
			break;	
		}
		else {
			printf("\n\n\n\t\t\t\t\tINVALID CREDENTIALS!!!\n");
			printf("\n\n\t\t\t\t\t%d try remaining!!!\n\n\n",--i);
			getch();
		}	
	}	
	if(i<=0) {
		printf("\n\n\n\t\tYou have entered wrong credentials 3 times, system is exiting...\n\n\n\n");
		exit(-1);
	}
}

void menu() {
	int ch;
	do {
	    system("cls");
        head();
        
		printf("\n\n\n\t\t\t\t*** MENU ***");
		printf("\n\n\t\t1.Add Book");
		printf("\n\t\t2.Delete Book");
		printf("\n\t\t3.Issue Book");
		printf("\n\t\t4.Return Book");
		printf("\n\t\t5.Add Member");
		printf("\n\t\t6.Delete Member");
		printf("\n\t\t0.Exit");

		printf("\n\n\n\t\tEnter choice => ");
		scanf("%d",&ch);

		switch(ch) {
			case 1:
					addBook();                        
					break;
			case 2:
					deleteBook();
                    break;
			case 3:
                    updateList(1);
                    break;
			case 4:
                    updateList(2);
	                break;
			case 5:
                    addMember();
                    break;
			case 6:
                    deleteMember();
                    break;
			case 0:
					printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
	                break;
			default:
				printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
		}																						//Switch Ended
	}while(ch!=0);																				//Loop Ended
}																								//MENU Function Ended	

void addBook() {
	FILE *fpB;
	struct books b;

	system("cls");
	head();
	
	fpB=fopen("books.bin","ab");
	if(fpB==NULL) {
		printf("\n\nUnable to fetch records!!");
		getch();
		return;
	}
	printf("\n\nEnter Book No. => ");
	scanf("%u",&b.num);
	fflush(stdin);
	if(searchBook(b.num==1)) {
		printf("\n\n\t\t\tBook number already taken!! Try Again..");
		fclose(fpB);
		getch();
		return;
	}
	printf("\nEnter Book Name => ");
	gets(b.name);
	fflush(stdin);
	printf("\nEnter Author Name => ");
	gets(b.author);
	
	fwrite(&b,sizeof(b),1,fpB);
	fclose(fpB);

	printf("\n\n\t\t\t\tBook added Successfully!!!");
	getch();
}

void deleteBook() {
	unsigned int num,flag=0;
	FILE *fpR,*fpW;
	struct books b;
	
	system("cls");
	head();
	
	fpR=fopen("books.bin","rb");
	if(fpR=NULL) {
		printf("\n\n\t\tUnable to fetch records!!!");
		getch();
		return;
	}
	
	fpW=fopen("temp.bin","wb");
	printf("\n\n\t\tEnter Book no. to delete => ");
	scanf("%u",&num);

	while(fread(&b,sizeof(b),1,fpR)) {
		if(num!=b.num)
			fwrite(&b,sizeof(b),1,fpW);
		else {
			printf("\n\n\t\tBook Deleted");
			flag=1;
		}
	}
	if(flag==0)	{
		printf("\n\n\t\tNo such book exist!!!");
		getch();
	}
	fclose(fpR);
	fclose(fpW);
	remove("books.bin");
	rename("temp.bin","books.bin");
}

void updateList(int a) {
	if(a==1) {														//when argument 1 is passed it means that the book is to be issued
		unsigned int book,mem;
		FILE *fpR;
		struct records r;
				
		system("cls");
		head();
		printf("\n\nEnter member ID => ");
		scanf("%u",&mem);
		
		if(searchMember(mem)==1) {									//To confirm if entered member exist or not
			printf("\nEnter book no. => ");
			scanf("%u",&book);
			
			if(searchList(book)==0) {								//To check if book is already issued!!
				if(searchBook(book)==1) {							//To check if book exist in records
					fpR=fopen("records.bin","wb");
					if(fpR==NULL) {
						printf("\n\nUnable to fetch records!!");
						getch();
						return;
					}
					r.b_num=book;
					r.m_num=mem;
					fprintf(fpR,"\n%u %u",r.b_num,r.m_num);
					fclose(fpR);
					printf("\n\n\n\t\tBook issued to member ID => %u",mem);
					getch();
				}
				else {
					printf("\n\n\t\tNo such book exist in records!!!");
					getch();
					return;
				}
			}
			else {
				printf("\n\n\t\tBook no. %u is already issued!!!",book);
				getch();
				return;
			}
		}
		else {
			printf("\n\n\t\tNo Such member exist!!\n\t\tCheck member ID or add a new member..");
			getch();
			return;
		}
	}																						//Book Issue condition ends

	else
		if(a==2) {																			//when argument 2 is passed it means that the book is to be returned
			unsigned int book,flag=0;
			FILE *fpR,*fpW;
			struct records r;
				
			system("cls");
			head();
			printf("\n\nEnter book no. => ");
			scanf("%u",&book);
			
			fpR=fopen("records.bin","rb");
			if(fpR=NULL) {
				printf("\n\n\t\tUnable to fetch records!!!");
				getch();
				return;
			}
			fpW=fopen("temp.bin","wb");
			r.b_num=book;
				
			while(fread(&r,sizeof(r),1,fpR)) {		
				if(book!=r.b_num)
					fwrite(&r,sizeof(r),1,fpW);
				else {		
					printf("\n\n\t\tBook returned");
					flag=1;
				}
			}
			if(flag==0) {		
				printf("\n\n\t\tBook no. %u is not issued!!!",book);
				getch();
			}		
			fclose(fpR);
			fclose(fpW);
			remove("records.bin");
			rename("temp.bin","records.bin");			
		}																				//Return book ended	
}																						//function updateList() ended

int searchList(int book) {
	int flag=0;
	struct records r;
	FILE *fpR;
	
	fpR=fopen("records.bin","rb");
	if(fpR==NULL) {
		printf("\n\n\t\t\tRecords could not be fetched!!!");
		getch();
		return;
	}
	while(fread(&r,sizeof(r),1,fpR)) {
		if(book==r.b_num) {
			flag=1;
			fclose(fpR);
			return flag;
		}
	}
	fclose(fpR);
	return flag;
}

int searchBook(int book) {
	int flag=0;
	struct books b;
	FILE *fpB;

	printf("\n\nfubgdrgbhsfrikbughrsfikndbgkujndfbrhvgkujh")	;
	fpB=fopen("books.bin","rb");
	if(fpB==NULL) {
		printf("\n\n\t\t\tRecords could not be fetched!!!");
		getch();
		return flag;
	}
	while(fread(&b,sizeof(b),1,fpB)) {
		if(book==b.num) {
			flag=1;
			fclose(fpB);
			return flag;
		}
	}
	fclose(fpB);
	return flag;
}

void addMember() {
	FILE *fpM;
	struct members m;

	system("cls");
	head();
	
	fpM=fopen("members.bin","ab");
	if(fpM==NULL) {
		printf("\n\nUnable to fetch records!!");
		getch();
		return;
	}
	
	printf("\n\nEnter Member No. => ");
	scanf("%u",&m.num);
	fflush(stdin);
	if(searchMember(m.num)==1) {
		printf("\n\n\t\t\tMember number already taken!! Try Again..");
		fclose(fpM);
		getch();
		return;
	}
	printf("\nEnter Member Name => ");
	gets(m.name);
	fflush(stdin);
	printf("\nEnter Member address => ");
	gets(m.addr);
	printf("\nEnter Member mob. no. => ");
	scanf("%s",&m.mob);
	fwrite(&m,sizeof(m),1,fpM);
	fclose(fpM);

	printf("\n\n\t\t\t\tMember added Successfully!!!");
	getch();
}

void deleteMember() {
	unsigned int num,flag=0;
	FILE *fpM,*fpW;
	struct members m;
	
	system("cls");
	head();
	
	fpM=fopen("members.bin","rb");
	if(fpM=NULL) {
		printf("\n\n\t\tUnable to fetch records!!!");
		getch();
		return;
	}
	fpW=fopen("temp.bin","wb");
	printf("\n\n\t\tEnter member no. to delete => ");
	scanf("%u",&num);

	while(fread(&m,sizeof(m),1,fpM)) {
		if(num!=m.num)
			fwrite(&m,sizeof(m),1,fpW);
		else {
			printf("\n\n\t\tMember Deleted");
			flag=1;
		}
	}
	if(flag==0) {
		printf("\n\n\t\tNo such member exist!!!");
		getch();
	}
	fclose(fpM);
	fclose(fpW);
	remove("members.bin");
	rename("temp.bin","members.bin");
}

int searchMember(int mem) {
	int flag=0;
	struct members m;
	FILE *fpM;
	
	fpM=fopen("members.bin","rb");
	if(fpM==NULL) {
		printf("\n\n\t\t\tRecords could not be fetched!!!");
		getch();
		return;
	}
	while(fread(&m,sizeof(m),1,fpM)) {
		if(mem==m.num) {
			flag=1;
			fclose(fpM);
			return flag;
		}
	}
	fclose(fpM);
	return flag;
}

///############################################			Source Code for Library management System Ended			##################################################
