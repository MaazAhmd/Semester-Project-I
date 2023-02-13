#include <iostream>
#include<string>
#include<fstream>
using namespace std;

struct Bookname
{
	char name[20];
	int codes;
}bookname[50];

typedef struct bookdata
{
	int ISBN, av_quan, tot_quan;
	char title[50], author[50], pub_date[9];
}Books;

typedef struct userData
{
	char username[50];
	char password[20];
} userdata;
struct Lendingname {
	char namel[20];
	int codesl;
}lendingname[50];
typedef struct Lending {
	char dateofreturn[10], dateofissue[10], name[50];
	int isbn;
}lending;

typedef struct Fine {
	char name[50];
	char fineRs[10];
}fine;

void getnames(Bookname bookname[], int& index)
{
	fstream get;
	get.open("Books/files.txt", ios::in);
	for (int i = 0; get.getline(bookname[i].name, 20, '%'); i++)
	{
		index++;
	}
}
void extractIntegers(Bookname bookname[], int& index)
{
	for (int main = 0; main < index; main++)
	{
		char temp[100];
		strcpy_s(temp, 100, bookname[main].name);
		string str = temp;
		size_t n = str.size();
		string ext = "";
		for (int i = 0; i < n; i++)
		{
			if (str[i] - '0' >= 0 && str[i] - '0' <= 9)
				ext += str[i];
			else if (ext.size() > 0)
			{
				bookname[main].codes = stoi(ext);
				ext = "";
			}
		}
		if (ext.size() > 0)
			bookname[main].codes = stoi(ext);
	}
}
void readdata(Books books[], Bookname bookname[], int& index)
{

	fstream list;
	for (int i = 0; i < index; i++)
	{
		list.open(bookname[i].name, ios::in);
		list >> books[i].ISBN;
		list.getline(books[i].title, 100, '%');
		list.getline(books[i].author, 100, '%');
		list.getline(books[i].pub_date, 9, '%');
		list >> books[i].av_quan;
		list >> books[i].tot_quan;
		list.close();
	}
}
void displaydata(Books books[], int index)
{
	for (int j = 0; j < index; j++)
	{
		cout << books[j].ISBN;
		cout << books[j].title << " ";
		cout << books[j].author << " ";
		cout << books[j].pub_date << " ";
		cout << books[j].av_quan << " ";
		cout << books[j].tot_quan << " ";
		cout << endl;
	}

}
void addpath( int isbn, char code[])
{
	char arr1[20];
	strcpy_s(arr1, code);
	string t = to_string(isbn);
	char const* n_char = t.c_str();
	strcat_s(code, 20, n_char);
	strcat_s(code, 20, ".txt");
}
void addbook(Books books[], Bookname bookname[], char title[], int isbn, char author[], char pub_date[], int& index)
{
	char s_title[100] = " ";
	strcat_s(s_title, 20, title);
	books[index].ISBN = isbn;
	strcpy_s(books[index].title, s_title);
	strcpy_s(books[index].author, author);
	strcpy_s(books[index].pub_date, pub_date);
	books[index].av_quan = 1;
	books[index].tot_quan = 1;
	char code[20] = "Books/";
	addpath(isbn, code);
	strcpy_s(bookname[index].name, code);
	index++;
}
void addquantity(Books books[], int isbn, int quan, int index)
{
	for (int i = 0; i < index; i++)
	{
		if (books[i].ISBN == isbn)
		{
			books[i].av_quan += quan;
			books[i].tot_quan += quan;
		}
	}
}
void removename(Bookname bookname[], int rem_isbn, int& index)
{
	for (int i = 0; i < index; i++)
	{
		if (rem_isbn == bookname[i].codes)
		{
			static int b_index = i;
			for (int i = b_index; i <= index; i++)
				bookname[i] = bookname[i + 1];
		}
	}
}
void removebook(Books books[], Bookname bookname[], int rem_isbn, int quan, int& index)
{
	int i;
	for (i = 0; i < index; i++)
	{
		if (bookname[i].codes == rem_isbn)
		{
			if (quan == -1)
			{
				char del[100];
				strcpy_s(del, bookname[i].name);
				remove(del);
				removename(bookname, rem_isbn, index);
				for (int j = i; j <= index; j++)
				{
					books[j] = books[j + 1];
				}
				index--;
				break;
			}
			else
			{
				books[i].av_quan -= quan;
				books[i].tot_quan -= quan;
			}
		}
	}
}

