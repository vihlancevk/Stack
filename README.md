# StructStack
Stack structure implemented on the basis of an array. 

The main feature of this project is its protection, which is implemented using: 
  
1) The canary method;
  
2) Hashing (using the HashRot13 function).  
  
This structure can work in two modes: RELEASE and DEBUG.  
In the second case, the maximum mode of protecting the glass from damage, and a file is also created *log.txt*, in which all debugging information will be output, while being as detailed as possible.
