#include "Database.h"
//#include <iostream>

void clearCin() {
	string temp;
	getline(cin, temp);
}


Database *Database::instance = 0;

Database::Database() : driver(get_driver_instance()) {
	connection_properties["hostName"] = DB_HOST;
	connection_properties["port"] = DB_PORT;
	connection_properties["userName"] = DB_USER;
	connection_properties["password"] = DB_PASS;
	connection_properties["OPT_RECONNECT"] = true;
}
	

Database & Database::getInstance() {
	if (Database::instance == 0) {
		instance = new Database();
	}
	return *instance;
}

Connection * Database::getConnection() {
	try {
		Connection *con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		return con;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
	return 0;
}


//Option 1
void Database::printallBooks() {
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	Statement *stmt = con->createStatement();
	ResultSet *rset = stmt->executeQuery("SELECT * FROM book WHERE current_stock != 0");
	int counter = 1;
	rset->beforeFirst();
	cout << "Books Currently In Stock:" << endl;

	while (rset->next()) {
		cout << counter << ") " << rset->getString("name") << ". By: " << rset->getString("author") << "." << endl;
		counter++;
	}

	delete con;
	delete stmt;
	delete rset;
	counter = 1;
}

//Option 2
void Database::printopenOrders() {

	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	Statement *stmt = con->createStatement();
	ResultSet *rset = stmt->executeQuery("SELECT * FROM orders inner join order_book WHERE order_status != 'Closed' AND orders.order_num = order_book.order_num;");

	rset->beforeFirst();
	cout << "Open Orders:" << endl;

	while (rset->next()) {
		cout << "Order Number: " << rset->getUInt("order_num") << ". Order Date: " << rset->getString("order_date") << ". Status: " << rset->getString("order_status") << "." << endl;
	}

	delete con;
	delete stmt;
	delete rset;
}

//Option 3
void Database::printallcustomers() {
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	Statement *stmt = con->createStatement();
	ResultSet *rset = stmt->executeQuery("SELECT * FROM customer;");

	rset->beforeFirst();
	cout << "Customers List:" << endl;

	while (rset->next()) {
		cout << "id: " << rset->getUInt("customer_id") << "\tName: "
			<< rset->getString("first_name") << " " << rset->getString("last_name") << "\t\tPhone Number: "
			<< rset->getString("phone_num") <<endl;
	}

	delete con;
	delete stmt;
	delete rset;
}

//Option 4
void Database::printallSuppliers() {
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	Statement *stmt = con->createStatement();
	ResultSet *rset = stmt->executeQuery("SELECT * FROM supplier;");

	rset->beforeFirst();
	cout << "Suppliers List:" << endl;

	while (rset->next()) {
		cout << "Supplier id: " << rset->getUInt("supplier_id") << "\t\t Supplier Name: "
			<< rset->getString("supplier_name") << endl;
	}

	delete con;
	delete stmt;
	delete rset;
}

//Option 5
void Database::printalldealsbetwDates() {
	string date;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM deal WHERE deal.deal_date BETWEEN ? AND ?;");

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	cin >> date;
	pstmt->setString(1, date);

	cout << "Please Enter End Date:> ";
	cin >> date;

	pstmt->setString(2, date);

	rset = pstmt->executeQuery();

	rset->beforeFirst();
	cout << "Deals:" << endl;

	while (rset->next()) {
		cout << "Deal Number: " << rset->getUInt("deal_num") << ". \tDeal Date: "
			<< rset->getString("deal_date") << "." << endl;
	}

	delete con;
	delete pstmt;
	delete rset;
}

//Option 6
void Database::printbooksinDiscount() {
	int counter = 1;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	Statement *stmt = con->createStatement();
	rset = stmt->executeQuery("SELECT * FROM book WHERE book.global_discount > 0;");

	rset->beforeFirst();
	cout << "Books On Discout:" << endl;

	while (rset->next()) {

		cout << counter << ") " << rset->getString("name") << ", By "
			<< rset->getString("author") << ",\tIs On A " << rset->getDouble("global_discount") * 100 << "% Discount." << endl;
		++counter;
	}

	delete con;
	delete stmt;
	delete rset;
}

//Option 7
void Database::checkinInventory() {

	string bookName;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM book WHERE book.name = ?;");

	cout << "Please Enter Book Name:> ";
	clearCin();
	getline(cin, bookName);
	pstmt->setString(1, bookName);
	rset = pstmt->executeQuery();

	rset->beforeFirst();

	if (rset->next()) {
		int stock = rset->getInt("current_stock");
		cout << "The Book '" << rset->getString("name") << "' Is Currently ";

		if (stock) {
			cout << "In Stock With " << stock << " Units." << endl;
		}
		else cout << " NOT In Stock!" << endl;
	}

	else cout << "Book Named '" << bookName << "' Was Not Found In The Store..." << endl;

	delete con;
	delete pstmt;
	delete rset;
}

// Option 8
void Database::printSupplierofBook() {

	string bookName;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("CREATE TABLE temp AS SELECT * FROM book inner join supplier_book WHERE book.name = supplier_book.book_name AND supplier_book.book_name = ? group by supplier_id;");

	cout << "Please Enter Book Name:> ";
	clearCin();
	getline(cin, bookName);
	pstmt->setString(1, bookName);
	pstmt->execute();
	pstmt = con->prepareStatement("SELECT supplier.supplier_id, supplier_name FROM temp inner join supplier WHERE temp.supplier_id = supplier.supplier_id;");
	rset = pstmt->executeQuery();

	rset->beforeFirst();

	cout << "The Book '" << bookName << "' Is Supplied By: " << endl;
	while (rset->next()) {
		cout << "Supplier Number: " << rset->getString("supplier_id") << ". Supplier Name: " << rset->getString("supplier_name") << "." << endl;
	}

	pstmt = con->prepareStatement("DROP TABLE temp");
	pstmt->execute();

	delete con;
	delete pstmt;
	delete rset;
}

// Option 9
void Database::bookSellsSince() {

	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM deal INNER JOIN deal_book WHERE deal.deal_num = deal_book.deal_num AND book_name = ? AND deal_date >= ?;");

	cout << "Please Enter Book Name:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(1, temp);
	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Date:> ";
	getline(cin, temp);
	pstmt->setString(2, temp);

	rset = pstmt->executeQuery();

	rset->beforeFirst();
	if (rset->next()) {
		cout << "The Book '" << rset->getString("book_name") << "' Was Ordered " << rset->rowsCount() << " Times Since " << temp << endl;
	}

	else cout << "Invalid Book Name Or Date Or the Book was not Sold " << endl;

	delete con;
	delete pstmt;
	delete rset;
}

// Option 10
void Database::bookscustomerBoughtSince() {

	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM deal INNER JOIN deal_book WHERE deal.deal_num = deal_book.deal_num AND deal.customer_id = ? AND deal.deal_date >= ?;");

	cout << "Please Enter customer id:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(1, temp);
	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Date:> ";
	getline(cin, temp);
	pstmt->setString(2, temp);

	rset = pstmt->executeQuery();

	rset->beforeFirst();

	if (rset->next()) {

		pstmt = con->prepareStatement("SELECT first_name, last_name FROM customer WHERE customer_id = ?;");
		string test = rset->getString("customer_id");
		pstmt->setString(1, test);
		ResultSet *rset2 = pstmt->executeQuery();

		if (rset2->first()) {
			cout << rset2->getString("first_name") << " " << rset2->getString("last_name") << " Has Ordered " << rset->rowsCount() << " Books Since " << temp << endl;
		}
	}

	else cout << "Invaid customer id Or Date." << endl;

	delete con;
	delete pstmt;
	delete rset;
}

// Option 11 
void Database::bestCustomerDetails() {

	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT customer.first_name, customer.last_name, SUM(deal_books_count.count_books) as counter FROM customer INNER JOIN(SELECT deal.customer_id, COUNT(*) AS count_books FROM deal INNER JOIN deal_book WHERE deal_book.deal_num = deal.deal_num AND deal.deal_date >= ? GROUP BY deal.deal_num) AS deal_books_count ON deal_books_count.customer_id = customer.customer_id group by first_name ORDER BY counter desc;");
	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Date:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(1, temp);
	rset = pstmt->executeQuery();

	if (rset->first()) {
		cout << rset->getString("first_name") << " " << rset->getString("last_name") << ", Has Ordered The Most Books (" << rset->getUInt("counter") << ") Since " << temp << endl;
	}

	else cout << "Invalid Date or No customers Found That Ordered Any Books After The Date That Was Given." << endl;


	delete con;
	delete pstmt;
	delete rset;
}

//Option 12
void Database::bestSupplierDetails() {
	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT supplier.supplier_id, supplier.supplier_name, SUM(order_books_count.count_books) as counter	FROM supplier INNER JOIN (SELECT orders.supplier_id, COUNT(*) AS count_books FROM orders "
		"INNER JOIN order_book WHERE order_book.order_num = orders.order_num AND orders.order_date >= ? "
		"GROUP BY orders.order_num) AS order_books_count WHERE order_books_count.supplier_id = supplier.supplier_id "
		"group by supplier_id ORDER BY counter desc;");

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter A Date:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(1, temp);

	rset = pstmt->executeQuery();
	rset->first();

	if (rset->first()) {
		cout << rset->getString("supplier_name") << " Has Supplied " << rset->getString("counter") << " Books Since " << temp << ". More Than Any Other Supplier." << endl;
	}
	else cout << "Invalid Date." << endl;

	delete con;
	delete pstmt;
	delete rset;
}

//Option 13
void Database::numOfOrdersbetween() {

	string temp1, temp2;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM orders where order_date >= ? AND order_date <= ?;");

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	clearCin();
	getline(cin, temp1);
	pstmt->setString(1, temp1);
	cout << "Please Enter End Date:> ";
	getline(cin, temp2);
	pstmt->setString(2, temp2);
	rset = pstmt->executeQuery();

	if (rset->first()) {
		cout << "Number Of Orders Between " << temp1 << " And " << temp2 << " Is: " << rset->rowsCount() << "." << endl;
	}
	else cout << "Invalid Date or No Orders Found That Where Ordered Between Dates." << endl;


	delete con;
	delete pstmt;
	delete rset;
}

//Option 14
void Database::numOfDealsbetween() {

	string temp1, temp2;
	int counter = 1;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT orders.order_num, orders.order_date, first_name, last_name FROM orders inner join order_book inner join customer WHERE order_date BETWEEN ? AND ? AND order_status = 'Closed'"
		"AND orders.order_num = order_book.order_num AND orders.customer_id = customer.customer_id group by order_num;");

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	clearCin();
	getline(cin, temp1);
	pstmt->setString(1, temp1);

	cout << "Please Enter End Date:> ";
	getline(cin, temp2);
	pstmt->setString(2, temp2);

	rset = pstmt->executeQuery();
	rset->beforeFirst();

	cout << "\nOrders That Turned Into A Deal Between:\n " << temp1 << " And " << temp2 << ":" << endl;

	while (rset->next()) {
		cout << counter << ") Ordered for: " << rset->getString("first_name") << " " << rset->getString("last_name") << " At: " << rset->getString("order_date") << endl;
		++counter;
	}

	delete con;
	delete pstmt;
	delete rset;
}

