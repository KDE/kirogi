# USAGE
## Building
From docker directory
````
docker build -f Dockerfile . -t kirogi
````

## Using
From kirogi root
````
docker run -p 8083:8083 --rm -it -v `pwd`:/kirogi kirogi
````

Then, use your browser at `http://localhost:8083/vnc.html`.
````
cd kirogi
mkdir build
cd build
cmake ..
make
make install
kirogi
````
