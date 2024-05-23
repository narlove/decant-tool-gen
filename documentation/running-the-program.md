## Prerequisites
1. Docker
## Steps
1. Run the Docker daemon (I use the Docker desktop app).
2. Clone the program `git clone https://github.com/narlove/decant-tool-gen.git`
3. Move into the main folder `cd decant-tool-gen`
4. ~~Configure the `config.json` if need be. The default values should suffice.~~ The config file is not currently read from. Please use the constants defined in `main.cpp` instead
5. Build the image `docker build -t decant-tool-gen:latest .` This process may take some time.
6. Run the image `docker run decant-tool-gen:latest`
7. The program should run, taking in input from `target/keycodes.csv` and looking in the database, located at `target/pallets.csv`