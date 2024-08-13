
# DepQBF Python Wrapper

This repository provides a simple Python wrapper for the incremental interface of the Quantified CDCL solver [DepQBF](https://lonsing.github.io/depqbf/).

## Installation

### Step 1: Install pybind11

Assuming that `pip` is installed, you can install `pybind11` with the following command:

```bash
pip install pybind11
```

### Step 2: Clone the Repository

Download the repository, including the submodules, by running:

```bash
git clone --recursive https://github.com/fslivovsky/depqbf-python.git
```

### Step 3: Download Additional Requirements

Navigate to the folder and run the following command to download Nenofex and PicoSAT, two dependencies required by DepQBF:

```bash
sh download_requirements.sh
```

### Step 4: Build the Package

Create a build directory, run CMake, and make the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

You should now be able to import the `depqbf_module` package in Python:

```python
import depqbf_module
```
