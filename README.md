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


![fformats2](https://user-images.githubusercontent.com/46706647/208359652-38b983ab-01ef-4389-b798-92cee9ccd4fd.PNG)

# Authors
Samuel Kent and Henri Scaffidi
