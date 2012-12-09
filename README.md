na_containers
=============

o vector-like container with support for missing (N/A) values

o brand new array2d implementation:
  - packed storage
  - column- or row-major completely transparent
  - with row and column slices 
  - iterable rows and columns
  - specialization for strided and unstrided iterators for efficiency
  - transparent access to major element iteration in order for efficient
    iterations and copy operations