//Option 15
void Database::totalDiscount() {

	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT first_name, last_name, SUM(ceiling(deal_sum * deal.discount)) AS total_discount FROM deal INNER JOIN customer WHERE "
		"deal_date >= ? AND deal.customer_id = ? AND customer.customer_id = deal.customer_id ;");


	cout << "Please Enter Customer id:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(2, temp);
	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter A Date:> ";
	getline(cin, temp);
	pstmt->setString(1, temp);

	rset = pstmt->executeQuery();
	rset->first();

	if (rset->first()) {
		cout << rset->getString("first_name") << " " << rset->getString("last_name") << " Has Received A Total Discount Of " << rset->getUInt("total_discount") << "$ Since " << temp << "." << endl;
	}
	else cout << "Invalid Date Or Customer id." << endl;

	delete con;
	delete pstmt;
	delete rset;

}

//Option 16
void Database::totalOfIncome() {

	string temp1, temp2;
	int quarter = 0, sum = 0;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT deal_sum, SUM(book.supplier_price) AS price FROM deal INNER JOIN deal_book INNER JOIN book WHERE deal_date BETWEEN ? AND ? "
		"AND deal.deal_num = deal_book.deal_num AND book.name = deal_book.book_name group by deal.deal_num;");

	cout << "Please Enter Year:> ";
	clearCin();
	getline(cin, temp1);
		



	temp2 = temp1 + "-01-01";
	pstmt->setString(1, temp2);
	temp2 = temp1 + "-03-30";
	pstmt->setString(2, temp2);
	rset = pstmt->executeQuery();
	while (rset->next()) {
		sum += rset->getUInt("deal_sum") - rset->getUInt("price");
	}

	cout << "The Total Sum Of Q1 is: " << sum << "$." << endl;

	temp2 = temp1 + "-04-01";
	pstmt->setString(1, temp2);
	temp2 = temp1 + "-06-30";
	pstmt->setString(2, temp2);
	rset = pstmt->executeQuery();
	while (rset->next()) {
		sum += rset->getUInt("deal_sum") - rset->getUInt("price");
	}

	cout << "The Total Sum Of Q2 is: " << sum << "$." << endl;

	temp2 = temp1 + "-07-01";
	pstmt->setString(1, temp2);
	temp2 = temp1 + "-09-30";
	pstmt->setString(2, temp2);
	rset = pstmt->executeQuery();
	while (rset->next()) {
		sum += rset->getUInt("deal_sum") - rset->getUInt("price");
	}

	cout << "The Total Sum Of Q3 is: " << sum << "$." << endl;

	temp2 = temp1 + "-10-01";
	pstmt->setString(1, temp2);
	temp2 = temp1 + "-12-30";
	pstmt->setString(2, temp2);
	rset = pstmt->executeQuery();
	while (rset->next()) {
		sum += rset->getUInt("deal_sum") - rset->getUInt("price");
	}

	cout << "The Total Sum Of Q4 is: " << sum << "$." << endl;

	delete con;
	delete pstmt;
	delete rset;

}

