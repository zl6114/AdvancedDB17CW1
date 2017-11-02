#include "Yelp-odb.hxx"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <odb/database.hxx>
#include <odb/mssql/database.hxx>
#include <odb/transaction.hxx>

using std::cout;
using std::endl;
using odb::mssql::database;
using odb::transaction;
using odb::query;
using odb::result;
using std::to_string;
using odb::lazy_shared_ptr;
using odb::lazy_ptr;
using std::weak_ptr;
using std::shared_ptr;

std::vector<std::string> findHours(odb::database& db, std::string username) {
	std::vector<std::string> result;
	std::vector<lazy_ptr<review>>::iterator i;

	transaction t(db.begin());
	// t.tracer(odb::stderr_tracer);
	//std::cout << "The Answer for user " << username << "   is:"  << '\n';
	auto names = db.query <user> (odb::query<user>::name == username);
	for (auto & User : names){
		//find all user have the that username
		for(auto & Review : User.reviews_){
			//get all reviews that commented
			auto Business = (Review.load() -> business_id);
			//get the business_id of each review
			for(auto & Hours : (Business -> hours_)){
				//find the hours
					//std::cout << (Hours.load() -> hours) << '\n';
			 		result.push_back((Hours.load() -> hours));
			}
		}
	}

	//std::cout << '\n';

	t.commit();
	return result;
}

std::vector<StarCount> countStars(odb::database& db, float latMin, float latMax,
																	float longMin, float longMax)
{

	std::stringstream Query;
	// std::cout << '\n';
	// std::cout << "Here the query of counting the star is : " << '\n';
		// Count the stars
	Query << "SELECT REVIEW.STARS AS stars,\nCOUNT(REVIEW.STARS) AS count\n"
				<< "FROM   REVIEW INNER JOIN BUSINESS "
				<< "ON REVIEW.business_id = Business.id\n"
				<< "WHERE  BUSINESS.LATITUDE >= " << latMin << "\n"
				<< "AND    BUSINESS.LATITUDE <= " << latMax << "\n"
				<< "AND    BUSINESS.LONGITUDE >= " << longMin << "\n"
				<< "AND    BUSINESS.LONGITUDE <= " << longMax << "\n"
				<< "GROUP BY REVIEW.STARS \n"
				<< "ORDER BY stars ASC";

	odb::result<StarCount> R;
	std::vector<StarCount> result;
	transaction t(db.begin());
	//t.tracer(odb::stderr_tracer);
	R = db.query<StarCount>(Query.str());
	for (auto i (R.begin ()); i != R.end (); ++i)
  {
		StarCount temp;
		// for testing use
		// std::cout << "\tStar:  " << i -> stars
		// 					<< "\tCount:  " << i -> count <<  '\n';
		temp.stars = i -> stars;
		temp.count = i -> count;
		result.push_back(temp);
  }
	t.commit();
	return result;
}

void createIndex(odb::database& db){

	// --SQL Server 2012 and SQL Server 2014: you need to drop the nonclustered indexes
	// --in order to create the columnstore index.

	//std::cout << "Creating a columnstore index to accelerate the query :" << '\n';
	transaction t(db.begin());
	//t.tracer(odb::stderr_tracer);
	db.execute ("CREATE COLUMNSTORE INDEX STARS_C ON REVIEW (STARS, BUSINESS_ID)");
	t.commit();

	// create a columnstore index to accelerate your query
}

void dropIndex(odb::database& db){

	//std::cout << "Now drop the columnstore index thats created : " << '\n';
	transaction t(db.begin());
	//t.tracer(odb::stderr_tracer);
	db.execute ("DROP INDEX STARS_C ON REVIEW");
	t.commit();

}

// FROM
// ON
// JOIN
// WHERE
// GROUP BY
// WITH CUBE or WITH ROLLUP
// HAVING
// SELECT
// DISTINCT
// ORDER BY
// TOP

// ---------------------------------------------
// No need to change anything below this line
// ---------------------------------------------

LastQueryTime getLastQueryRuntime(odb::database& db) {
	transaction t(db.begin());
	auto time = db.query_one<LastQueryTime>();
	t.commit();
	return *time;
}

