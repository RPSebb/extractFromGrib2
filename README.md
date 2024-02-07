Main application extract data from grib2 file to a json file.
./main path_to_grib_file output_folder longitude_min longitude_max latitude_min latitude_max

longitude and latitude are desired coordinate.

The output will be located to :
./json/date/output_folder/reseau/level_type/shortname/filename.json

date format is YYYY-MM-DD
reseau values are {00, 06, 12, 18}
level_type values are {"surface", "height", "isobaric"}
shortname values {"2t", "2r", "t", "ttc", ...}
filename = step value in message

So for each message in the grib file, a file will be created with a name equal to step.

This program was made to extract weather data for Reunion Island because API data on the web was not as precise as Meteo France data's (for the desired location).
And I wanted only a few data from the file.
