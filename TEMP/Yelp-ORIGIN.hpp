#include <memory>
#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include <set>
#include <string>
#include <vector>
#include <iostream>


class review;
class hours;
class business;
class user;

typedef std::vector<odb::lazy_weak_ptr<review>> reviews;
typedef ::user user_type;

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

//The first is to find the opening hours of all restaurants a user has rated
//6.4 Lazy Pointers

#pragma db object
class hours{
public:

#pragma db id
int id;

//14.4.7 default
#pragma db default(null)
std::string hours;

#pragma db value_not_null
std::vector<odb::lazy_shared_ptr<business>> business_id;

};

#pragma db object
class business{
public:
#pragma db id //value_not_null?
std::string id;
//can i specifie business_ twice????
#pragma db value_not_null inverse(business_id)
std::vector<odb::lazy_weak_ptr<review>> reviews_;

#pragma db value_not_null inverse(business_id)
std::vector<odb::lazy_weak_ptr<hours>> hours_;


};


#pragma db object table("REVIEW")
class review{

private:
friend class odb::access;

#pragma db id
std::string id;

#pragma db not_null
odb::lazy_shared_ptr<business> business_id;

#pragma db not_null
odb::lazy_shared_ptr<user_type> user_id;


};

// #pragma db object table("USER")
// class User {
//   public:
//     User () {}
//
//     #pragma db id column("ID") type("VARCHAR(22)")
//     std::string id;
//     ...

#pragma db object table("USER")
class user{
public:
typedef ::reviews reviews_type;
const reviews_type&reviews () const
{
     return reviews_;
}
reviews_type&reviews ()
{
    return reviews_;
}

private:
friend class odb::access;
#pragma db id column("ID") type("VARCHAR(22)")
std::string id;

#pragma db value_not_null inverse(user_id)
reviews_type reviews_;

};
// ---------------------------------------------
// No need to change anything above this line
// ---------------------------------------------
