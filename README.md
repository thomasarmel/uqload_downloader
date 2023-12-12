# uqload_downloader

Download videos from uqload

Have you ever tried to download a video from uqload ? You check the source code of the page, you discover an url like "
https:// m30.uqload. com /[...]/v.mp4" and you try to open it in a new tab. An surprise: __error_wrong_ip__ :(

So this software permits to skirt this inconvenience.

Simply lauch __uqload_downloader.exe https:// uqload. com/embed-[...].html yourmovie.mp4__ and voila

---

Linux users can run [./cli/download_from_kodi_log.sh](./cli/download_from_kodi_log.sh) (Linux Mint tested)
to install uqload_downloader and detects uqload from KODI/VStream logs and ask for copying it to $HOME/astroport

---

C# version: https://github.com/gameplayoffert/UqloadDownloader

--

# UQload Downloader Instructions to Use online! :)

This guide provides step-by-step instructions on how to use the UQload Downloader.

## Steps

1. **Mount your Google Drive**: This allows you to access files from your Google Drive.
    ```python
    from google.colab import drive
    drive.mount('/content/drive')
    ```

2. **Change the current directory to your Google Drive**:
    ```python
    %cd /content/drive/MyDrive/
    ```
    > **Note**: You may want to change to a different directory to avoid cluttering your Drive's home directory with the files for UQload Downloader.

3. **Clone the 'uqload_downloader' repository from GitHub** and then **change the current directory to 'uqload_downloader/cli'**:
    ```python
    !git clone https://github.com/thomasarmel/uqload_downloader
    %cd uqload_downloader/cli
    ```

4. **Update the package lists for upgrades and new package installations** and then **install the 'git' and 'libcurl4-openssl-dev' packages**:
    ```python
    !apt-get update
    !apt-get install -y git libcurl4-openssl-dev
    ```

5. **Make the 'configure' file executable** and then **run the 'configure' script**:
    ```python
    !chmod +x configure
    !./configure
    ```

6. **Compile the program**:
    ```python
    !make
    ```

7. **Install the program**:
    ```python
    !make install
    ```

8. **Use the 'uqload_downloader' command to download a movie from 'uqload.io'**:
    ```python
    !uqload_downloader https://uqload.io/embed-XXXXX.html NameOfMovie.mp4
    ```
    Replace `'https://uqload.io/embed-XXXXX.html'` with the actual URL of the movie you want to download and `'NameOfMovie.mp4'` with the name you want to give to the downloaded movie file.

Please replace the placeholders with your actual data where necessary.