//Option 17
void Database::newCustomerSince() {

	string temp;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM customer WHERE join_date >= ?;");
	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Date:> ";
	clearCin();
	getline(cin, temp);
	pstmt->setString(1, temp);
	rset = pstmt->executeQuery();

	if (!rset->first()) {
		cout << "Invalid Date or No customers Found That Joined After The Date That Was Given." << endl;
		delete con;
		delete pstmt;
		delete rset;
		return;
	}

	rset->beforeFirst();
	int count = 1;

	while (rset->next()) {
		cout << count << ") " << rset->getString("first_name") << " " << rset->getString("last_name") << ",\tJoined At " << rset->getString("join_date") << "." << endl;
		++count;
	}

	delete con;
	delete pstmt;
	delete rset;
}

//Option 18
void Database::totalPurchasesSupplierbetween() {

	string temp1, temp2, supplier;
	int counter = 1, sum = 0;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT book_name, supplier_price FROM order_book INNER JOIN orders INNER JOIN book WHERE order_book.book_name = book.name "
		"AND orders.order_date BETWEEN ? AND ? AND orders.order_num = order_book.order_num AND orders.supplier_id = ?;");

	PreparedStatement *pstmt2 = con->prepareStatement("SELECT supplier_name FROM supplier WHERE supplier.supplier_id = ?;");

	cout << "Please Enter Supplier Number:> ";
	clearCin();
	getline(cin, supplier);
	pstmt->setString(3, supplier);
	pstmt2->setString(1, supplier);

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	getline(cin, temp1);
	pstmt->setString(1, temp1);

	cout << "Please Enter End Date:> ";
	getline(cin, temp2);
	pstmt->setString(2, temp2);

	rset = pstmt2->executeQuery();

	if (rset->rowsCount() == 0) {
		cout << "Invalid Supplier Number." << endl;
		return;
	}

	rset->first();
	supplier = rset->getString("supplier_name");

	rset = pstmt->executeQuery();
	if (rset->rowsCount() == 0) {
		cout << "Invalid Dates Or No Orders Where Found Between Given Dates." << endl;
		return;
	}

	cout << "\nTotal Purchases From " << supplier << " Between " << temp1 << " And " << temp2 << ":" << endl;

	rset->beforeFirst();

	while (rset->next()) {
		sum += rset->getUInt("supplier_price");
		cout << counter << ") " << rset->getString("book_name") << ", Costing " << rset->getString("supplier_price") << "$." << endl;
		++counter;
	}
	cout << "\nSub-Total: " << sum << "$." << endl;

	delete con;
	delete pstmt;
	delete pstmt2;
	delete rset;
}



