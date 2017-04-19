# IDcard Recognization
the project contains 4 modules:
* rectangle deskew -> dir deskew
* textline detection -> dir detect/ 
* textline recognization -> dir textline/
* classify engine(needed by textline recognization) -> dir deep_engine/
* a interface to call -> dir idcardrec/

note: Detection is not included here, see CTPN

## build
mkdir build && cd build && cmake .. && make


