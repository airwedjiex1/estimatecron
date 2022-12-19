# estimatecron
A system utility program which checks the correctness of a simplified crontab-file, and determines some simple metrics about the set of commands specified in the file

The estimatecron program checks the validity of the information in its input files (reporting any errors), simulates the execution of commands invoked during the requested month, and report the values of: 
- The name of the most frequently executed command (a single word) 
- The total number of commands invoked (a non-negative integer) 
- The maximum number of commands running at any time (a non-negative integer) 


# Invocation Example
./estimatecron  month  crontab-file  estimates-file 

Specifies:
- The month (of the current year) to be analysed (either an integer 0..11, or a 3-character name)
- The name of the two required text files (you may choose any names for the files)

# File Formats

![fileformats](https://user-images.githubusercontent.com/46706647/208359406-81c9e913-f5f1-4ffd-8eed-d12bfac077f6.PNG)
