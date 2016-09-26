# hyphaplugins

Plugins for the hypha framework.

## Installation

```{r, engine='bash', count_lines}
git clone https://github.com/hyphaproject/hypha.git
cd hypha
mkdir build
cd build
cmake ..
make
cd ../..

git clone https://github.com/hyphaproject/hyphaplugins.git
cd hyphaplugins
mkdir build
cd build
cmake -Dhypha_DIR=../../hypha ..
make
```