void writename(Bookname bookname[], int& index)
{
	fstream put;
	put.open("Books/files.txt", ios::out);
	for (int i = 0; i < index; i++)
	{
		put << bookname[i].name << '%';
	}
	put.close();
}
void writedata(Books books[], Bookname bookname[], int& index)
{
	fstream list;
	for (int i = 0; i < index; i++)
	{
		list.open(bookname[i].name, ios::out);
		list << books[i].ISBN;
		list << books[i].title << '%';
		list << books[i].author << '%';
		list << books[i].pub_date << '%';
		list << books[i].av_quan << ' ';
		list << books[i].tot_quan;
		list.close();
	}
}
int load_users(userdata user[])
{
	int i = 0;
	fstream login;
	login.open("Login Data.txt", ios::in);
	if (login.is_open())
	{
		for (i; !login.eof() && i < 10; i++)
		{
			login.getline(user[i].username, 50, '|');
			login.getline(user[i].password, 20, '\n');
		}
		login.close();
		return i;
	}
	else
		return -1;
}
bool login_check(userdata user[], int total_users, string in_username, string in_password)
{
	for (int i = 0; i < total_users; i++)
	{
		if ((in_username == user[i].username) && (in_password == user[i].password))	//User found
			return true;
	}
	return false;
}
bool login_input()
{
	while (true)
	{
		int c;
		cout << "1.Login\t2.SignUp";
		cout << "\nPlease enter your choice: ";
		cin >> c;
		if (c == 1)
			return true;
		else if (c == 2)
			return false;
		else cout << "\nWrong input. Please try again.\n\n";
	}
}
bool login(string& username, userdata user[], int& total_users)
{
	string password;
	cout << "\nEnter Username: "; cin >> username;
	cout << "Enter Password: "; cin >> password;	//Convert password into * using getch using seperate function
	if (login_check(user, total_users, username, password))
	{
		cout << "\nLogin Successful.";
		return true;
	}
	else return false;
}
bool signup(userdata user[], int& total_users, string& username)
{
	if (total_users > 10)
	{
		cout << "\nCannot create a new user. Maximum limit reached.";
		return false;
	}
	char newuser[50], newpassword[20];
	cout << "\nSignUp:";
signup:
	cout << "\nEnter a username: "; cin >> newuser;
	for (int i = 0; i < total_users; i++)
	{
		if (strcmp(user[i].username, newuser) == 0)
		{
			cout << "\nUsername already taken. Please try again.\n";
			goto signup;
		}
	}
createpassword:
	cout << "\nCreate a Password (Minimum 4 characters): "; cin >> newpassword;
	if (strlen(newpassword) < 4)
	{
		cout << "\nInvalid password. Minimum 4 characters.\n";
		goto createpassword;
	}
	strcpy_s(user[total_users].username, newuser);	//Value only copied to the array. Not on the files.
	strcpy_s(user[total_users].password, newpassword);
	total_users++;
	cout << "\nAccount created successfully.\n";
	username = newuser;
	fstream write;
	write.open("Login Data.txt", ios::app);
	if (write.is_open())
	{
		write << endl << newuser << "|" << newpassword;		//Now copied to the files too
		write.close();
	}
	else
	{
		cout << "\nUnable to open file.\n";
		return false;
	}
	return true;
}

int menu()
{
	while (true)
	{
		int choice;
		cout << "\n1. Search a book in the database\n2. Add a book in the database\n3. Remove a book from the database\n4. Open the Lending Menu";
		cout << "\n8. Sign Out\n9. Exit the program\nEnter your choice:  ";
		cin >> choice;
		//if (choice > 0 && choice < 5 && choice != 9)
		return choice;
		cout << "\nWrong input. Please try again.";
	}
}
void displaybookinformation(Books books[], int size, int index) {
	cout << "Book name: ";
	cout << books[index].title << endl;
	cout << "Author's Name: ";
	cout << books[index].author << endl;
	cout << "Publisher's date: ";
	cout << books[index].pub_date << endl;
	cout << "Quantity avalaible: ";
	cout << books[index].av_quan << endl;
	cout << "Total Quantity: ";
	cout << books[index].tot_quan << endl;
}

