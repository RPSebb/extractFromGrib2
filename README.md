Main application extract data from grib2 file to a json file.
./main path_to_grib_file output_folder longitude_min longitude_max latitude_min latitude_max

longitude and latitude are desired coordinate.

The output will be located to :
./json/date/output_folder/network_time/level_type/shortname/filename.json

date format is YYYY-MM-DD

network_time values are {00, 06, 12, 18}

level_type values are {"surface", "height", "isobaric"}

shortname values {"2t", "2r", "t", "ttc", ...}

filename = step value in message


So for each message in the grib file, a json will be created with a name equal to step and parent directory equal to the shortname.

Example:
./main 2024-02-07/AROME-OM/INDIEN/00/SP1/01.grib2 REUNION 55.2 55.85 -21.40 -20.85

- json/  
  - 2024-02-07/  
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


json file :  
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


This program was made to extract weather data for Reunion Island because API data on the web was not as precise as Meteo France data's (for the desired location).  
And I wanted only a few data from the file.
