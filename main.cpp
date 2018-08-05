#include "Database.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	Database &db = Database::getInstance();
	int select = 99;
	while (select != 0) {
		cout << "\n\n"
			<< " Welcome to your Book Store. \n Please choose one of the following:\t   \n\t\t\t\t\t\t\t\t\t   \n"
			<< " 1) Show All Books In Inventory.\t\t\t\t\t\t   \n 2) Show Open Orders.\t\t\t\t\t\t\t   \n"
			<< " 3) Show List of Customers.\t\t\t\t\t\t\t   \n 4) Show List of Suppliers.\t\t\t\t\t\t   \n"
			<< " 5) Show All Deals Between 2 Dates.\t\t\t\t\t   \n 6) Show All Books On Discount.\t\t\t\t\t"  
			<< "\n 7) Check If Book X Is In Stock.\t\t\t\t\t   \n"
			<< " 8) Show All Suppliers of Book X.\t\t\t\t\t   \n 9) Show Number OF Copies Of A Book Sold Since A Given Date.\t\t"   
			<< "\n 10) Show How Many Books A customer Has Bought Since A Given Date.\t   \n"
			<< " 11) Show The customer Who Bought The Most Books Since A Given Date.\t   \n" 
			<< " 12) Show The Supplier That Supplied The Most Books Since A Given Date."   
			<< "\n 13) Show All Orders Between Two Given Dates.\t\t\t\t   \n"
			<< " 14) Show All Orders That Turned Into a Deal Between Two Dates.\t   \n" 
			<< " 15) Show The Total Discount A customer Has Recieved Since A Given Date."    
			<< "\n 16) Show Store's Profit In A Given Quarter.\t\t\t\t   \n"
			<< " 17) Show customers That Joined After A Given Date.\t\t\t   \n" 
			<< " 18) Show Total Of Purcheses From A Supplier Between Two Given Dates.     \n"
			<< " 19) Show A Worker's Sales Between Two Dates.\t\t\t\t   \n" 
			<< " 20) Show Top 10 Most Sold Books Between Two Given Dates.\t\t   \n" 
			<< " 0) End the program.\t\t\t\t\t\t\t   \n"
			<< "\nYour Choice: ";
		cin >> select;

		switch (select) {

		case 0: {
			
			cout << "Bye!" << endl;
			exit(EXIT_SUCCESS);
		}

		case 1: {
			db.printallBooks();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 2: {
			db.printopenOrders();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 3: {
			db.printallcustomers();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 4: {
			db.printallSuppliers();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 5: {
			db.printalldealsbetwDates();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 6: {
			db.printbooksinDiscount();
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 7: {
			db.checkinInventory();
			cin.clear();
			break;
		}

		case 8: {
			db.printSupplierofBook();
			cin.clear();
			break;
		}

		case 9: {
			db.bookSellsSince();
			cin.clear();
			break;
		}

		case 10: {
			db.bookscustomerBoughtSince();
			cin.clear();
			break;
		}

		case 11: {
			db.bestCustomerDetails();
			cin.clear();
			break;
		}
		case 12: {
			db.bestSupplierDetails();
			cin.clear();
			break;
		}

		case 13: {
			db.numOfOrdersbetween();
			cin.clear();
			break;
		}

		case 14: {
			db.numOfDealsbetween();
			cin.ignore(10000, '\n');
			break;
		}

		case 15: {
			db.totalDiscount();
			cin.clear();
			break;
		}
		
		
		case 16: {
			db.totalOfIncome(); //missing
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}

		case 17: {
			db.newCustomerSince();
			cin.clear();
			break;
		}
		case 18: {
			db.totalPurchasesSupplierbetween();
			cin.clear();
			break;
		}

		case 19: {
			db.totalSalesWorkerbetween();
			cin.clear();
			break;
		}

		case 20: {
			db.top10SalesBooks();
			cin.clear();
			break;
		}

		default: {
			cout << "\ninvalid choice" << endl;
			select = 42;
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}




		}

		cout << "\nPress 1 to Return To the Main Menu.\nYour Choice: " << endl;
		select = 0;
		while (select != 1) {
			cin >> select;
			cin.clear();
			cin.ignore(10000, '\n');
		}


	}


	

	return 0;
}