int searchbook(Books books[], int size, int isbn)
{
	for (int i = 0; i < size; i++)
	{
		if (books[i].ISBN == isbn)
		{
			return i;
		}
	}
	return -1;
}
void getnameslending(Lendingname lendingname[], int& sizel)
{
	fstream getl;
	getl.open("Lending/lending.txt", ios::in);
	for (int i = 0; getl.getline(lendingname[i].namel, 20, '%'); i++)
		sizel++;
}
void extractIntegerslending(Lendingname lendingname[], int& sizel)
{
	for (int mainl = 0; mainl < sizel; mainl++)
	{
		char templ[100];
		strcpy_s(templ, 100, lendingname[mainl].namel);
		string strl = templ;
		size_t l = strl.size();
		string extl = "";
		for (int i = 0; i < l; i++)
		{
			if (strl[i] - '0' >= 0 && strl[i] - '0' <= 9)
				extl += strl[i];
			else if (extl.size() > 0)
			{
				lendingname[mainl].codesl = stoi(extl);
				extl = "";
			}
		}
		if (extl.size() > 0)
			lendingname[mainl].codesl = stoi(extl);
	}
}
void loaddatainlending(lending arr[],Lendingname lendingname[], int& sizel)
{
	int i;
	fstream data;
	for (i = 0; i<sizel; i++)
	{
		data.open(lendingname[i].namel, ios::in);
		data >> arr[i].isbn;
		data.getline(arr[i].dateofissue, 15, '%');
		data.getline(arr[i].dateofreturn, 15, '%');
		data.getline(arr[i].name, 100, '\n');
		data.close();
	}
}
void loadfines(fine finelist[], int& size)
{
	int i = 0;
	fstream file;
	file.open("Fines.txt", ios::in);
	if (file.is_open())
	{
		for (i = 0; !file.eof(); i++)
		{
			file.getline(finelist[i].name, 50, '|');
			file.getline(finelist[i].fineRs, 10, '\n');
		}
		size = i;
	}
	file.close();
}
void writefines(fine finelist[], int size)
{
	fstream file;
	file.open("Fines.txt", ios::out);
	if (file.is_open())
	{
		for (int i = 0; i <= size; i++)
		{
			file << finelist[i].name << "|";
			file << finelist[i].fineRs << endl;
		}
	}
	file.close();
}
void writenamelending(Lendingname lendingname[], int& sizel)
{
	fstream putl;
	putl.open("Lending/lending.txt", ios::out);
	for (int i = 0; i < sizel; i++)
	{
		putl << lendingname[i].namel << '%';
	}
	putl.close();
}
void writedatainlending(lending arr[], int size)
{
	fstream list;
	list.open("Lending/lending.txt", ios::out);
	for (int i = 0; i < size; i++)
	{
		list << arr[i].isbn;
		list << arr[i].dateofissue << "%";
		list << arr[i].dateofreturn << "%";
		list << arr[i].name << endl;
	}
	list.close();
}
int searchinlending(lending arr[], int size, int isbn)
{
	for (int i = 0; i < size; i++)
	{
		if (arr[i].isbn == isbn)
		{
			return i;
		}
	}
	return -1;
}
void addpathlending(int isbn, char text[])
{
	char arrl[20];
	strcpy_s(arrl, text);
	string l = to_string(isbn);
	char const* l_char = l.c_str();
	strcat_s(text, 20, l_char);
	strcat_s(text, 20, "l.txt");
}
void addbookinlending(lending arr[],Lendingname lendingname[], char name[], int isbn, char dateofissue[], char dateofreturn[], int& sizel)
{
	arr[sizel].isbn = isbn;
	strcpy_s(arr[sizel].name, name);
	strcpy_s(arr[sizel].dateofissue, dateofissue);
	strcpy_s(arr[sizel].dateofreturn, dateofreturn);
	char text[20] = "Lending/";
	addpathlending(isbn, text);
	strcpy_s(lendingname[sizel].namel, text);
	sizel++;
}
void removenamelending(Lendingname lendingname[], int isbn, int& sizel)
{
	for (int i = 0; i < sizel; i++)
	{
		if (isbn == lendingname[i].codesl)
		{
			static int b_index = i;
			for (int i = b_index; i <= sizel; i++)
				lendingname[i] = lendingname[i + 1];
		}
	}
}
void removebookinlending(lending lend[],Lendingname leningname[], int isbn, int quan, int& sizel)
{
	int i;
	for ( i = 0; i < sizel; i++)
	{
		if (lendingname[i].codesl == isbn)
		{
			if (quan == -1)
			{
				char dell[100];
				strcpy_s(dell,100, lendingname[i].namel);
				remove(dell);
				removenamelending(lendingname,isbn, sizel);
				for (int j = i; j <= sizel; j++)
				{
					lend[j] = lend[j + 1];
				}
				sizel--;
				break;
			}
		}
	}
}

