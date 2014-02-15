reducto
=======

The goal of this project is to use linear algebra matrix operations to 
<a href="https://www.youtube.com/watch?v=vtu-7xvN7mw">shrink</a> large datasets. The specific 
<a href="http://www.cs.uakron.edu/~zduan/class/635/projects/project2/project2.pdf">requirements</a> consist of two 
major parts: image compression and dimensionality reduction. Image compression is accomplished by using singular 
value decomposition (SVD), and high-dimensional dataset dimensionality reduction is accomplished by using principal 
component analysis (PCA). 

### Part I: Image Compression

The most important operation used in this section is 
<a href="http://en.wikipedia.org/wiki/Singular_value_decomposition">singular value decomposition</a> (SVD). A 
singular value decomposition of an _m_ by _n_ matrix _M_ is a factorization of the form M=UΣV*, where
  * U is an _m_ by _m_ unitary matrix
  * Σ is an _m_ by _n_ rectangular diagonal matrix with nonnegative numbers on the diagonal, and
  * V* is the transpose of a unitary matrix V.


#### Uses

##### `reducto 1 image.pgm`

Saves the necessary information for the PGM image in ASCII PGM format using binary encoding, without preserving 
comments. 2 bytes are used to save the width of the image, 2 bytes are used to save the height are the image, 1 byte 
is used to save the grey scale, and 1 byte is used for the grey level of each pixel.

For example, if the image.pgm contains:

<p> <pre>
P2
#my image
4 5
255
234 23 12 345 22 11 22 11 22 1
0 1 2 3 4 5 6 7 8 9 10
</pre> </p>

then the file will be saved using 2 + 2 + 1 + (4 * 5) = 25 bytes. The comment is not preserved. The output is a 
binary file named image_b.pgm. 


##### `reducto 2 image_b.pgm`

Reverses the previous command. Converts a binary file, image_b.pgm, to an ASCII PGM file. The original comment is not
preserved, and the new comment is "# Back off! I will make you teensy!". The outupt file is saved as image2.pgm.


##### `reducto 3 header.txt SVD.txt k`

Saves the necessary information in the approximated image in a binary format. The input includes the header of the 
original image, three matrices U, Σ, V, and an integer k representing the rank of the approximation. The saved image 
is named image_b.pgsm.SVD, is recoverable, and can be viewed using any PGM viewer. The goal is to save storage.

The header.txt contains three integers (width, height, grey scale levels). The SVD.txt contains three matrices (U, Σ,
V). 


##### `reducto 4 image_b.pgm.SVD`

Reverses the previous command. Takes a PGM image that was compressed using SVD and outputs an uncompressed PGM 
image, named image_k.pgm, that can be viewed using any pgm viewer. 


### Part II: Dimensionality Reduction

The most important algorithm used in this section is 
<a href="http://en.wikipedia.org/wiki/Principal_component_analysis">principal component analysis</a> (PCA). It is a 
statistical procedure that uses orthogonal transformation to convert a set of observations of possibly correlated 
variables into a set of values of linearly uncorrelated variables called principal components. It's commonly used for
dimensionality reduction for visualization of high-dimensional datasets.


### Testing

##### `test storage`

Runs multiple test cases of image compression and decompression using the first two reducto commands and varying 
sizes of images, as well as the last two commands that use SVD with varying levels of rank approximation _k_ and 
relaxing the norm of the error matrix.

##### `test timing`

Runs multiple timing metrics using the aforementioned commands for image compression and decompression based on size 
of the original image as well as compression size.
