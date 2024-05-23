## Prerequisites
1. NodeJS
2. NPM (Node Package Manager)
## Steps
1. Clone the program `git clone https://github.com/narlove/decant-tool-gen.git`
2. Edit the `config.json` file if anything needs to be changed.
3. Navigate into the api folder `cd api`
4. Initialise node, to install dependencies `npm i`
5. Configure the IPv4 address. First, create the `process.env` file using `touch process.env`.
6. Open the `process.env` file with a file reader of your choice
7. Input `IPV4_ADDRESS=0.0.0.0`, replacing `0.0.0.0` with your IPv4 address.
8. (Do not navigate out of the api folder) Run `node .`
9. You should see `listening on port xxxx` appear in the terminal
10. To stop running the API, press `ctrl+c` on your keyboard
