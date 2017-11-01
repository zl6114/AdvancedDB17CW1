# AdvancedDB17CW1

## short (one or two paragraphs) explanation of the performance before and after creating the columnstore index (filename explanation.txt).

Convert a rowstore table to a clustered columnstore index or create a nonclustered columnstore index. Use a columnstore index to efficiently run real-time operational analytics on an OLTP workload or to improve data compression and query performance for data warehousing workloads.


The performance difference before and after creating the columnstore index is shocking, I am getting 68 percents of performance improvement on average, here is my result according to the virtual machine:

before indexing 

after indexing 


Here is my query:

Select sadasdas
asd
as
das
asd
sa

According to the lecture, rowstore index is N-ary Storage Model (NSM), each attribute in a row is stored next to each other, however, in the columnar storage model, also known as The Decomposed Storage Model (DSM), the data is stored in a column based manner. NSM works best when sloving OLTP (On-line Transaction Processing) problems which invloves large number of short on-line transactions, for example INSERT, UPDATE and DELETE, and DSM in the other hand, works better when dealing with OLAP (On-line Analytical Processing) querys which usually involves more complex format and aggregations. In this case, as you can see, the query we are dealing with is calculating the sum of star in a region, which mainly is an aggregation process. 
 
 


