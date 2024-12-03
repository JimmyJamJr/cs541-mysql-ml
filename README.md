# CS541-mysql-ml

Purdue University <br/>
CS 541 Database Systems <br/>
Dr. Walid Aref <br/>
MySQL-ML 1 <br/>
Jimson Huang, Gregg Puttkammer, Grant Parker
<br/>
<br/>
Class project in which we used MySQL's user-defined function (UDF) feature to incopearte machine learning predicates, including sentiment analysis and outliers detection. The ML predicates are implmeneted in a Python script and then incoperated into C++ for MySQL using the PyBind11 library.
<br/>

# Setup:
Modify the Makefile to have the correct include paths for Pybind, Python3, and MySQL. Then run make all.

# Example Usage:
```
select sentiment(column) from sentiment limit 10;
select outliers(value) from outliers;
```
