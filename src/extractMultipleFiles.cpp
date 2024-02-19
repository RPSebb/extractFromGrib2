#include <fstream>
#include <eccodes.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <vector>
#include <utils.hpp>
#include <map>

// Map of errors message
std::map<int, std::string> errors {
   {1, "Not enough parameters"},
   {2, "Input coordinates cannot be converted to double."},
   {3, "Input folder does not exist."},
   {4, "Input folder is not a folder."},
   {5, "Input coordinates out of bounds."}
};

struct Range { double first, last; };
struct CoordinatesRange { Range longitude, latitude; };

struct GribData {
    // Constants
    CoordinatesRange coordinates;
    struct { double i, j; } resolution;
    struct { long   i, j; }          n;
    char dataDate[20];
    char dataTime[20];
    size_t gridSize;

    // Message
    char        units[20];
    char    shortName[20];
    char  typeOfLevel[20];
    char validityDate[20];
    char validityTime[20];
    long      step, level;
    std::vector<double>   grid;
};

struct InputData {
    CoordinatesRange coordinates;
    std::string gribFolder;
    std::string outputFolder;
};

int getIndex(float longitude, float latitude, CoordinatesRange coords, int ni, int res) {
    int iLon = round((longitude - coords.longitude.first) / res);
    int iLat = round((latitude  -  coords.latitude.first) / res);
    return iLat * ni + iLon;
}

std::string convertDate(std::string date) {
    return fmt::format("{}-{}-{}", date.substr(0, 4), date.substr(4, 2), date.substr(6, 2));
}

std::string convertTime(std::string time) {
    return fmt::format("{}:{}", time.substr(0, 2), time.substr(2, 2));
}

// Get user input data
void getInputData(InputData& data, int argc, char* argv[]) {
    if(argc > 1)  { data.gribFolder = argv[1]; }
    else          { data.gribFolder = "grib";  }

    if(argc <= 6) { throw 1; }

    try {
        data.coordinates.longitude.first = std::stod(argv[3]);
        data.coordinates.longitude.last  = std::stod(argv[4]);
        data.coordinates.latitude.first  = std::stod(argv[6]);
        data.coordinates.latitude.last   = std::stod(argv[5]);
    } catch (std::exception& e) { throw 2; }
}

// Extract unchaged data from grib file
void getConstantData(codes_handle* handle, GribData& data) {
    size_t len;
    len = 20; codes_get_string(handle, "dataDate", data.dataDate, &len);
    len = 20; codes_get_string(handle, "dataTime", data.dataTime, &len);
    codes_get_double(handle, "longitudeOfFirstGridPointInDegrees", &data.coordinates.longitude.first);
    codes_get_double(handle, "longitudeOfLastGridPointInDegrees" , &data.coordinates.longitude.last );
    codes_get_double(handle, "latitudeOfFirstGridPointInDegrees" , &data.coordinates.latitude.first );
    codes_get_double(handle, "latitudeOfLastGridPointInDegrees"  , &data.coordinates.latitude.last  );
    codes_get_double(handle, "iDirectionIncrementInDegrees"      , &data.resolution.i   );
    codes_get_double(handle, "jDirectionIncrementInDegrees"      , &data.resolution.j   );
    codes_get_size  (handle, "values"                            , &data.gridSize       );
    codes_get_long  (handle, "Ni"                                , &data.n.i            );
    codes_get_long  (handle, "Nj"                                , &data.n.j            );
}

// Extract data from grib message
void getMessageData(codes_handle* handle, GribData& data) {
    size_t len;
    len = 20; codes_get_string(handle, "shortName"       , data.shortName   , &len);
    len = 20; codes_get_string(handle, "typeOfLevel"     , data.typeOfLevel , &len);
    len = 20; codes_get_string(handle, "validityDate"    , data.validityDate, &len);
    len = 20; codes_get_string(handle, "validityTime"    , data.validityTime, &len);
    len = 20; codes_get_string(handle, "units"           , data.units       , &len);
    codes_get_long(handle, "step" , &data.step );
    codes_get_long(handle, "level", &data.level);
}

// Extract data with a certain coordinates
void getGridData(codes_handle* handle, GribData& grib, InputData& input) {
    size_t size = grib.gridSize;
    double* values = new double[size];
    codes_get_double_array(handle, "values", values, &size);

    int indexLonMin =  round((input.coordinates.longitude.first - grib.coordinates.longitude.first) / grib.resolution.i);
    int indexLonMax =  round((input.coordinates.longitude.last  - grib.coordinates.longitude.first) / grib.resolution.i);
    int indexLatMin =  round((grib.coordinates.latitude.first   - input.coordinates.latitude.first) / grib.resolution.i);
    int indexLatMax =  round((grib.coordinates.latitude.first   - input.coordinates.latitude.last ) / grib.resolution.i);

    for(int y = indexLatMin; y <= indexLatMax; y++) {
        for(int x = indexLonMin; x <= indexLonMax; x++) {
            int i = y * grib.n.i + x; // Position in grid
            grib.grid.push_back(values[i]);
        }
    }

    delete[] values;
}

