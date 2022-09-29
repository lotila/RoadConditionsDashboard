# RoadConditionsDashboard

A piece of software for monitoring how weather affects road maintenance and condition.

Link to prototype design project in Figma:
[PrototypeDesign](https://www.figma.com/file/A3VuGjqxbROXrJS2WUY8wV/UI-Proto-0.1)


## About project

## Version Control Guidelines

* Do not commit to main branch unless it is merge from dev branch and tagged properly
* Do not commit to dev branch, do a merge request and wait for approval from peer
* Use sensible branch names
* Git is not for binary files (except some GUI images etc.)
* Break those rules when necessary

## Dependencies
### Tools

* cmake 3.16 or higher

### Libs

* Qt6.3
* TinyXML-2 9.0

## Build
To build with cmake you should have Qt6 library path available. (QT_INSTALLATION in this example)

```
QT_INSTALLATION=~/bin/Qt/6.3.1/gcc_64/

# Generate build system
cmake -D CMAKE_PREFIX_PATH=$QT_INSTALLATION -S . -B build

# build
cd build
make

# test
./RoadConditionsDashboard
# or without gui
./RoadConditionsDashboard --no-gui
```

