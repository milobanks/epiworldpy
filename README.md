# epiworldpy: Python bindings for defm

| CI           | status                                                                                                                                                                     |
|--------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| conda.recipe | [![Conda Actions Status](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/conda.yml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions?query=workflow%3AConda) |
| pip builds   | [![Pip Actions Status](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions?query=workflow%3APip)       |

This is a python wrapper of the [`epiworld c++`
library](https://github.com/USCbiostats/epiworld), and in particular, of
the `defm` module that provides utilities for fitting discrete
exponential family models. This is possible using the
[`pybind11`](https://pybind11.readthedocs.io/en/stable/) library (which
rocks!).

The `defm` module is already
<a href="https://github.com/UofUEpiBio/defm" target="_blank">implemented
in R</a> and it can be used to count motifs and build Markov Random
fields and other models like ERGMs. More information in [this
preprint](https://arxiv.org/abs/2211.00627).

# Installation

- clone this repository
- `pip install ./epiworldpy`

# Examples

## Base setup

Here we show how to create a `defm` object and add terms to it. We will
use the following data:

``` python
# Loading the module
import epiworldpy as m
import numpy as np

# Creating a binary array
y = np.column_stack(
  (np.array([0, 0, 1, 1, 1, 1]),
  np.array([0, 1, 1, 0, 0, 1]))
)

# Adding some random X plus the ids
x = np.array([1, 2.0, 3.4, 3, 1, 2])
id = np.array([1, 1, 1, 2, 2, 2])
```

Once we have the needed data – the binary array `y`, the covariates `x`
and the ids `id` – we can create a `defm` object.

``` python
# Creating the object
obj = m.new_defm(id, y, x, column_major = False)

# Printing the object on screen shows it is a pointer
obj
```

    <epiworldpy._core.DEFM at 0x7f0bd16949b0>

Adding terms via formula

``` python
m.term_formula(obj, "{y0}")
m.term_formula(obj, "{y1}")
m.term_formula(obj, "{0y0, y1}")
obj.init()
obj.print() 
```

    Num. of Arrays       : 6
    Support size         : 6
    Support size range   : [4, 4]
    Arrays in powerset   : 24
    Transform. Fun.      : no
    Model terms ( 3)    :
     - Logit intercept y0
     - Logit intercept y1
     - Motif {y0⁻, y1⁺}
    Model rules (1)    :
     - Markov model of order 0
    Model Y variables (2):
      0) y0
      1) y1

``` python
counts = m.get_stats(obj)
counts
```

    array([[0., 0., 0.],
           [0., 1., 1.],
           [1., 1., 0.],
           [1., 0., 0.],
           [1., 0., 0.],
           [1., 1., 0.]])

Notice the first two columns coincide with the `y` array:

``` python
y
```

    array([[0, 0],
           [0, 1],
           [1, 1],
           [1, 0],
           [1, 0],
           [1, 1]])

Computing likelihood

``` python
par = np.array([.5, -.5, 1.0])
obj.likelihood(par, as_log = True)

from scipy.optimize import minimize

# Function to find the MLE
def mle(par):
  return -obj.likelihood(par, as_log = True)

def defm_mle(par):
  res = minimize(mle, par)
  return {
    "par": res.x,
    "or": np.exp(res.x),
    "se": np.sqrt(np.diag(res.hess_inv)),
    "ll": -res.fun,
    "optimres": res
  }

# Finding the MLE
res = defm_mle(np.array([.5, -.5, 1.0]))
res
```

    {'par': array([ 6.93148905e-01, -1.45864412e-05,  1.43467100e-05]),
     'or': array([2.00000345, 0.99998541, 1.00001435]),
     'se': array([1.24166077, 1.01824626, 1.7324735 ]),
     'll': -7.9779680932546455,
     'optimres':   message: Optimization terminated successfully.
       success: True
        status: 0
           fun: 7.9779680932546455
             x: [ 6.931e-01 -1.459e-05  1.435e-05]
           nit: 7
           jac: [ 0.000e+00 -3.815e-06  5.722e-06]
      hess_inv: [[ 1.542e+00 -4.843e-01  1.528e+00]
                 [-4.843e-01  1.037e+00 -9.952e-01]
                 [ 1.528e+00 -9.952e-01  3.001e+00]]
          nfev: 32
          njev: 8}

And simulating

``` python
m.simulate(obj, par)
```

    array([[0, 0],
           [0, 1],
           [1, 0],
           [1, 1],
           [1, 1],
           [1, 0]], dtype=int32)

# Acknowledgements

This port work was supported by the Office of the Assistant Secretary of
Defense for Health Affairs through the Epilepsy Research Program under
Award No. HT94252310221. Opinions, interpretations, conclusions, and
recommendations are those of the author and are not necessarily endorsed
by the Department of Defense.
