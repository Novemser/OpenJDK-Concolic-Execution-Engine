# How do we implement a concolic execution engine?

We do not modify the original logic of openjdk.

In order to extend openjdk for concolic execution, following steps should be done:

* Define the exactly code point where concolic execution starts and ends
* Make variables symbolic
* Track the data and control flow of each symolic values
