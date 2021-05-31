# How to install ?

If you use a Debian-based Linux distribution, simply run the following commands:

```
sudo apt-get update && sudo apt-get install git libcurl4-openssl-dev
git clone https://github.com/thomasarmel/uqload_downloader
cd uqload_downloader/cli
sudo apt-get update && sudo apt-get install libcurl4-openssl-dev
./configure
make
sudo make install
uqload_downloader https://uqload.com/embed-xxxxxxx.html movie.mp4
```
