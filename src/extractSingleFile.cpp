#include <iostream>
#include <fstream>
#include <string>

#include <filesystem>
#include <sys/stat.h>

#include <eccodes.h>
#include <fmt/format.h>
#include <fmt/os.h>

#include <vector>
#include <unistd.h>
#include <libgen.h>
 

// From grib file
double lonFirst, lonLast, latFirst, latLast, iRes, jRes; long ni;

// From config file
double inLonFirst, inLonLast, inLatFirst, inLatLast;

// Calculated
int indexLonMin, indexLonMax, indexLatMin, indexLatMax, dataSize;

int NB_FILES = 0; size_t nbValues;
std::string folderName = "default";

// Functions declarations
std::string getLevelType(char(&typeOfLevel)[20], long &level);
bool areCoodinatesInRange();
void createDirectory(std::string path);
void extractMetadata(std::string path);
void extractData(std::string path);
float toMultipleOf(float value, float mutiple);
void writeFile(std::string path, double*& values, std::string validityDate, long validityTime, std::string units);
int getIndex(float lon, float lat);
int getIndexLon(float lon);
int getIndexLat(float lat);

int main(int argc, char* argv[]) {

    std::string filename;
    
    if(argc > 1) { filename = argv[1]; }
    else { printf("No file selected\n"); exit(1);  }
    if(argc > 2) { folderName = argv[2]; }
    else { printf("No folder name selected"); }

    extractMetadata(filename);

    if(argc > 6) {
        try{
            inLonFirst = std::stod(argv[3]);
            inLonLast  = std::stod(argv[4]);
            inLatFirst = std::stod(argv[6]);
            inLatLast  = std::stod(argv[5]);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        } 
    } else { std::cerr << "Missing coordinates" << std::endl; exit(1); }

    if( inLonFirst < lonFirst || inLonFirst > lonLast ||
        inLonLast  < lonFirst || inLonLast  > lonLast ||
        inLatFirst > latFirst || inLatFirst < latLast ||
        inLatLast  > latFirst || inLatLast  < latLast   ) { 
        
        std::cerr << "Coordinate out of range"<< std::endl;
        exit(0);
    }

    indexLonMin =  getIndexLon(inLonFirst);
    indexLonMax =  getIndexLon(inLonLast);
    indexLatMin =  getIndexLat(inLatFirst);
    indexLatMax =  getIndexLat(inLatLast);

    extractData(filename);

    return 0;
}

std::string getLevelType(char(&typeOfLevel)[20], long &level) {

    if(strcmp(typeOfLevel,           "surface") == 0) { return "surface"; } 
    if(strcmp(typeOfLevel,           "meanSea") == 0) { return "surface"; }
    if(strcmp(typeOfLevel, "heightAboveGround") == 0) { return (level < 20 ? "surface" : "height"); }
    if(strcmp(typeOfLevel,     "isobaricInhPa") == 0) { return "isobaric"; }
    return "unknown" ;
}

