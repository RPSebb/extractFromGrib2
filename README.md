## ExtractSingleFile
#### Parameters:

```<file_name> <output_folder> <longitude_min> <longitude_max> <latitude_min> <latitude_max>```

#### Description:

This program extract data from a single grib2 file for a certain coordinates range and write it in multiple json files.<br>
For each shortname present in each message from the grib2 file a json file will be created.<br>
Each file will be named after his step value.<br>

#### Example:

<figure>
  <div style="border-radius: 4px 4px 0px 0px; background-color: #444; padding: 5px 10px;">Bash</div>

  ```Shell
  ./extractSingleFile 2024-02-07/AROME-OM/INDIEN/00/SP1/01.grib2 REUNION 55.2 55.85 -21.40 -20.85 
  ./extractSingleFile 2024-02-07/AROME-OM/INDIEN/00/SP1/03.grib2 REUNION 55.2 55.85 -21.40 -20.85
  ./extractSingleFile 2024-02-07/AROME-OM/INDIEN/00/SP1/07.grib2 REUNION 55.2 55.85 -21.40 -20.85
  ```

  <figcaption><i>Call three times the program with differents files.</i></figcaption>
</figure></br></br> 

<figure>
  <div style="border-radius: 4px 4px 0px 0px; background-color: #444; padding: 5px 10px;">Directory Tree</div>

  ```
  json/
  └── 2024-02-07/
      └── REUNION/
          └── 00/
              └── surface/
                  ├── 2r/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ├── 2t/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ├── 10efg/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ├── 10fg/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ├── 10nfg/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ├── 10si/
                  |   ├── 01.json
                  |   ├── 03.json
                  |   └── 07.json
                  ...        
  ```
  <figcaption><i>Directory tree result after program call.</i></figcaption>
</figure></br></br>

<figure>
  <div style="border-radius: 4px 4px 0px 0px; background-color: #444; padding: 5px 10px;">Json</div>

  ```JSON
  {  
    "units":"%",  
    "forecast_date":"2024-02-05",  
    "forecast_time":"01",  
    "longitude":{"first":55.2, "last":55.85},  
    "latitude":{"first":-20.85, "last":-21.4},  
    "ni":26,  
    "res": 0.025,  
    "index": "round((lon - longitude.first) / res) + round((latitude.first - lat)  / res) * ni",  
    "grid": [1,2,3]
  }
  ```
  <figcaption><i>Json file content.</i></figcaption>
</figure></br></br>

## ExtractMultipleFiles UNFINISHED !
#### Parameters:

```<grib_folder> <output_folder> <longitude_min> <longitude_max> <latitude_min> <latitude_max>```

#### Description:

This program will recursivly search grib2 files in a certain folder.<br>
For each file founded, it will extract data for a certain coordinates range and write it in multiple json files.<br>
For each shortname present in each message from the grib2 file a json file will be created.<br>
Each file will be named after his step value.<br>

This program was made to extract weather data for Reunion Island because API on the web do not provide data precise as Meteo France data's for Reunion Island.<br>