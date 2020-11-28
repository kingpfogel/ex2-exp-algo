# ex2-exp-algo
Solutions to the first exercise sheet of experimental algorithms. (Group: L. Thyssen and N. Neesemann)

Hope this helps. Did not know how to get openmp to work on OS X. With cmake it works..
### How to work on OS X with CLion and CMake:
Prerequisites: make sure you have homebrew installed and it might be necessary to install OpenMP:
1. brew update
2. brew install llvm libomp

Then, get the git-repo and open one of the subprojects in cmake and build it:
1. git clone https://github.com/kingpfogel/ex2-exp-algo.git
2. Open one of the following subprojects in CLion and build the cmake project:
    1. ex2-hash-dynamic
    2. ex2-wordcount

### How to work on the gruenau remote machines:
1. git clone https://github.com/kingpfogel/ex2-exp-algo.git
2. choose one of the following:
    1. cd ex2-hash-dynamic
    2. cd ex2-wordcount
3. mkdir build-dir && cd build-dir
4. choose one of the following matching the selection above:
    1. meson --buildtype=debugoptimized ../develop/hashing && ninja
    2. meson --buildtype=debugoptimized ../develop/word-count && ninja
    
### Overview:
- tool: simexpal version 0.4
- run condition: must run on gruenau
- test environment: gruenau3 or gruenau4

### Common workflow:

- git clone git@github.com:kingpfogel/ex1-hash-basics.git
- git checkout -b PAK_your-feature-branch-name
- Change somethings on that branch and add and commit these changes
- git add --all / git add some-file.py
- git commit -m 'meaningful message'
- git push origin PAK_your-feature-branch-name
- Open a pull request on github.com
