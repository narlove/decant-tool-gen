## Prerequisites
1. NodeJS
2. NPM (Node Package Manager)
## Steps
1. Clone the program `git clone https://github.com/narlove/decant-tool-gen.git`
2. Edit the `config.json` file if anything needs to be changed.
3. Navigate into the api folder `cd api`
4. Initialise node, to install dependencies `npm i`
5. Configure the IPv4 address.
	5a. Create the `process.env` file `touch process.env`
    5b. Open the `process.env` file with a file reader of your choice
    5c. Input `IPV4_ADDRESS=0.0.0.0`, replacing `0.0.0.0` with your IPv4 address.
6. (Do not navigate out of the api folder) Run `node .`
7. You should see `listening on port xxxx` appear in the terminal
8. To stop running the API, press `ctrl+c` on your keyboard