#include <memory>
#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include <set>
#include <string>
#include <vector>
#include <iostream>

class user;
class review;
class business;
class hours;



#pragma db view
class StarCount{
public:
	int stars;
	int count;
};


#pragma db view query("select top 1 text, last_elapsed_time from sys.dm_exec_query_stats cross apply sys.dm_exec_sql_text(sql_handle) order by last_execution_time desc")
class LastQueryTime{
public:
	std::string text;
	long elapsed_time;
};

#pragma db object table("HOURS")
class hours{
public:
	#pragma db id column("ID") type("INT")
	int id;

	//14.4.7 default
	#pragma db default(null) column("HOURS") type("VARCHAR(255)")
	std::string hours;

	#pragma db value_not_null
	std::shared_ptr<business> business_id;
};

//The first is to find the opening hours of all restaurants a user has rated
//6.4 Lazy Pointers

#pragma db object table("BUSINESS")
class business{
public:
#pragma db id column("ID") type("VARCHAR(22)")//value_not_null?
std::string id;

#pragma db column("NAME") type("VARCHAR(255)")//value_not_null?
std::string name;
//can i specifie business_ twice????
#pragma db value_not_null inverse(business_id)
std::vector<odb::lazy_ptr<review>> reviews_;

#pragma db value_not_null inverse(business_id)
std::vector<odb::lazy_ptr<hours>> hours_;

};


#pragma db object table("REVIEW")
class review{
public:

std::string review_id_print() {
	return id;
}

#pragma db id column("ID") type("VARCHAR(22)")
std::string id;

#pragma db not_null
std::shared_ptr<user> user_id;

#pragma db not_null
std::shared_ptr<business> business_id;


};


#pragma db object table("USER")
class user{
public:

std::string user_id_print() {
		return id;
	}

std::vector<odb::lazy_ptr<review>> getVector()
{
	  return reviews_;
}

#pragma db id column("ID") type("VARCHAR(22)")
std::string id;

#pragma db column("NAME") type("VARCHAR(256)")
std::string name;

#pragma db value_not_null inverse(user_id)
std::vector<odb::lazy_ptr<review>> reviews_;

};
// ---------------------------------------------
// No need to change anything above this line
// ---------------------------------------------
