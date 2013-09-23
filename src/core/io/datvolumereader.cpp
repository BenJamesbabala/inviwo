#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

namespace inviwo {


DatVolumeReader::DatVolumeReader(const std::string filePath) 
    : VolumeReader()
    , sourceFileAbsolutePath_(filePath)
{}

Data* DatVolumeReader::readData() {    
    return readRawVolumeData();
}

void DatVolumeReader::readDatFileSettings(std::string filePath, ReaderSettings& readerSettings)  {
    readerSettings.rawFileAbsolutePath_ = "";
    readerSettings.dataFormat_ = "";
    readerSettings.dimensions_ = ivec3(0,0,0);

    if(!URLParser::fileExists(filePath)){
        filePath = URLParser::addBasePath(filePath);
        if(!URLParser::fileExists(filePath)) {
            // FIXME: We need to throw an exception here (or return false)
            LogInfoCustom("DatVolumeReader::readDatFileSettings", "File " + filePath + " does not exist.");     
            return;
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);

    std::string fileExtension = URLParser::getFileExtension(filePath);

    ivwAssert(fileExtension=="dat", "should be a *.dat file");

    if (fileExtension=="dat") {

        //Read the dat file content
        std::istream* f = new std::ifstream(filePath.c_str());
        std::string textLine; 

        std::string key;
        while (!f->eof()) {
            getline(*f, textLine);
            std::stringstream ss(textLine);
            transform(textLine.begin(), textLine.end(), textLine.begin(), (int (*)(int))tolower);
            key = "";
            ss >> key;            
            if (key=="ObjectFileName:") {
                ss >> readerSettings.rawFileAbsolutePath_;
                readerSettings.rawFileAbsolutePath_ = fileDirectory + readerSettings.rawFileAbsolutePath_;
            }
            else if (key=="Resolution:") {
                ss >> readerSettings.dimensions_.x;
                ss >> readerSettings.dimensions_.y;
                ss >> readerSettings.dimensions_.z;
            }
            else if (key=="Format:") {
                ss >> readerSettings.dataFormat_;

                if (readerSettings.dataFormat_=="UCHAR") {
                    readerSettings.dataFormat_ = DataUINT8::str();
                }
                else if (readerSettings.dataFormat_=="USHORT") {
                    readerSettings.dataFormat_ = DataUINT16::str();
                }
                else
                    readerSettings.dataFormat_="";
            }                    
        };
    }       
}

Data* DatVolumeReader::readRawVolumeData() {

    ReaderSettings readerSettings;

    //Get reader settings
    DatVolumeReader::readDatFileSettings(sourceFileAbsolutePath_, readerSettings);    

    //read raw data
    void* rawData = RawVolumeReader::loadRawData(readerSettings);

    ivwAssert(rawData!=0, "Unable to read raw file data");        

    //Allocate volume handle
    Volume* data = new StandardVolume();
    DataRepresentation* dataRepresentation = 0;

    //Allocate representation
    if (readerSettings.dataFormat_ == "") {
        delete data;
        data = 0;
        return data;    
    }
    #define DataFormatIdMacro(i) else if (readerSettings.dataFormat_ == "##i") { dataRepresentation = new VolumeRAM_##i(static_cast<Data##i::type*>(rawData), readerSettings.dimensions_); }
    #include <inviwo/core/util/formatsdefinefunc.h>
    else {
        delete data;
        data = 0;
        return data;
    }

    //add representation to handle
    data->addRepresentation(dataRepresentation);
    return data;
}




} // namespace
