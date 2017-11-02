# AdvancedDB17CW1

## short (one or two paragraphs) explanation of the performance before and after creating the columnstore index (filename explanation.txt).

The performance difference before and after creating the columnstore index is shocking, I am getting 68 percents of performance improvement on average, here is my result according to the virtual machine:

before indexing 

after indexing 


Here is my query:

Select sadasdas


According to the lecture, rowstore index is smililar to N-ary Storage Model (NSM), each attribute in a row is stored next to each other, however, in the columnar storage model, also known as The Decomposed Storage Model (DSM), the data is stored in a column based manner. NSM works best when sloving OLTP (On-line Transaction Processing) problems which invloves large number of short on-line transactions, for example INSERT, UPDATE and DELETE, and DSM in the other hand, works better when dealing with OLAP (On-line Analytical Processing) querys which usually involves more complex format and aggregations. In this case, as you can see, the query we are dealing with is calculating the sum of star with in a region, which mainly is an aggregation process, thus after we created a columnstored index for the specific attribute, in this case, Review.stars and Review.bussiness_id, the speed incresed dramatically.

The resaons of this efficient change is mainly due to RAM locality and the lantency of the disk, assuming the CPU have infinite computing power thus it is always waiting for Memeory, the data is loaded into the ram and the cost of locating the information next to each other is much cheaper than locating information thats further apart, in the column store index, all the information we need is stored next to each, thus caching data is much faster compares to the row stored manner. In disk, if we assume RAM is not large enough, and the all the data is stored in disk, different page on the disk needs to load into the ram before CPU can access them, disk is relatively slow hardware and the cost of swaping pages is very high compares to RAM and CPU speed, thus less swap need, higher the locality, faster the query will run, in this case, coloumn stored have more information we need packed to one page of disk, which result a much faster operation. But what if the CPU is not all perfect? CPU performance is also different with different tuple layouts, for DSM, usually as long as the tuple block fits into L2 cache, the sequential access is much faster than NSM, DSM also wins for random access inside L1 cache, which perfectly explains why the performance is better when we store the data in a column based manner.   
 
 


