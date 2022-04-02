# AutoDNS-Query
> automatic DNS query program running on Linux

This program takes the string in the ".txt" file and uses '\n' as the key to send DNS queries one line at a time, and writes the corresponding ip to the file.

## Installing

Only Linux:

```sh
git clone https://github.com/Wonlf/AutoDNS-Query
```

## Running the tests

On Linux:

```sh
##input.txt Modify. you want

gcc main.c -o main.out
./main.out input.txt output.txt 3 1
```
| FirstOption    | SecondOption    | ThirdOption                                             | OptionFour            |
|----------------|-----------------|---------------------------------------------------------|-----------------------|
| input.txt path | output.txt path | n when truncated from the total string of the file to n | timeout period (.sec) |

## Update history

* 1.0
    * Read the file, receive a response from the DNS query in the domain, and write it to the file.
* 2.0
    * Accept the path of the file as an argument.
    * Adding dns query timeout function
    * Implementation of a function that shows the total execution time of a program
    * Add a function that allows the maximum number of strings to be truncated by the user. 
      * (ex. truncated by 20 if the total length of a string is 100.)

## Information

Wonlf – [wonlf.tistory.com](https://wonlf.tistory.com/) – qqwow741@gmail.com

For a detailed explanation, Please refer to [BLOG](https://wonlf.tistory.com/entry/ver20-C%EC%96%B8%EC%96%B4-DNS%EC%BF%BC%EB%A6%AC-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%A8-%EB%A7%8C%EB%93%A4%EA%B8%B0-gethostbyname-on-linux)