bool lend(lending arr[],Lendingname lendingname[], Books books[], int& sizeL, int sizeB)
{
	int isbn, i;
	char name[100], date[10], datereturn[10];

	cout << "\nEnter book's ISBN number: ";
	cin >> isbn;
	i = searchbook(books, sizeB, isbn);
	if (i == -1 || books[i].av_quan == 0)
	{
		cout << "\nBook not avalaible ";
		return false;
	}
	else
	{
		cout << "Enter date of issue: ";
		cin >> date;
		cout << "Enter expected date of return: ";
		cin >> datereturn;
		cin.ignore();
		cout << "Enter person's name : ";
		cin.getline(name, 100, '\n');
		cin.clear();

		addbookinlending(arr,lendingname, name, isbn, date, datereturn, sizeL);

		books[i].av_quan--;

		return true;
	}
}
bool returning(lending arr[],Lendingname lendingname[], Books books[], int& sizel, int sizeb, fine finelist[], int& sizeF)
{
	int isbn, i, t;
	char fine[10];
	cout << "\nEnter book's ISBN number : ";
	cin >> isbn;

	i = searchinlending(arr, sizel, isbn);
	if (i == -1)
	{
		cout << "\nError. Book not found.\n";
		return false;
	}
	for (int j = 0; j < sizeb; j++)
	{
		if (books[j].ISBN == isbn)
		{
			books[j].av_quan++;
			break;
		}
	}
	cout << "\n1. Book was returned on time\t2. Late\n";
	cin >> t;
	if (t == 2)
	{
		cout << "\nEnter the amount of fine : ";
		cin >> fine;
		cin.ignore();

		strcpy_s(finelist[sizeF].fineRs,10, fine);
		strcpy_s(finelist[sizeF].name, arr[i].name);
		writefines(finelist, sizeF);
		sizeF++;
		cout << "\nBook successfully returned with fine Rs." << fine << endl;
		removebookinlending(arr,lendingname, isbn, -1, sizel);
		return true;
	}
	else
	{
		cout << "\nBook returned successfully. No fine.\n";
		removebookinlending(arr,lendingname, isbn, -1, sizel);
	}
	return true;
}

void main_searchbook(Books books[], int size)
{
	char c;
	while (true)
	{
		int isbn;
		cout << "\nEnter the ISBN number of the book (Enter -1 to go back): ";
		cin >> isbn;
		if (isbn == -1)
			break;
		int i = searchbook(books, size, isbn);	//books is the array of the books with datatype book
		if (i == -1)
		{
			cout << "\nBook not found. Do you want to try again?\n1.Yes\t2.Back\t";
			cin >> c;
			if (c == '1')
				continue;
			else
				break;		//take a look here
		}
		else
		{
			cout << "\nBook found:\n\n";
			displaybookinformation(books, size, i);
			break;
		}
	}
}
void main_addbook(Books books[], int& size)
{
	char bkname[100], author[100], datepub[9];
	int isbn, quantity, totalquantity{};
	cout << "\nEnter the following data about the book: ";
	cout << "\nISBN number: "; cin >> isbn;
	int n = searchbook(books, size, isbn);
	if (n != -1)
	{
		cout << "\nBook already found with the title: " << books[n].title << endl;
		cout << "\nEnter the quantity you want to add: "; cin >> quantity;
		addquantity(books, isbn, quantity, size);
	}
	else
	{
		cout << "Name of the book: "; cin >> bkname;
		cout << "Author name: "; cin >> author;
		cout << "Date of publishing: "; cin >> datepub;
		cout << "Quantity of the book: "; cin >> quantity;
		addbook(books, bookname, bkname, isbn, author, datepub, size);	//adds the book in the "books" array
	}
}
bool main_removebook(Books books[], int& size)
{
	int isbn, quantity;
	char c;
	cout << "\nEnter the ISBN number of the book you want to remove: ";
	cin >> isbn;
choice:
	cout << "\n1. Remove the book from the database\n2. Remove some quantity of the book";
	cout << "\nEnter your choice: "; cin >> c;
	if (c == '1')
	{
		int n = searchbook(books, size, isbn);
		if (n != -1)
		{
			removebook(books, bookname, isbn, -1, size);	//if (removebook(books, isbn, -1, size))
			return true;
		}
		else
			return false;
	}
	else if (c == '2')
	{
		cout << "\nEnter quantity: ";
		cin >> quantity;
		int n = searchbook(books, size, isbn);

		if (n != -1)
		{
			removebook(books, bookname, isbn, quantity, size);	//if (removebook(books, isbn, quantity, size))
			return true;
		}
		else
			return false;
	}
	else
		goto choice;

	return false;
}

