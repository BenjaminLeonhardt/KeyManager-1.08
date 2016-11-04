sudo apt-get install libncurses5-dev libncursesw5-dev
sudo apt-get install xclip
g++ KeyManager.cpp AES.cpp sha256.cpp -lncurses -o KeyManager.out -std=c++11