//Option 19
void Database::totalSalesWorkerbetween() {

	string temp1, temp2;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT first_name, last_name, SUM(deal_sum) as sales FROM deal inner join worker where worker.worker_id = ? AND employee_id = ? AND deal.deal_date >= ? AND deal.deal_date <= ? AND is_canceled = false;");


	cout << "Please Enter Worker id:> ";
	clearCin();
	getline(cin, temp1);
	pstmt->setString(1, temp1);
	pstmt->setString(2, temp1);

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	getline(cin, temp1);
	pstmt->setString(3, temp1);

	cout << "Please Enter End Date:> ";
	getline(cin, temp2);
	pstmt->setString(4, temp2);

	rset = pstmt->executeQuery();
	rset->first();

	string first = rset->getString("first_name");
	string last = rset->getString("last_name");

	if (!(first == "") && !(last == "")) {
		cout << "The Total Sum Of Sales By " << first << " " << last << " Between " << temp1 << " And " << temp2 << " Is: " << rset->getUInt("sales") << "$." << endl;
	}
	else cout << "Invalid Dates Or Worker id." << endl;


	delete con;
	delete pstmt;
	delete rset;

}




//Option 20
void Database::top10SalesBooks() {

	string temp1, temp2;
	int counter = 1;
	Connection *con = driver->connect(connection_properties);
	con->setSchema(DB_NAME);
	ResultSet *rset;
	PreparedStatement *pstmt = con->prepareStatement("SELECT deal_book.book_name, COUNT(*) AS book_count FROM deal INNER JOIN deal_book WHERE deal.deal_num = deal_book.deal_num "
		"AND deal.deal_date >= ? AND deal.deal_date <= ? AND deal.is_canceled = false group by book_name ORDER BY book_count DESC;");

	cout << "\nPlease enter date in this pattern YYYY-MM-DD\n" << endl;
	cout << "Please Enter Start Date:> ";
	clearCin();
	getline(cin, temp1);
	pstmt->setString(1, temp1);

	cout << "Please Enter End Date:> ";
	getline(cin, temp2); 
	pstmt->setString(2, temp2);

	rset = pstmt->executeQuery();

	if (rset->rowsCount() == 0) {
		cout << "Invalid Dates Or No Deals Where Found Between Given Dates." << endl;
		return;
	}

	rset->beforeFirst();
	cout << "Top Books Sold Between " << temp1 << " And " << temp2 << " Are:" << endl;

	while (rset->next()) {
		cout << counter << ") " << rset->getString("book_name") << ", Selling " << rset->getString("book_count") << " Books." << endl;
		++counter;
	}


	delete con;
	delete pstmt;
	delete rset;
}



