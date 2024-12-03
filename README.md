# CS541-mysql-ml

Purdue University <br/>
CS 541 Database Systems <br/>
Dr. Walid Aref <br/>
MySQL-ML 1 <br/>
Jimson Huang, Gregg Puttkammer, Grant Parker
<br/>
<br/>
Class project in which we used MySQL's user-defined function (UDF) feature to incorporate machine learning predicates, including sentiment analysis and outlier detection. The ML predicates are implemeneted in a Python script and then incorporated into C++ for MySQL using the PyBind11 library.
<br/>

# Requirements:
Python 3.10 or higher <br/>
MySQL 8.0 or higher <br/>

# Setup:
Install required python modules: <br/>
```pip install -r requirements.txt``` <br/>
Add the MySQL directory to PATH: <br/>
```export PATH=${PATH}:/usr/local/mysql/bin``` <br/>
Compile using Makefile: <br/>
```sudo make all```

# Example Usage:
```
select sentiment(column) from sentiment limit 10;
select outliers(value) from outliers;
```