std::vector<std::string> hoursFixtureKn{
		"Friday|0:00-0:00",			 "Friday|0:00-0:00",			"Friday|11:00-1:00",
		"Friday|17:00-1:00",		 "Friday|17:00-1:00",			"Friday|17:30-23:00",
		"Friday|18:00-23:00",		 "Friday|8:00-2:00",			"Friday|9:00-2:00",
		"Monday|0:00-0:00",			 "Monday|0:00-0:00",			"Monday|11:00-23:00",
		"Monday|17:30-23:00",		 "Monday|18:00-23:00",		"Monday|8:00-2:00",
		"Monday|9:00-2:00",			 "Saturday|0:00-0:00",		"Saturday|0:00-0:00",
		"Saturday|11:00-1:00",	 "Saturday|11:30-23:00",	"Saturday|17:00-1:00",
		"Saturday|17:00-1:00",	 "Saturday|18:00-23:00",	"Saturday|8:00-4:00",
		"Saturday|9:00-2:00",		 "Sunday|0:00-0:00",			"Sunday|0:00-0:00",
		"Sunday|11:00-23:00",		 "Sunday|17:00-0:00",			"Sunday|17:00-0:00",
		"Sunday|17:30-23:00",		 "Sunday|18:00-23:00",		"Sunday|8:00-2:00",
		"Sunday|9:00-2:00",			 "Thursday|0:00-0:00",		"Thursday|0:00-0:00",
		"Thursday|11:00-23:00",	"Thursday|17:00-0:00",		"Thursday|17:00-0:00",
		"Thursday|17:30-23:00",	"Thursday|18:00-23:00",	"Thursday|8:00-2:00",
		"Thursday|9:00-2:00",		 "Tuesday|0:00-0:00",			"Tuesday|0:00-0:00",
		"Tuesday|11:00-23:00",	 "Tuesday|17:00-0:00",		"Tuesday|17:00-0:00",
		"Tuesday|17:30-23:00",	 "Tuesday|18:00-23:00",		"Tuesday|8:00-2:00",
		"Tuesday|9:00-2:00",		 "Wednesday|0:00-0:00",		"Wednesday|0:00-0:00",
		"Wednesday|11:00-23:00", "Wednesday|17:00-0:00",	"Wednesday|17:00-0:00",
		"Wednesday|17:30-23:00", "Wednesday|18:00-23:00", "Wednesday|8:00-2:00",
		"Wednesday|9:00-2:00"};

std::vector<std::string> hoursFixtureNeu{
		"Friday|11:30-2:00",		 "Friday|6:00-20:00",		 "Friday|9:00-19:00",		 "Monday|11:30-23:00",
		"Monday|6:00-20:00",		 "Monday|9:00-19:00",		 "Saturday|12:00-20:00", "Saturday|12:00-2:00",
		"Saturday|8:00-16:00",	 "Sunday|12:00-23:00",	 "Thursday|11:30-23:00", "Thursday|6:00-20:00",
		"Thursday|9:00-19:00",	 "Tuesday|11:30-23:00",	"Tuesday|6:00-20:00",	 "Tuesday|9:00-19:00",
		"Wednesday|11:30-23:00", "Wednesday|6:00-20:00", "Wednesday|9:00-19:00"};

std::vector<std::string> hoursFixtureEwr{"Friday|20:30-22:00", "Saturday|20:30-22:00",
																				 "Sunday|20:30-22:00", "Thursday|20:30-22:00"};

std::vector<StarCount> starFixture1{
		{1, 137039}, {2, 111817}, {3, 174317}, {4, 337639}, {5, 410518}};

std::vector<StarCount> starFixture2{{1, 28781}, {2, 19532}, {3, 27541}, {4, 56435}, {5, 83655}};

bool operator==(StarCount const& left, StarCount const& right) {
	return left.stars == right.stars && left.count == right.count;
};

int main(int argc, char** argv) {

	using namespace std;
	database db("SA", "AdvancedDB17", "yelp", "localhost");

	{ // testing find Hours
		auto hours = findHours(db, "kn");
		std::sort(hours.begin(), hours.end());
		cout << "findHours Test 1: " << (hours == hoursFixtureKn ? "passed" : "failed") << endl;

		hours = findHours(db, "neu");
		std::sort(hours.begin(), hours.end());
		cout << "findHours Test 2: " << (hours == hoursFixtureNeu ? "passed" : "failed") << endl;

		hours = findHours(db, "Ewr");
		std::sort(hours.begin(), hours.end());
		cout << "findHours Test 3: " << (hours == hoursFixtureEwr ? "passed" : "failed") << endl;
	}

	{ // testing countStars
		auto stars = countStars(db, 30.0, 45.7, -100.0, -1.0);
		std::sort(stars.begin(), stars.end(),
							[](auto left, auto right) { return left.stars < right.stars; });
		cout << "countStars Test 1: " << (stars == starFixture1 ? "passed" : "failed") << endl;

		stars = countStars(db, 4.0, 40., -90.0, -40.0);
		std::sort(stars.begin(), stars.end(),
							[](auto left, auto right) { return left.stars < right.stars; });
		cout << "countStars Test 2: " << (stars == starFixture2 ? "passed" : "failed") << endl;
	}

	{ // performance runs

		// warmup run
		countStars(db, 30.0, 45.7, -100.0, -1.0);
		for(size_t i = 0; i < 5; i++) {
			countStars(db, 30.0, 45.7, -100.0, -1.0);
			cout << "Run number " << i
					 << " time before indexing: " << getLastQueryRuntime(db).elapsed_time << endl;
		}

		createIndex(db);

		// warmup run
		countStars(db, 30.0, 45.7, -100.0, -1.0);
		for(size_t i = 0; i < 5; i++) {
			countStars(db, 30.0, 45.7, -100.0, -1.0);
			cout << " Run number " << i << " time after indexing: " << getLastQueryRuntime(db).elapsed_time
					 << endl;
		}
		cout << endl;

		dropIndex(db);
	}
	return 0;
}
