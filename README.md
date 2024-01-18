### Producer Consumer Program
## How to Run Program
1. Compile the source code using `make all`
2. Create a file similar to inputexample that specifies a list of transactions where each line specifies either a transaction(e.g. T5) or sleep command (e.g. S3)
    - S3 is a sleep command that runs for 3 seconds
    - T5 is a transaction command that runs for 5 seconds
3. Run the program using the command:
    `./prodcon <number of threads> [<id>]`
    - Ex. `.prodcon 5` to run 5 threads in program 