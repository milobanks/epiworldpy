# epiworldpy: Python bindings for epiworld


| CI           | status                                                                                                                                                                             |
|--------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| conda.recipe | [![Conda Actions Status](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/conda.yml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions?query=workflow%3AConda) |
| pip builds   | [![Pip Actions Status](https://github.com/UofUEpiBio/epiworldpy/actions/workflows/pip.yml/badge.svg)](https://github.com/UofUEpiBio/epiworldpy/actions?query=workflow%3APip)       |

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

    Global events:
     - Update infected individuals (runs daily)

    Virus(es):
     - covid-19 (baseline prevalence: 1.00%)

    Tool(s):
     (none)

    Model parameters:
     - Avg. Incubation days : 7.0000
     - Contact rate         : 2.0000
     - Prob. Recovery       : 0.1400
     - Prob. Transmission   : 0.1000

    <epiworldpy._core.ModelSEIRCONN at 0x10899ebb0>

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
    Last run elapsed t  : 14.00ms
    Last run speed      : 71.15 million agents x day / second
    Rewiring            : off

    Global events:
     - Update infected individuals (runs daily)

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
      - (0) Susceptible :  9900 -> 7275
      - (1) Exposed     :   100 -> 269
      - (2) Infected    :     0 -> 292
      - (3) Recovered   :     0 -> 2164

    Transition Probabilities:
     - Susceptible  1.00  0.00  0.00  0.00
     - Exposed      0.00  0.85  0.15  0.00
     - Infected     0.00  0.00  0.86  0.14
     - Recovered    0.00  0.00  0.00  1.00

    <epiworldpy._core.ModelSEIRCONN at 0x10899ebb0>

Let’s visualize the resulting time series

``` python
import numpy as np
import matplotlib.pyplot as plt

# Get the data from the database
history = covid19.getDb().getHistTotal()

# Extract unique states and dates
unique_states = np.unique(history['states'])
unique_dates = np.unique(history['dates'])

# Initialize a dictionary to store time series data for each state
time_series_data = {state: [] for state in unique_states}

# Populate the time series data for each state
for state in unique_states:
  for date in unique_dates:
    # Get the count for the current state and date
    mask = (history['states'] == state) & (history['dates'] == date)
    count = history['counts'][mask][0]
    time_series_data[state].append(count)

# Start the plotting!
plt.figure(figsize=(10, 6))

for state in unique_states:
  plt.plot(unique_dates, time_series_data[state], marker='o', label=state)

plt.xlabel('Time')
plt.ylabel('Count')
plt.title('Time Series of States')
plt.legend()
plt.grid(True)
plt.show()
```

![The data resulting from the COVID-19 SEIR model
run](README_files/figure-commonmark/series-visualization-output-1.png)

# Acknowledgements
