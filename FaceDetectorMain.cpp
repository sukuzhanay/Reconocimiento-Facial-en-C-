#include <iostream>
#include "FaceDetector.hpp"
using namespace std;
int main(int argc, char* argv[]){
	if (argc <= 1){
		cerr << "Argumentos incompletos path incorrecto" << endl;
		return 1;
	}
    FaceDetector fd;
    fd.loadCascade();
    if (int ret = fd.loadImage(argv[1])) return ret;
    cout << endl << "Cargada imagen " << argv[1] << endl;
    int faceCount = fd.detectFaces();
    cout << endl << "Reconocidas " << faceCount << " caras" << endl << endl;
    fd.saveJSON();
    cout << endl;
    fd.displayImage();
    return 0;
}