bool isInRange(double value, double min, double max) {
    return (value >= min && value <= max);
}

// latitude first is the greatest number in theses grib2 file
// idk if this is a universel convention 
bool areCoordinatesRangeWithinBounds(CoordinatesRange& coordsA, CoordinatesRange& coordsB) {
    return  isInRange(coordsA.longitude.first, coordsB.longitude.first, coordsB.longitude.last) &&
            isInRange(coordsA.longitude.last , coordsB.longitude.first, coordsB.longitude.last) &&
            isInRange(coordsA.latitude.first , coordsB.latitude.last  , coordsB.latitude.first) &&
            isInRange(coordsA.latitude.last  , coordsB.latitude.last  , coordsB.latitude.first); 
}

void writeFile(std::string path, GribData& grib, InputData& input) {
    fmt::ostream f = fmt::output_file(path);
    f.print("{{\n");
    f.print("\"units\":\"{}\",\n", grib.units);
    f.print("\"forecast_date\":\"{}\",\n", convertDate(grib.validityDate));
    f.print("\"forecast_time\":\"{}\",\n", convertTime(grib.validityTime));
    f.print("\"longitude\":{{\"first\":{}, \"last\":{}}},\n", input.coordinates.longitude.first, input.coordinates.longitude.last);
    f.print("\"latitude\" :{{\"first\":{}, \"last\":{}}},\n",  input.coordinates.latitude.first,  input.coordinates.latitude.last);
    f.print("\"res\": {}, \n", grib.resolution.i);
    f.print("\t\"index formula\": \"{} + {} * ni\",\n", 
    "round((desired_longitude - longitude.first) / res)",
    "round((latitude.first - desired_latitude)  / res)");
    f.print("\"grid\":{::.2f}", grib.grid);
    f.print("\n}}");
    f.close();
}

std::string getLevelType(char* typeOfLevel, int level) {
    if(strcmp(typeOfLevel,           "surface") == 0) { return "surface"; } 
    if(strcmp(typeOfLevel,           "meanSea") == 0) { return "surface"; }
    if(strcmp(typeOfLevel, "heightAboveGround") == 0) { return (level < 20 ? "surface" : "height"); }
    if(strcmp(typeOfLevel,     "isobaricInhPa") == 0) { return "isobaric"; }
    return "unknown" ;
}

// Return folder location containing the json file
std::string getDestinationPath(GribData& grib, InputData& input) {
    std::string type = getLevelType(grib.typeOfLevel, grib.level);
    std::string path = fmt::format("json/{}/{}/{}/{}/{}", convertDate(grib.dataDate), input.outputFolder, grib.dataTime, type, grib.shortName);
    if(type != "surface") { path += "/" + grib.level; }
    return path;
}


void iterateMessages(std::string filename, InputData& inputData) {
    FILE* file = fopen(filename.c_str(), "r");
    codes_handle* handle; 
    int error;
    int count = 0;
    GribData gribData;

    while((handle = codes_handle_new_from_file(0, file ,PRODUCT_GRIB, &error)) != NULL) {
        if(count == 0) { 
            getConstantData(handle, gribData);
            if(!areCoordinatesRangeWithinBounds(inputData.coordinates, gribData.coordinates)) {
                codes_handle_delete(handle);
                throw 5;
            }
        }

        gribData.grid.resize(0);
        getMessageData(handle, gribData);
        getGridData(handle, gribData, inputData);

        std::string path = getDestinationPath(gribData, inputData);
        createDirectory(path);
        
        std::string filename = fmt::format("{}/{:02d}.json", path, gribData.step);
        writeFile(filename, gribData, inputData);

        codes_handle_delete(handle);
    }

    fclose(file);
}


int main(int argc, char* argv[]) {

    struct InputData inputData;

    try {
        getInputData(inputData, argc, argv);

        if(!std::filesystem::exists(inputData.gribFolder))       { throw 3; }
        if(!std::filesystem::is_directory(inputData.gribFolder)) { throw 4; }

        std::vector<std::string> filenames;
        getFilesRecursive(inputData.gribFolder, filenames);

        for(int i = 0; i < filenames.size(); i++) {
            iterateMessages(filenames[i], inputData);
        }

    } catch(int code) {
        std::cerr << errors[code] << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    
    return 0;
}