int main()
{
	Books books[50];
	int size = 0;
	getnames(bookname, size);
	extractIntegers(bookname, size);
	readdata(books, bookname, size);
	writename(bookname, size);
	writedata(books, bookname, size);
	displaydata(books, size);

	fine finearray[50];
	lending lendarray[50]{};
	int sizef = 0, sizel = 0;
	int c2, c3;
	string username;
	userdata user[10];	//Max users are 10
	int total_users = load_users(user);
	getnameslending(lendingname, sizel);
	extractIntegerslending(lendingname, sizel);
	loaddatainlending(lendarray,lendingname, sizel);
	loadfines(finearray, sizef);
Login:
	if (login_input())
	{
		if (!login(username, user, total_users))
		{
			cout << "\nUsername/Password incorrect. Please try again.\n";
			if (!login(username, user, total_users))
			{
				cout << "\nUsername/Password incorrect.\nDo you want to sign-up instead? or continue?\n\n";
				goto Login;
			}
		}
	}
	else
	{
		if (!signup(user, total_users, username))
		{
			cout << "\nError Occurred. Please try logging in.\n";
			goto Login;
		}
	}

	cout << "\n\nCurrently signed in with username: " << username << endl << endl;

	while (true)
	{
		c2 = menu();

		switch (c2)
		{
		case 1:
		{
			main_searchbook(books, size);
			continue;
		}
		case 2:	//Convert this to a seperate function too
		{
			main_addbook(books, size);
			cout << "\nBook Added Successfully.\n";
			continue;
		}
		case 3:
		{
			if (main_removebook(books, size))
				cout << "\nBook removed successfully.\n";
			else
				cout << "\nBook not found.\n";
			continue;
		}
		case 4:
		{
			int c;


			cout << "\nCurrent elements in the file: ";	//Just for checking
			for (int i = 0; i < sizel; i++)
			{
				cout << "\nBook lended to " << lendarray[i].name;
				cout << "\nDate of issue " << lendarray[i].dateofissue;
				cout << "\nDate of return " << lendarray[i].dateofreturn;
				cout << "\nISBN " << lendarray[i].isbn << endl;
			}

			cout << "\n1. Returning a book\t2. Lending a book\n";
			cin >> c;
			if (c == 1)
			{
				returning(lendarray,lendingname, books, sizel, size, finearray, sizef);
			}
			else if (c == 2)
			{
				lend(lendarray,lendingname, books, sizel, size);

				for (int i = 0; i < sizel; i++)	//Just for checking
				{
					cout << "\nBook lended to " << lendarray[i].name;
					cout << "\nDate of issue " << lendarray[i].dateofissue;
					cout << "\nDate of return " << lendarray[i].dateofreturn;
					cout << "\nISBN " << lendarray[i].isbn << endl;
				}
			}
			writenamelending(lendingname, sizel);
			writedatainlending(lendarray, sizel);
			continue;
		}

		case 8:
		{
			cout << "\n\nSuccesfully Logged out.\n";
		inputc3:
			cout << "\n1.Exit the program\t2.Continue\n";
			cin >> c3;
			if (c3 == 1)
				break;
			else if (c3 == 2)
				goto Login;
			else
				cout << "\nInvalid input: Please try again.\n";
			goto inputc3;
		}
		case 9:
		{
			cout << "\n\n\nExiting the Program.\n\n\n";
			break;
		}
		}
		break;
	}
	displaydata(books, size);
	writedata(books, bookname, size);
	writename(bookname, size);
	return 0;
}