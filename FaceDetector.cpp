#include "FaceDetector.hpp"
/* declaration for function in giflib_to_opencv.cpp to convert
   gif images to a image file that can be used in opencv */

int FaceDetector::loadCascade(){
    if (!faceHaarCascade_.load(HAAR_CASCADE_PATH)){
        std::cerr << "ERROR: No se pudo cargar el Algoritmo Haar Cascade" << HAAR_CASCADE_PATH << std::endl;
        return -1; // failure
    }
    return 0; // success
}

int FaceDetector::loadImage(std::string imagePath){
    imagePath_ = imagePath;
    std::string fileExtension;
    if (imagePath.find_last_of(".") != std::string::npos){
        fileExtension = imagePath.substr(imagePath.find_last_of(".")+1);
        for(unsigned int i = 0; i < fileExtension.length(); ++i)
            fileExtension[i] = tolower(fileExtension[i]);
    }

    if ((fileExtension == "jpg") || (fileExtension == "jpeg")){ //Elije la funcion basada en la extension
        image_ = cv::imread(imagePath);}
    else if (fileExtension == "png"){
        image_ = cv::imread(imagePath);}
    else{
        std::cerr << "ERROR: extension No soportada " << imagePath << std::endl;
        return -1;}
    if (image_.empty()){ // Check si la img fue cargada
        std::cerr << "ERROR: Imagen NO cargada " << imagePath << std::endl;
        return -2;}
    return 0; // success!
}

int FaceDetector::detectFaces(){
    if (image_.empty()){
        std::cerr << "ERROR: Cannot detect faces; image_ not loaded" << std::endl;
        return -1; // failure
    }
    cv::Mat imageGrayScale;// convierte la img a escala de grises
    cv::cvtColor(image_, imageGrayScale, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(imageGrayScale, imageGrayScale);

    // use haar cascade to detect faces
    faceHaarCascade_.detectMultiScale(imageGrayScale, faces_, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(250, 250));
    return faces_.size(); // number of faces detected
}

void FaceDetector::displayImage(){
    if (image_.empty()){
        std::cerr << "ERROR: no se puede mostrar la img / No cargada" << std::endl;
        return; // failure
    }

    for (int i = 0; i < faces_.size(); i++){
		cv::Point center(faces_[i].x + faces_[i].width / 2, faces_[i].y + faces_[i].height / 2);
		cv::ellipse(image_, center, cv::Size(faces_[i].width / 2, faces_[i].height / 2), 0, 0, 360, cv::Scalar(0, 255, 0), 10, 8, 0);
	}

    shrinkImage(1280, 720);// scale img to fit within 1280x720

	cv::namedWindow("Face Detector", cv::WINDOW_AUTOSIZE);  //crea una ventana para mostar img
	cv::imshow("Face Detector", image_);
	cv::waitKey(0);
}
void FaceDetector::saveJSON(){
    if (imagePath_.empty()){
        std::cerr << "ERROR: No puede guardar en JSON; Path no valido" << std::endl;
        return; // failure
    }

    Json::Value result;
	 Json::StreamWriterBuilder builder;
	 std::ofstream outFile;

    // Rellenar los campos del Json
    result["countFaces"] = faces_.size();
    char absolutePath [PATH_MAX + 1];
    result["imageLocation"] = realpath(imagePath_.c_str(), absolutePath);

    // crea la salida del json en el mismo dir. que la img
    std::string outFileName = imagePath_ + ".json";
	 outFile.open(outFileName);
	 builder.newStreamWriter()->write(result, &outFile);
	 outFile.close();
    
    std::cout << "Guardado " << outFileName << std::endl;
}

void FaceDetector::shrinkImage(int maxWidth, int maxHeight){
    if (image_.empty()){
        std::cerr << "ERROR: No se puede escalar la img" << std::endl;
        return; // failure
    }

    // scale image_ to fit within maxWidth x maxHeight
    cv::Size s = image_.size();
    if ((s.width/s.height) > (maxWidth/maxHeight)) // scale based on width
    {
        cv::resize(image_, image_,
                   cv::Size(maxWidth, (s.height * maxWidth / s.width)));
    }
    else // scale based on height
    {
        cv::resize(image_, image_,
                   cv::Size((s.width * maxHeight / s.height), maxHeight));
    }
}