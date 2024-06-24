# epiworldpy: Python bindings for epiworld


[![Pip
Build](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yaml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yaml)

This is a python wrapper of the [`epiworld c++`
library](https://github.com/UofUEpiBio/epiworld), an ABM simulation
engine. This is possible using the
[`pybind11`](https://pybind11.readthedocs.io/en/stable/) library (which
rocks!).

The `epiworld` module is already
<a href="https://github.com/UofUEpiBio/epiworldR"
target="_blank">implemented in R</a>.

# Installation

- clone this repository
- `pip install ./epiworldpy`

# Examples

## Base setup

Here we show how to create a `SEIR` object and add terms to it. We will
use the following data:

``` python
# Loading the module
import epiworldpy as m
covid19 = m.ModelSEIR(
  name              = 'covid-19',
  n                 = 10000,
  prevalence        = .01,
  contact_rate      = 2.0,
  transmission_rate = .1,
  incubation_days   = 7.0,
  recovery_rate     = 0.14
)
```

We can now take a look at the model

``` python
# Creating the object
covid19.print(False)
```

    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : Susceptible-Exposed-Infected-Removed (SEIR) (connected)
    Population size     : 10000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 0 (of 0)
    Number of viruses   : 1
    Last run elapsed t  : -
    Rewiring            : off

    Global actions:
     (none)

    Virus(es):
     - covid-19 (baseline prevalence: 1.00%)

    Tool(s):
     (none)

    Model parameters:
     - Avg. Incubation days : 7.0000
     - Contact rate         : 2.0000
     - Prob. Recovery       : 0.1400
     - Prob. Transmission   : 0.1000

    <epiworldpy._core.ModelSEIRCONN at 0x10522f7f0>

And run it and see what we get

``` python
covid19.run(100, 223)
covid19.print(False)
```

    _________________________________________________________________________
    Running the model...
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
     done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : Susceptible-Exposed-Infected-Removed (SEIR) (connected)
    Population size     : 10000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 100 (of 100)
    Number of viruses   : 1
    Last run elapsed t  : 49.00ms
    Last run speed      : 20.34 million agents x day / second
    Rewiring            : off

    Global actions:
     (none)

    Virus(es):
     - covid-19 (baseline prevalence: 1.00%)

    Tool(s):
     (none)

    Model parameters:
     - Avg. Incubation days : 7.0000
     - Contact rate         : 2.0000
     - Prob. Recovery       : 0.1400
     - Prob. Transmission   : 0.1000

    Distribution of the population at time 100:
      - (0) Susceptible :  9900 -> 7500
      - (1) Exposed     :   100 -> 261
      - (2) Infected    :     0 -> 238
      - (3) Recovered   :     0 -> 2001

    Transition Probabilities:
     - Susceptible  1.00  0.00  0.00  0.00
     - Exposed      0.00  0.86  0.14  0.00
     - Infected     0.00  0.00  0.86  0.14
     - Recovered    0.00  0.00  0.00  1.00

    <epiworldpy._core.ModelSEIRCONN at 0x10522f7f0>

# Acknowledgements