// Create a directory and return the status
void createDirectory(std::string path) {

    try {
        if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

}

std::string getExeDirectory() {

    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (len == -1) { return ""; }
    buffer[len]   = '\0';
    std::string dir = dirname(buffer);
    return dir + '/';
}

void extractMetadata(std::string filename) {
    FILE* file = fopen(filename.c_str(), "r"); int error;
    codes_handle* handle = codes_handle_new_from_file(0, file, PRODUCT_GRIB, &error);

    codes_get_double(handle, "longitudeOfFirstGridPointInDegrees", &lonFirst);
    codes_get_double(handle, "longitudeOfLastGridPointInDegrees" , &lonLast );
    codes_get_double(handle, "latitudeOfFirstGridPointInDegrees" , &latFirst);
    codes_get_double(handle, "latitudeOfLastGridPointInDegrees"  , &latLast );
    codes_get_double(handle, "iDirectionIncrementInDegrees"      , &iRes    );
    codes_get_double(handle, "jDirectionIncrementInDegrees"      , &jRes    );
    codes_get_size  (handle, "values"                            , &nbValues);
    codes_get_long  (handle, "Ni"                                , &ni      );
    codes_handle_delete(handle);
    fclose(file);
}

void extractData(std::string filename) {
    FILE* file = fopen(filename.c_str(), "r");
    codes_handle* handle; int error;
    size_t len;
    char shortName[20];
    char typeOfLevel[20];
    char validityDate[20];
    char units[20];
    long step, year, month, day, hour, level, validityTime;
    while((handle = codes_handle_new_from_file(0, file ,PRODUCT_GRIB, &error)) != NULL) {

        len = 20;
        codes_get_string(handle, "shortName"   , shortName   , &len);
        len = 20;
        codes_get_string(handle, "typeOfLevel" , typeOfLevel , &len);
        len = 20;
        codes_get_string(handle, "validityDate", validityDate, &len);
        len = 20;
        codes_get_string(handle, "units", units, &len);

        codes_get_long(handle, "step" , &step );
        codes_get_long(handle, "year" , &year );
        codes_get_long(handle, "month", &month);
        codes_get_long(handle, "day"  , &day  );
        codes_get_long(handle, "hour" , &hour );
        codes_get_long(handle, "level", &level);
        codes_get_long(handle, "validityTime" , &validityTime);

        std::string type = getLevelType(typeOfLevel, level);

        std::string date = fmt::format("{}-{:02d}-{:02d}", year, month, day);

        std::string path;

        if(type == "surface") {
            path = fmt::format("json/{}/{}/{:02d}/{}/{}", date, folderName, hour, type, shortName);
            createDirectory(path);
        } else {
            path = fmt::format("json/{}/{}/{:02d}/{}/{}/{}", date, folderName, hour, type, shortName, level);
            createDirectory(path);
        }

        size_t size = nbValues;
        double* values = new double[size];

        codes_get_double_array(handle, "values", values, &size);

        path = fmt::format("{}/{:02d}.json", path, step);
        writeFile(path, values, validityDate, validityTime, units);

        delete[] values;
        codes_handle_delete(handle);
    }

}

void writeFile(std::string path, double*& values, std::string validityDate, long validityTime, std::string units) {
    fmt::ostream f = fmt::output_file(path);

    f.print("{{\n");
    f.print("\t\"units\":\"{}\",\n", units);
    f.print("\t\"forecast_date\":\"{}-{}-{}\",\n", validityDate.substr(0, 4), validityDate.substr(4, 2), validityDate.substr(6, 2));
    f.print("\t\"forecast_time\":\"{:02d}\",\n", validityTime / 100);
    f.print("\t\"{}\":{{\"first\":{}, \"last\":{}}},\n", "longitude", inLonFirst, inLonLast);
    f.print("\t\"{}\":{{\"first\":{}, \"last\":{}}},\n", "latitude", inLatFirst, inLatLast);
    f.print("\t\"ni\":{},\n", indexLonMax - indexLonMin);
    f.print("\t\"res\": {}, \n", iRes);
    f.print("\t\"index\": \"{} + {} * ni\",\n", 
        "round((desired_longitude - longitude.first) / res)",
        "round((latitude.first - desired_latitude)  / res)");
    f.print("\t\"grid\": [");

    for(int y = indexLatMin; y <= indexLatMax; y++) {
        for(int x = indexLonMin; x <= indexLonMax; x++) {
            int i = y * ni + x; // Position in grid
            f.print("{:.2f}", values[i]);
            if(!(y == indexLatMax & x == indexLonMax)) {
                f.print(",");
            }
        }
    }

    f.print("]\n");
    f.print("}}");

    f.close();
}

float toMultipleOf(float value, float multiple) {
    return round(value / multiple) * multiple;
}

int getIndex(float lon, float lat) {
    int iLon = getIndexLon(lon);
    int iLat = getIndexLat(lat);
    return iLat * ni + iLon;
}

int getIndexLon(float lon) {
    return round((lon - lonFirst) / iRes);
}

int getIndexLat(float lat) {
    return round((latFirst - lat) / iRes);
}