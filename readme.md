# Archiver

I realised archiver, which can can compress any number of files into one. 
Also, my program can decompress one archive into original files.

## Algorithm

I used [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) to compress and decompress my files. Also, 
my algorithm haven't loss of data during compression. I was reading char from files, encoded this char by binary 
sequence in [trie](https://en.wikipedia.org/wiki/Trie). Then every encoded char is vertex. Moreover, no one terminate 
vertex isn't parent of other terminate vertex. Because of it we can unambiguously decode out data. Using this trie 
we create [Canonical Huffman code](https://en.wikipedia.org/wiki/Canonical_Huffman_code). Also, we encode special
symbols:

* `FILENAME_END = 256`
* `ONE_MORE_FILE = 257`
* `ARCHIVE_END = 258`

They help us to separate different files and data from filename, which encodes too. Length of our char is 9 bits.
Also, we put into archive information about alphabet and lengths of encoded chars. Using this information, we can
retrieve binary sequence for every char. After we read a data and decode it.

## Program build

After copying of data into local directory we need write in terminal following:

* `cmake PROJECT_ROOT`
* `make`

Now we can use our executable file `PROJECT_NAME`. If you want to run your program then write in terminal following:

* `./PROJECT_NAME`

## Program usage

You can launch this program with three different flags:

1. `./PROJECT_NAME -c ARCHIVE_NAME INPUT_FILE_NAME_1 INPUT_FILE_NAME_2 ...`
2. `./PROJECT_NAME -d ARCHIVE_NAME`
3. `./PROJECT_NAME -h`

First variant compress your files `INPUT_FILE_NAME_1`, `INPUT_FILE_NAME_2` ... into `ARCHIVE_NAME`. if file `ARCHIVE_NAME` 
already exists then this file will be overwritten. `ARCHIVE_NAME` is necessary parameter. Also, you should write at least
one `INPUT_FILE_NAME` parameter.

Second variant decompress `ARCHIVE_NAME` into files with their original names. If these files already exists then
they will be overwritten. `ARCHIVE_NAME` is necessary parameter.

Third variant writes into standard output stream usage help.

## Benchmarks

| Filename | Input file size | Output file size | Ratio | Compression time | Decompression time | Compression speed | Decompression speed |
| --- | --- | --- | --- | --- | --- | --- | --- |
| document.docx | 13,85 KB | 13,04 KB | 1,0621 | 0 min 0,055 sec | 0 min 0,176 sec | 251,85 KB/sec | 74,1 KB/sec |
| music.mp3 | 7,15 MB | 7,11 MB | 1,0045 | 0 min 17,815 sec | 1 min 29,054 sec | 410,76 KB/sec | 81,8 KB/sec |
| photo.jpg | 379,3 KB | 369,66 KB | 1,0261 | 0 min 0,963 sec | 0 min 4,34 sec | 393,88 KB/sec | 85,18 KB/sec |
| video.mkv | 653,92 MB | 382,55 MB |  1,7094 | 25 min 32,402 sec | 89 min 29,937 sec | 436,97 KB/sec | 72,95 KB/sec |
| bigdocument.doc | 4,07 MB | 2,42 MB | 1,6796 | 0 min 9,603 sec | 0 min 30,321 sec | 433,83 KB/sec | 81,81 KB/sec |

We can see that decompression speed is roughly equal, but compression speed increases due to grow of input data. Also, big 
natural text and video is files with the best coefficient of compression.
