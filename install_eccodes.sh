version="2.34.0"
code="45757960"
filename="eccodes-$version-Source"
archive="$filename.tar.gz"
url="https://confluence.ecmwf.int/download/attachments/$code/$archive"
buildFolder="build"
libraryFolder="eccodes"
threads=$(nproc)

# If library already exist, ask permision to continue
if [ -d "$libraryFolder" ]; then
    echo "$libraryFolder already exist."
    echo "Existing files will be deleted."
    echo "Continue installation ? (y|)"
    read answer

    # Delete library on yes answer
    if [ "$answer" = "y" ]; then
        rm -rf $libraryFolder
    else
        exit 0
    fi
fi

# Download $archive if it does not exist
if [ ! -e "$archive" ]; then
    wget "$url"
fi

# Extract $archive if folder does not exist
if [ ! -d "$filename" ]; then
    tar -xzf "$archive"
fi

# Delete build
rm -rf "$buildFolder"
mkdir "$buildFolder"

cmake   -DENABLE_ECCODES_THREADS=ON             \
        -DENABLE_FORTRAN=OFF                    \
        -DBUILD_SHARED_LIBS=ON                  \
        -DCMAKE_INSTALL_PREFIX="$libraryFolder" \
        -DECBUILD_2_COMPAT_DEPRECATE=OFF        \
        -DENABLE_BUILD_TOOLS=OFF                \
        -DENABLE_EXAMPLES=OFF                   \
        -DENABLE_INSTALL_ECCODES_SAMPLES=OFF    \
        -DENABLE_PRODUCT_BUFR=OFF               \
        -S "$filename"                          \
        -B "$buildFolder"

# ccmake "$buildFolder"
make -C "$buildFolder" -j "$threads"
make install -C "$buildFolder" -j "$threads"

# Delete residual files
rm -rf "$buildFolder" "$filename" "$archive"