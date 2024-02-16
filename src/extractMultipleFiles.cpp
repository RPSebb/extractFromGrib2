#include <fstream>
#include <eccodes.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <vector>
#include <utils.hpp>

struct FileData {
    struct { double first, last; }  longitude;
    struct { double first, last; }   latitude;
    struct { double     i,    j; } resolution;
    struct { long       i,    j; }          n;
    std::string date, time;
    size_t gridSize;
};

struct MessageData {
    char shortName[20], typeOfLevel[20], units[20];
    std::string date, time;
    long step;
    double* values;
};

struct InputData {
    std::string gribFolder;
    std::string outputFolder;
    struct { double first, last; } longitude;
    struct { double first, last; }  latitude;
};

std::vector<std::string> filenames;

void getInputData(InputData& data, int argc, char* argv[]) {
    if(argc > 1)  { data.gribFolder = argv[1]; }
    else          { data.gribFolder = "grib";  }

    if(argc <= 6) { throw std::runtime_error("Coordinates are missing or incomplet."); }

    try {
        data.longitude.first = std::stod(argv[3]);
        data.longitude.last  = std::stod(argv[4]);
        data.latitude.first  = std::stod(argv[6]);
        data.latitude.last   = std::stod(argv[5]);
    } catch (std::exception& e) {
        throw std::runtime_error("Input coordinates cannot be converted to double.");
    }
};

void getFileData(codes_handle* handle, FileData& fileData) {
    size_t len; char date[20], time[20];

    len = 20; codes_get_string(handle, "dataDate", date, &len);
    len = 20; codes_get_string(handle, "dataTime", time, &len);

    codes_get_double(handle, "longitudeOfFirstGridPointInDegrees", &fileData.longitude.first);
    codes_get_double(handle, "longitudeOfLastGridPointInDegrees" , &fileData.longitude.last );
    codes_get_double(handle, "latitudeOfFirstGridPointInDegrees" , &fileData.latitude.first );
    codes_get_double(handle, "latitudeOfLastGridPointInDegrees"  , &fileData.latitude.last  );
    codes_get_double(handle, "iDirectionIncrementInDegrees"      , &fileData.resolution.i   );
    codes_get_double(handle, "jDirectionIncrementInDegrees"      , &fileData.resolution.j   );
    codes_get_size  (handle, "values"                            , &fileData.gridSize       );
    codes_get_long  (handle, "Ni"                                , &fileData.n.i            );
    codes_get_long  (handle, "Nj"                                , &fileData.n.j            );

    std::string _date = date;
    std::string _time = time;
    fileData.date =  fmt::format("{}-{}-{}", 
                    _date.substr(0, 4),
                    _date.substr(4, 2), 
                    _date.substr(6, 2));
    fileData.time = fmt::format("{}:{}", _time.substr(0, 2), _time.substr(2, 2));
}

void getMessageData(codes_handle* handle, MessageData& messageData) {
    size_t len; char date[20];
    len = 20; codes_get_string(handle, "shortName"       , messageData.shortName   , &len);
    len = 20; codes_get_string(handle, "typeOfLevel"     , messageData.typeOfLevel , &len);
    len = 20; codes_get_string(handle, "validityDateTime", date                    , &len);
    len = 20; codes_get_string(handle, "units"           , messageData.units       , &len);
    
    codes_get_long(handle, "step", &messageData.step);

    std::string _date = date;
    messageData.date =  fmt::format("{}-{}-{}", 
                        _date.substr(0, 4),
                        _date.substr(4, 2), 
                        _date.substr(6, 2));
    messageData.time = fmt::format("{}:{}", _date.substr(11, 2), _date.substr(13, 2));
}

bool isInRange(double value, double min, double max) {
    return (value >= min && value <= max);
}

bool areInputCoordinatesInRange(InputData& inputData, FileData& fileData) {
    return  isInRange(inputData.longitude.first, fileData.longitude.first, fileData.longitude.last) &&
            isInRange(inputData.longitude.last , fileData.longitude.first, fileData.longitude.last) &&
            isInRange(inputData.latitude.first , fileData.latitude.last  , fileData.latitude.first) &&
            isInRange(inputData.latitude.last  , fileData.latitude.last  , fileData.latitude.first); 
}

void printFileData(FileData& data) {
    fmt::print("\n - - - - - - - File data - - - - - - - \n");
    fmt::print("longitude      : {{{}, {}}}\n", data.longitude.first, data.longitude.last);
    fmt::print("latitude       : {{{}, {}}}\n", data.latitude.first , data.latitude.last );
    fmt::print("resolution     : {{{}, {}}}\n", data.resolution.i   , data.resolution.j  );
    fmt::print("n              : {{{}, {}}}\n", data.n.i            , data.n.j           );
    fmt::print("date           : {}\n", data.date    );
    fmt::print("time           : {}\n", data.time    );
    fmt::print("grid size      : {}\n", data.gridSize);
}

void printMessageDate(MessageData& data) {
    fmt::print("\n - - - - - - - Message data - - - - - - - \n");
    fmt::print("shortName      : {}\n", data.shortName  );
    fmt::print("type of level  : {}\n", data.typeOfLevel);
    fmt::print("units          : {}\n", data.units      );
    fmt::print("date           : {}\n", data.date       );
    fmt::print("time           : {}\n", data.time       );
    fmt::print("step           : {}\n", data.step       );
}

void printInputData(InputData& data) {
    fmt::print("\n - - - - - - - Input data - - - - - - - \n");
    fmt::print("longitude      : {{{}, {}}}\n", data.longitude.first, data.longitude.last);
    fmt::print("latitude       : {{{}, {}}}\n", data.latitude.first , data.latitude.last );
    fmt::print("input  folder  : {}\n"        , data.gribFolder  );
    fmt::print("output folder  : {}\n"        , data.outputFolder);
}

void iterateMessages(std::string filename, InputData& inputData) {
    FILE* file = fopen(filename.c_str(), "r");
    codes_handle* handle; 
    int error;
    int count = 0;
    FileData fileData;

    while((handle = codes_handle_new_from_file(0, file ,PRODUCT_GRIB, &error)) != NULL) {
        if(count == 0) { 
            getFileData(handle, fileData);
            // printFileData(fileData);
            if(!areInputCoordinatesInRange(inputData, fileData)) {
                std::cout << "Coordinate out of range" << std::endl;
                codes_handle_delete(handle);
                break;
            }
        }

        MessageData messageData;
        getMessageData(handle, messageData);

        if(type == "surface") {
            path = fmt::format("json/{}/{}/{:02d}/{}/{}", date, folderName, hour, type, shortName);
            createDirectory(path);
        } else {
            path = fmt::format("json/{}/{}/{:02d}/{}/{}/{}", date, folderName, hour, type, shortName, level);
            createDirectory(path);
        }
        
        printFileData(fileData);
        printMessageDate(messageData);

        codes_handle_delete(handle);
    }

    fclose(file);
}

int main(int argc, char* argv[]) {

    // std::vector<float> a = {1,2,3,4,5};
    // fmt::print("values : {}\n", a);
    
    struct FileData fileData;
    struct InputData inputData;

    
    try {
        getInputData(inputData, argc, argv);

        if( !std::filesystem::exists(inputData.gribFolder) || 
            !std::filesystem::is_directory(inputData.gribFolder)) 
        {
            throw std::runtime_error(inputData.gribFolder + " is not a folder or do not exist.");
        }

        // printInputData(inputData);

        getFilesRecursive(inputData.gribFolder, filenames);
        iterateMessages(filenames[12], inputData);

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    
    return 0;
}