declare -A model

# Constants
model["ANTIL"]="OMAN"
model["GUYANE"]="OMGU"
model["INDIEN"]="OMOI"
model["NCALED"]="OMNC"
model["POLYN"]="OMPF"


# Get parameters
grid=0.025
product=${model[$1]}
package=$2
date=$3
refTime=$4
step_start=$5
step_end=$6


destinationFolder="./grib/$date/AROME-OM/$1/$model/$refTime/$package"

# Retrieve api key from file "apikey"
apikey=$(<"./apikey")

# Old url to get grib file
# url="http://dcpc-nwp.meteo.fr/services/PS_GetCache_DCPCPreviNum?model=AROME-OM-INDIEN&grid=${grid}&package=${package}&time=${step}H&referencetime=${date}T${refTime}:00:00Z&format=grib2"

# Create folder if it does not exist
if [ ! -d "$destinationFolder" ]; then
    mkdir -p "$destinationFolder"
fi


for ((i=$step_start; i<=$step_end; i++))
do
    # Ensure that step form is 2 char long
    if [ "$i" -lt 10 ]; then
        step="0$i"
    else
        step="$i"
    fi

    url=$(cat <<FIN | tr -d '\n'
https://public-api.meteofrance.fr/previnum/
DPPaquetAROME-OM/
v1/models/AROME-OM-$1/
grids/${grid}/
packages/${package}/
product${product}?
referencetime=${date}T${refTime}:00:00Z
&time=0${step}H
&format=grib2
FIN
)

    echo $url
    curl -o "${destinationFolder}/${step}.grib2" -H "apikey: ${apikey}" "${url}"
done