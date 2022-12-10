# SLIC

## Introduction

The SLIC algorithm was developped by researchers at EPFL in 2010.
It constrasted with the other competing State-of-the-Art superpixels segmentation algorithms because it was the fastest at the time and was easy to use, ie. it did not require many parameters to obtain a good segmentation.
In fact, the _Simple Linear Iterative Clustering_ algorithm runs in `O(N)` time, compared to `O(NlogN)` or even `O(N^2)` for other methods.

## Guide

### Requirements
* `make`
* `gcc`

### How to use

1. Clone the repository
2. Place desired `.ppm` image in the `src` directory and modify `NAME_IMG_IN` in `main.c`.
3. Build using make
```
$ cd slic/src && make
```
4. Run the executable
```
$ ./main
```

## Results

Below are some of the results obtained after running the SLIC aglorithm, based on `n`, the number of superpixels.
| Initial Image  | `n=256` | `n=512`  | `n=1024`  |
|---|---|---|---|
| ![Initial Image](/src/results/lady/lady.png)  | ![Segmented Image](/src/results/lady/lady_segmented_256.png)  | ![Segmented Image](/src/results/lady/lady_segmented_512.png)   | ![Segmented Image](/src/results/lady/lady_segmented_1024.png)     |
| ![Initial Image](/src/results/church/church.png)  | ![Segmented Image](/src/results/church/church_segmented_256.png)  | ![Segmented Image](/src/results/church/church_segmented_512.png)   |  ![Segmented Image](/src/results/church/church_segmented_1024.png)  |
|   |   |   |   |


## Contribution

Contributions are more than welcome, simply clone the repository and open a PR.

## Disclaimer

This code is provided _as is_ and may contain bugs, or not accurately transcribe the algorithm given in the original paper it is based on.