# Memory-Register-Simulation
*** May be incomplete or not working properly ***

This is a small memory/register with cache simulation written in C++ for a class project.

Program was made using Visual Studio 2017.

Register and memory is implemented using vectors of integers.
Cache is implemented using a struct of a set of eight "blocks" that each contain two fields. 

A "Block" consists of:
 - Valid bit = Boolean value that signals whether "block" contains useful data
 - Tag = Four integer string used to identify block from word size address
 - LRU = Boolean value that signals whether "block" was recently used. (History bit)
 - Data = Bitset string of 32 bits that contain data being transfered

Store and load operations are located in "input.txt" file.
 - 100011 = Store contents from register to cache
 - 101011 = Load contents from cache to register
