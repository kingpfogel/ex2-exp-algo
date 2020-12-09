cmake-build-debug/./hashing --algo dynamic_scaling --max-fill 0.5 >> output.txt
cmake-build-debug/./hashing --algo dynamic_scaling --max-fill 0.8 >> output.txt
cmake-build-debug/./hashing --algo dynamic_scaling --max-fill 0.9 >> output.txt
cmake-build-debug/./hashing --algo dynamic_scaling --max-fill 0.95 >> output.txt
cmake-build-debug/./hashing --algo dynamic_scaling --max-fill 0.99 >> output.txt


cmake-build-debug/./hashing --algo dynamic --max-fill 0.5 >> output.txt
cmake-build-debug/./hashing --algo dynamic --max-fill 0.8 >> output.txt
cmake-build-debug/./hashing --algo dynamic --max-fill 0.9 >> output.txt
cmake-build-debug/./hashing --algo dynamic --max-fill 0.95 >> output.txt
cmake-build-debug/./hashing --algo dynamic --max-fill 0.99 >> output.txt

cmake-build-debug/./hashing --algo static --max-fill 0.99 >> output.txt
cmake-build-debug/./hashing --algo stl --max-fill 0.99 >> output.txt
cmake-build-debug/./hashing --algo static_scaling --max-fill 0.99 >> output.txt