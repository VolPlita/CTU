# Salary

<i>Code in solution is shit, but it works</i>

You'll given an ID of the region, that is like a zipcode, and a salary around that location. For example 0 could be Prague and 01 could be Prague 1 and so on. But it doesn't matter. (Format is specified on the test data)

After taking the region and the salary of it, you'll receive a query. That query starts with ? and has the either the * meaning all region(everything) or just a ID. For example if the input is

+ 00 1000

+ 0 100

+ 000 1000

then the query is 0, then 00, 0, 000 will be in but not 100.

Your task is to retreive the most frequent(or repeated) salary around encompassing region. For 0 then based on the previous example the most frequent salary would be 1000 [2].

In case if the 2 distinct salary has the same frequency, for example, 1500 [3] and 2000 [3], then print both of them. Mode: 1500, 2000 [3]

Also in the query, if the given query is not existing in the given dataset of region and salary, print N/A

Remember: ID is not a integer. 0100 != 100

- You return invalid input if

<b>1</b> after querying has started, it shouldn't add more region and salary
<b>2</b> input doesn't start with + or ?
<b>3</b> region contains something not digit
<b>4</b> salary is not an integer

<b>Image from test:</b>
![salary picture](../../../images/sal1.jpg)
![salary picture](../../../images/sal2)
