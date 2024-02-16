extractSingleFile <file_name> <folder_output> <longitude_min> <longitude_max> <latitude_min> <latitude_max>

extractSingleFile extract data from a single grib2 file for a certain coordinates range and write it in multiple json files.
For each shortname present in each message from grib2 file and json file will be created.
Each file will be named after his step value.

The output directory will look like this :
- json/  
  - <date>/
    - <folder_output>/
      - <hour>/
        - <level_type>/
          - <shortname>/
            - <json_file>

Example:
./extractSingleFile 2024-02-07/AROME-OM/INDIEN/00/SP1/01.grib2 REUNION 55.2 55.85 -21.40 -20.85

- json/  
  - 2024-02-07/
    - REUNION/
      - 00/
        - surface/
          - 2r/
            - 01.json
          - 2t/
            - 01.json
          - 10si/
            - 01.json
          - 10u/
            - 01.json
          - 10v/
            - 01.json
          - 10wdir/
            - 01.json
          - prmsl/
            - 01.json

The json file content will looks like:
{  
  "units":"%",  
  "forecast_date":"2024-02-05",  
  "forecast_time":"01",  
  "longitude":{"first":55.2, "last":55.85},  
  "latitude":{"first":-20.85, "last":-21.4},  
  "ni":26,  
  "res": 0.025,  
  "index": "round((lon - longitude.first) / res) + round((latitude.first - lat)  / res) * ni",  
  "grid": [values...]  
}

extractMultipleFiles will recursivly search grib2 files in a certain folder.
For each file founded, the program will do the same thing as extractSingleFile .
This program was made to extract weather data for Reunion Island because API on the web do not provide data precise as Meteo France data's for Reunion Island. 