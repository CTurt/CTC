CTC
===
CTurt Compression is a lossless compression algorithm designed for text data.

It is my first attempt at compression, and is based on an article about Huffman compression.

Written entirely from scratch, in C.

### What kind of data compresses best?
Anything which uses a small subset of all possible 256 characters, and which also has characters that are much more frequent than others.

Plain text, is a good example of this because there are usually just the basic ASCII characters, and not the less common symbols. Distribution of characters is also not even, for example in the English language, e is much more frequently used than z.

Most binary data will typically increase in file size after compression since it usually uses most of the possible 256 characters, and has a more even distribution of characters than plain text.

### How does it work?

Firstly, the data is analysed to create a sorted list of the most common to the least common characters.

Each character is assigned a bit code, which is shorter if the character is more frequently used:

    Bit code  |  Index in the frequency sorted list
    00        |  0
    01        |  1
    100       |  2
    101       |  3
    1100      |  4
    1101      |  5
    11100     |  6
    11101     |  7
    111100    |  8
    111101    |  9
    1111100   |  10
    ...

For example, if the most frequently used character is e, then it will be assigned the bit code 00.

Before the sorted list is written, each byte in the list is set to the difference between it, and the previous byte; if a lot of characters are unused, there will be long sections of bytes containing just 1.

RLE compression is then applied to all of the 1s in the difference encoded list, usually making it smaller.

A header containing the length of the content, and the length of the RLE compressed difference list is written. Then the RLE compressed difference list itself is written.

Finally, every byte's bit code is looked up, and written to the output.

### Results
This README.md file is reduced by about 20 percent when compressed.

A 100 by 100 blank bitmap is reduced by about 70 percent when compressed, and will reduce even more if compressed again.