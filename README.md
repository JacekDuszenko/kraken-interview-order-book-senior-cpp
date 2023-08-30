# Recruitment test for kraken C++ senior engineer. 

Read this README, implement something similar and you automatically land a second stage interview. No need to thank me.

## Task
Implement an order book according to the spec.

## What is an order book?
[This](https://www.investopedia.com/terms/o/order-book.asp)

## Algorithms & Data Structures
A very good and not demanding (in terms of implementation) solution is to use two priority queues, one for sell orders and the other forbuy orders. In C++ this is easily done by using a set. Sets in C++ are ordered and internally implemented as red black trees. This givesyou very nice properties - insertion is O(logn), removal of an arbitrary element is O(logn). You will use insertion while implementing adding an order to the book, deletion while cancelling an active order. WHy not use a regular priority queue? [That's why](https://stackoverflow.com/a/10142015)

Buy order queue is a max heap, storing the orders with the biggest price  at the top, sell order queue is a min heap, storing the lowestpriced ones at the top. You can easily implement a market order by using the same queues, just set their price to 0 in case of sell order and to MAX_INT in case of buy order and adjust your logic.

There is a O(1) insertion solution but it's not worth implementing as it is essentially a LRU cache reimplementation (doubly linked list with a hashmap) where you have to rely on pointers arithmetic a lot which is tricky to get without making a stupid one-off error. Solution described above is enough to pass the interview.

Tags: order book, kraken, crypto, exchange, order, execution, trade,crypto

How to run the server and test client.

# Build the server
1. Build the Docker image `docker build -t krakentask .`
2. Run the container and exec into it `docker run -it krakentask /bin/bash`
3. cd into `/home` directory in the running container.
4. Run `cmake .` to generate build files.
5. Run `cmake --build .` to build the binary and tests.

# Run the server and the client inside the container
1. In a home directory in a shell inside the running container, run `./krakentask` to start the server on port 1234.
2. Open a new terminal and exec into the container to get a second shell. Firstly check the hash of a running docker image by running `docker ps`.
Then exec into the container like this`docker exec -it [CONTAINER_HASH] /bin/bash`. Go to the /home directory.
2. Run `python3.8 client.py inputFile.csv localhost 1234` to parse and send data contained in `inputFile.csv` into the server.

You should see output data being printed in the first shell.

# Run unit tests
To start unit tests, run `./unit_tests` binary that's located inside the `test` directory. From `/home` directory you can run `./test/unit_tests`.

