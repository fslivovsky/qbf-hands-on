# QBF Hands-On Session

Below, you'll find links to generators, benchmarks, solvers, preprocessors, and (further down), a Python interface to a quantified CDCL solver.

## Crafted Formula Generator

https://github.com/marseidl/qbffam.git

## Benchmarks

- https://qbf23.pages.sai.jku.at/gallery/qdimacs20.tar.zst
- https://qbf23.pages.sai.jku.at/gallery/qdimacs.tar.xz
- https://qbf23.pages.sai.jku.at/gallery/qcir20.tar.zst
- https://qbf23.pages.sai.jku.at/gallery/qcir.tar.xz

## Prenex CNF (QDIMACS) Solvers

- https://github.com/ltentrup/caqe
- https://lonsing.github.io/depqbf/
- https://github.com/fslivovsky/qute
- https://github.com/MikolasJanota/qesto

## Prenex Non-CNF (QCIR) Solvers

- https://github.com/ltentrup/quabs
- https://github.com/MikolasJanota/qfun
- https://github.com/fslivovsky/qute
- https://github.com/MikolasJanota/cqesto

## Preprocessors

- https://fmv.jku.at/bloqqer/
- https://projects.informatik.uni-freiburg.de/projects/dqbf/files (HQSPre)

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
from depqbf_module import DepQBF
```
You can create an instance of the solver, optionally passing a set of clauses (as a list of lists) to the constructor:

```python
s = DepQBF(bootstrap_with=[[1, 2], [1, -2]])
```

Variables that are not explicitly quantified are implicitly declared as existentially quantified at the outermost level. Accordingly, the formula in the above snippet is true (satisfiable):

```python
s.solve()
```
```plaintext
True
```

To create a *real* QBF, we first have to define the quantifier prefix by adding quantifier blocks, starting with the outermost (leftmost) quantifier block. For instance, we can create a universal quantifier block for variable 1, and an inner existential quantifier block containing variable 2. Note that we have to create a new solver instance since variable 1 has been (implicitly) declared as existential.

```python
s = DepQBF()
s.add_quantifier_block(False, [1])
s.add_quantifier_block(True, [2])
```

The first argument of the `add_quantifier_block` method is a Boolean value that indicates whether the quantifier block is existential (or universal).
Having declared our variables, we can now add the same clauses as before and call solve again:

```python
s.append_formula([[1,2],[1,-2])
s.solve()
```
```plaintext
False
```

If the QBF is false and the first quantifier block is universal, we can read out the values of these variables in a universal winning strategy:

```python
s.get_outermost_model()
```
```plaintext
[-1]
```

We can also solve a QBF under a set of assumptions:

```python
s.solve([1])
```
```plaintext
True
```

Afterwards, we can extract a subset of relevant assumptions:

```python
s.get_core()
```
```plaintext
[1]
```
