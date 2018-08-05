#ifndef _DB_H
#define _DB_H

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>

using namespace sql;
using namespace std;
#define DB_NAME "MyBook_Store"
#define DB_HOST "tcp://127.0.0.1/"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_PASS "h79519753"

class Database {
private:
	// singleton instance
	static Database *instance;

	Driver *driver;
	ConnectOptionsMap connection_properties;

	// private CTOR (singleton)s
	Database();
public:
	static Database& getInstance();

	Connection *getConnection();

	virtual ~Database() {}
	

	/* Menu Options Functions */

	//Option 1
	void printallBooks();
	//Option 2
	void printopenOrders();
	//Option 3
	void printallcustomers();
	//Option 4
	void printallSuppliers();
	//Option 5
	void printalldealsbetwDates();
	//Option 6
	void printbooksinDiscount();
	//Option 7
	void checkinInventory();
	// Option 8
	void printSupplierofBook();
	// Option 9
	void bookSellsSince();
	// Option 10
	void bookscustomerBoughtSince();
	// Option 11 
	void bestCustomerDetails();
	//Option 12
	void bestSupplierDetails();
	//Option 13
	void numOfOrdersbetween();
	//Option 14
	void numOfDealsbetween();
	//Option 15
	void totalDiscount();
	//Option 16
	void totalOfIncome();
	//Option 17
	void newCustomerSince();
	//Option 18
	void totalPurchasesSupplierbetween();
	//Option 19
	void totalSalesWorkerbetween();
	//Option 20
	void top10SalesBooks();


};
#endif