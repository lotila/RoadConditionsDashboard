# RoadConditionsDashboard

A piece of software for monitoring how weather affects road maintenance and condition.

Link to prototype design project in Figma:
[PrototypeDesign](https://www.figma.com/file/A3VuGjqxbROXrJS2WUY8wV/UI-Proto-0.1)


## About project

The software helps users to monitor weather and road conditions in Finland. Users can study how different weather conditions affect road maintenance requirements. The software fetches data such as weather forecasts, temperature, visibility, road slippage, road traffic, and traffic cameras. The road data is fetched from Digitraffic and weather data from Ilmatieteenlaitos. The software allows users to select locations and study data in graphical format. Users can also save data for later studies.

## Version Control Guidelines

* Do not commit to main branch unless it is merge from dev branch and tagged properly
* Do not commit to dev branch, do a merge request and wait for approval from peer
* Use sensible branch names
* Git is not for binary files (except some GUI images etc.)
* Break those rules when necessary

## Dependencies
### Tools

* cmake 3.16 or higher
* proper compiler (GCC 9.4 and MSVC)

### Libs

* Qt6.3
* libgl1-mesa-dev (needed by QtWidget)
* libcurl (tested with 7.68.0)
* TinyXML-2 9.0 (in repository)
* json from Niels Lohmann (in repository)

## Build
To build with cmake you should have Qt6 library path available. (QT_INSTALLATION in this example)

```
QT_INSTALLATION=~/bin/Qt/6.3.1/gcc_64/
# or ~/bin/Qt/6.3.1/gcc_64/lib/cmake  (depends on qt installation, maybe)

# Generate build system files
cmake -D CMAKE_PREFIX_PATH=$QT_INSTALLATION -S . -B build

# build
cd build
make

# test
./RoadConditionsDashboard
```

