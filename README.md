# ACO
ACO is a lossless quality scores compression algorithm based on adaptive coding order and this software can be used for both encoding and decoding.
# Installing
ACO can be used on Windows,Linux or Mac. We only provide a source distribution and allow users to test or improve. In particular, ACO has no external dependencies so that it is easy to implement its function.\\

The distribution is configured out of the box for linux. To build on a Mac, change the setting parameters of the command line to work properly\\

There is currently no makefile for Windows. A visual studio project can be made (and testing has been done on windows to verify compatibility) but the steps are beyond the scope of this guide, because you must take several additional steps to guarantee a sane environment on windows (such as replacing stdint and inttypes with correct versions). For this reason we also currently do not distribute a windows build script.
# Algorithm
The main objective of ACO is to traverse the quality scores along the most relative directions, which can be regarded as a reorganization of the stack of independent 1D quality scores vectors into highly related 2D matrices. Another improvement of the proposed ACO technique over the existing techniques is the compound context modeling strategy. Instead of the adjacent QS values, the ACO context models consists of two additional aspects: 1) the global average of every reads; 2) the variant of DNA bases. The compound context model not only benefits the probability estimation and arithmetic coding, more importantly, in the implementation, it prevents ACO from multiple random access of the input FASTQ file:  the compressing process can be accomplished in only one-path, at the cost of some context dilution and side-information.

# Bugs and Feedback
Please use GitHub issues to open bug reports or provide feedback about ACO.

